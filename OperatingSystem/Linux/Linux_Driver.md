# Linux_Driver

---- Chime Lee

## 一、概述

**Linux的驱动开发的特点**：

* 相同点：裸机、`FreeRTOS`的核心逻辑是相通的，主要区别在于**代码架构**不同。

* 在大型项目中，若更换`MCU`对于`FreeRTOS`和裸机，全部都要推倒重来。

**分层**：

```txt
用户程序 read/write → 内核 file_operations → 驱动逻辑 → transfer API → MCU 硬件操作
```

* **设备驱动层**
* **核心API层**
* **底层控制器驱动层**

**分离**：

* **设备驱动**和**控制器驱动**分离 。

* 将**初始化所需数据保存在设备树**中，而**代码就是驱动**。

**设备分类**：

* **字符型设备**（按比特操作）
  * **执行操作**：`open/read/write/close`等
  * **核心结构体**：`file_operations`
* **块设备**：硬盘、EMMC、Flash、U盘
  * **执行操作**：`open/read/write/close`等
  * **核心结构体**：`block_operations`
* **网络设备**：网卡、蓝牙、`Wifi`
  * **核心结构体**：`net_operations`


**Linux驱动开发过程**：

* **修改设备树**：包含设备硬件所有参数的，进行设备初始化。
* **编写驱动**：读写
* **中断**：中断处理

**Linux驱动开发基本知识**：

* 所有**硬件操作**都抽象成对**文件操作**。
* 所有的设备，在`dev/`目录下都会有对应文件。会有主设备号（设备大类）子设备号（设备的第几个）

**`~/.bashrc`作用**：`bash shell`用户级配置文件，`rc`是`run commands`的缩写，

* 每次启动**非登陆时Bash终端**时，`Bash`会自动加载并执行这个文件里的命令。

## 二、文件句柄的前世今生

**概述**：应用层打开文件时，都会得到一个文件句柄，每个文件句柄，在内核中都有一个`struct file`与之对应。

```cpp
struct file {
	union {
		struct llist_node	fu_llist;
		struct rcu_head 	fu_rcuhead;
	} f_u;
	struct path		f_path;
	struct inode		*f_inode;	/* cached value */
	const struct file_operations	*f_op;		

	/*
	 * Protects f_ep_links, f_flags.
	 * Must not be taken from IRQ context.
	 */
	spinlock_t		f_lock;
	atomic_long_t		f_count;
	unsigned int 		f_flags;
	fmode_t			f_mode;
	struct mutex		f_pos_lock;
	loff_t			f_pos;
	struct fown_struct	f_owner;
	const struct cred	*f_cred;
	struct file_ra_state	f_ra;

	u64			f_version;
#ifdef CONFIG_SECURITY
	void			*f_security;
#endif
	/* needed for tty driver, and maybe others */
	void			*private_data;

#ifdef CONFIG_EPOLL
	/* Used by fs/eventpoll.c to link all the hooks to this file */
	struct list_head	f_ep_links;
	struct list_head	f_tfile_llink;
#endif /* #ifdef CONFIG_EPOLL */
	struct address_space	*f_mapping;
} __attribute__((aligned(4)));	/* lest something weird decides that 2 is OK */
```

* `int open(const char* pathname, int flags, mode_t mode);`中的内容也会保存在`f_path`、`f_flags`、`f_mode`。

* `open`呢：

  * 根据主设备号，在chardevs[]，中找到`file_operations`执行其中的`open`函数。
  * `file_operations`：

  ```c
  struct file_operations {
  	struct module *owner;
  	loff_t (*llseek) (struct file *, loff_t, int);
  	ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
  	ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
  	ssize_t (*read_iter) (struct kiocb *, struct iov_iter *);
  	ssize_t (*write_iter) (struct kiocb *, struct iov_iter *);
  	int (*iterate) (struct file *, struct dir_context *);
  	int (*iterate_shared) (struct file *, struct dir_context *);
  	unsigned int (*poll) (struct file *, struct poll_table_struct *);
  	long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
  	long (*compat_ioctl) (struct file *, unsigned int, unsigned long);
  	int (*mmap) (struct file *, struct vm_area_struct *);
  	int (*open) (struct inode *, struct file *);
  	int (*flush) (struct file *, fl_owner_t id);
  	int (*release) (struct inode *, struct file *);
  	int (*fsync) (struct file *, loff_t, loff_t, int datasync);
  	int (*fasync) (int, struct file *, int);
  	int (*lock) (struct file *, int, struct file_lock *);
  	ssize_t (*sendpage) (struct file *, struct page *, int, size_t, loff_t *, int);
  	unsigned long (*get_unmapped_area)(struct file *, unsigned long, unsigned long, unsigned long, unsigned long);
  	int (*check_flags)(int);
  	int (*flock) (struct file *, int, struct file_lock *);
  	ssize_t (*splice_write)(struct pipe_inode_info *, struct file *, loff_t *, size_t, unsigned int);
  	ssize_t (*splice_read)(struct file *, loff_t *, struct pipe_inode_info *, size_t, unsigned int);
  	int (*setlease)(struct file *, long, struct file_lock **, void **);
  	long (*fallocate)(struct file *file, int mode, loff_t offset,
  			  loff_t len);
  	void (*show_fdinfo)(struct seq_file *m, struct file *f);
  #ifndef CONFIG_MMU
  	unsigned (*mmap_capabilities)(struct file *);
  #endif
  	ssize_t (*copy_file_range)(struct file *, loff_t, struct file *,
  			loff_t, size_t, unsigned int);
  	int (*clone_file_range)(struct file *, loff_t, struct file *, loff_t,
  			u64);
  	ssize_t (*dedupe_file_range)(struct file *, u64, u64, struct file *,
  			u64);
  };
  ```


## 三、内核的操作

### 3.1 编译

```bash
# 清除上一次编译生成的过程文件
make mrproper

# arc/arm/config/..._defconfig 各厂家开发板的编译内核的配置,。根据裁剪好的配置文件生成.config文件，其用于下一步make zImage编译内核所用的缺省配置文件。
make 100ask_imx6ull_defconfig

# 内核裁剪（可选）
make menuconfig

# 编译内核镜像 arch/arm/boot/zImage
make zImage -j8

# 编译设备树 arch/arm/boot/dts
make dtbs

# 将生成好的二进制文件，都拷贝到某个目录下，方便拷贝到板子上。
cp -rf ./arch/arm/boot/zImage ~/nfs_rootfs/
cp -rf ./arch/arm/boot/dts/100ask_imx6ull-14x14.dtb ~/nfs_rootfs/

# 编译之前的一些模块（驱动） 
make modules

# 安装模块
    sudo make ARCH=arm INSTALL_MOD_PATH=/home/book/nfs_rootfs modules_install
```

### 3.2 安装

**设备挂载**：

```bash
mount -t nfs -o nolock,vers=3 192.168.5.11:/home/book/nfs_rootfs /mnt
```

**内容拷贝**：

```bash
cp  /mnt/zImage  /boot
cp  /mnt/*.dtb   /boot
cp  /mnt/lib/modules  /lib  -rfd
```

**动态加载**：****

* **安装驱动**：

```bash
insmod -f xxx.ko
```

* **删除驱动**：

```bash
rmmod -f xxx.ko
```

### 3.3 测试

## 四、设备树

**设备树**：Device Tree Source

* 保存设备所有**硬件初始化参数文件**，主要是为了**完成初始化**。

**路径**：`arch/arm/boot/dts/***.dtb`

**设备树属性**：

* `compatible`（兼容属性）：格式是**字符串**，用来描述“**该硬件设备兼容那些软件驱动**”。本质是建立“硬件设备”和“内核驱动”直接的匹配桥梁。

  * 与驱动中`device_driver`中的`out_of_match`：中的`compatible`匹配

  ```c
  struct device_driver {
  	const char		*name;
  	struct bus_type		*bus;
  
  	struct module		*owner;
  	const char		*mod_name;	/* used for built-in modules */
  
  	bool suppress_bind_attrs;	/* disables bind/unbind via sysfs */
  	enum probe_type probe_type;
  
  	const struct of_device_id	*of_match_table;
  	const struct acpi_device_id	*acpi_match_table;
  
  	int (*probe) (struct device *dev);
  	int (*remove) (struct device *dev);
  	void (*shutdown) (struct device *dev);
  	int (*suspend) (struct device *dev, pm_message_t state);
  	int (*resume) (struct device *dev);
  	const struct attribute_group **groups;
  
  	const struct dev_pm_ops *pm;
  
  	struct driver_private *p;
  };
  
  struct of_device_id {
  	char	name[32];
  	char	type[32];
  	char	compatible[128];
  	const void *data;
  };
  ```

* `reg`：`<0x0...., 0x4000>` 控制器所有寄存器的基地址，大小。

* `status`：“okay” 使能。

* `interrupts`：`<GIC_SPI 66 IRQ_TYPE_LEVEL_HIGH>` 中断类型，中断号（本身是66+32），触发方式。

* **信号和引脚**：

  * 定义引脚组

  ```bash
  pinctrl_i2c1: i2c1grp(引脚组) {
      fsl,pins = <
          MX6UL_PAD_UART4_TX_DATA__I2C1_SCL 0x4001b8b0	// 多路复用，电气参数配置
          MX6UL_PAD_UART4_RX_DATA__I2C1_SDA 0x4001b8b0
          >;
  };
  // <mux_reg conf_reg input_reg mux_mode input_val>
  // MX6UL_PAD_UART4_TX_DATA__I2C1_SCL  0x00b4 0x0340 0x05a4 2 1
  ```

  * 调用 ：

  ```bash
  &i2c1 {
      clock-frequency = <100000>;		// 时钟频率
      pinctrl-names = "default";		// pinctrl操作名称
      pinctrl-0 = <&pinctrl_i2c1>;	// pinctrl-0的
      status = "okay";				// 使能
  };
  ```

**从设备树角度设备分类**：

* **平台设备**：其会被展开为`plateform_device`结构体。
* **总线设备**：对应总线对应展开 i2c：`i2c_client`、spi：`spi_device`。

**设备树编译过程**：

```bas
DTS -> DTC -> DTB
```

* DTS (Device Tree Source)：设备树源码
* DTC (Device Tree Compiler)：设备树编译器
* DTB (Device Tree Blob (Binary large object))：设备树二进制大对象。

## 五、驱动编写

### 5.1 静态加载： 

* **一次注册**：向**内核注册驱动结构体**，实现**设备树**中**硬件设备信息**与**软件驱动**匹配。

  * **声明并定义匹配设备树`compatible`结构体`id_of_device`：**

  ```c
  static const struct of_device_id ask100_keys[] = {
      { .compatible = "厂家,驱动名称" },
  };
  ```

  * **声明并定义软件驱动结构体**：
    * **总线设备**：`platform_driver`；
    * **其他设备** ：`spi_driver`、`i2c_driver`；
  * **向内核注册驱动结构体**，触发**总线匹配机制**，实现**硬件设备与软件驱动的匹配**，匹配成功后回调用`probe()`函数：
    * **总线设备**：`platform_driver_register()`;
    * **其他设备**：`spi_register_driver`、`宏：module_i2c_driver`;

* **二次注册**：`probe`

  * **参数**：
    * **平台设备**：`plateform_deivce* dev`
    * **总线设备**：`i2c_client* client`、`spi_device *spi`
  * **硬件设备初始**：
    * **从设备树中解析硬件参数**：`reg`、`interrputs`、`gpios`、`frequency`
    * **硬件设备初始化**：若有外挂设备，对其进行初始化。
  * **软件层面的初始化**：
    * **构建设备号**：（主设备号 + 次设备号`MKDEV()`）
      * 动态申请：`alloc_chrdev_region()`；
      * 静态申请：`register_chrdev_region()`；手动指定
    * **初始化私有数据**：分配驱动私有结构体`kzalloc`。
    * **注册字符/块设备**：如果是字符设备通过`cdev_init`+`cdev_add`注册字符设备结构体`file_operations`关联到**设备节点**，块设备`bolck_operations`，网络设备`net_ops`。
    * **创建设备节点/类**：通过`class_create()`创建设备类、`device_create()`创建设备节点`/dev/xxx`，或用`sysfs_create_file()`创建`sysfs`节点。

* `KConfig`：定义驱动的**配置选项**，决定是否在编译菜单中显示。

```bash
config MY_STATIC_DRIVER
    tristate "My Static Load Driver"
    help
      This is a test driver for static loading.
      If you want to compile it into the kernel, select Y;
      If you want to compile as a module, select M;
      If not, select N.
```

* `Makefile`：关联配置与驱动源码，静态加载时用`obj-y`（动态加载用`obj-m`）

```cpp
# make menuconfig中选择了My Static Load Driver，编译内核镜像时会何如内容
obj-$(CONFIG_MY_STATIC_DRIVER) += board_100ask_imx6ull.o leddrv.o
```

### 5.2 动态加载：

**动态加载**：

**初始化**：

* **函数顺序**：`module_init`->`led_init`->`register_chrdev`->`class_create`->`get_board_led_opr`->`device_create`。
* **逻辑顺序**：模块初始化宏->模块初始化函数->注册字符型设备->创建大类->获取硬件操作结构体指针->创建设备文件。

**打开`open`**：

* **函数顺序**：`open`->`file_opeartions.open`->`led_drv_open`->`iminor()`->`p_led_ope->init(minor)`。
* **逻辑顺序**：系统调用`open`->`file_operations`中的`open`->注册的打开操作`led_drv_open`->获取子设备号`iminor(node)`->初始化获取硬件操作指针的`init()`。

**打开`wirte`**：

* **函数顺序**：`open`->`file_opeartions.write`->`led_drv_write`->`file_inode`->`iminor()`->`copy_from_user`->`p_led_ope->init(minor)`。
* **逻辑顺序**：系统调用`wirte`->`file_operations`中的`write`->注册的写操作`led_drv_write`->获取文件节点号`file_inode`->获取子设备号`iminor(node)`->**从用户空间将数据拷贝到内核**->初始化获取硬件操作指针的中`ctl`。

### 5.3 设备节点注册的过程：

**平台设备**：

* `platform_driver_register()`

* `__platform_driver_register(drv, THIS_MODULE)`，在这里填充了`plateform_driver`中`device_driver`的`bus_type* `的成员`&platform_bus_type`。
* `driver_register(&drv->driver)`;
* `bus_add_driver(drv)`;
* `driver_attach(drv);`
* `bus_for_each_dev(drv->bus, NULL, drv, __driver_attach)`; `__dirver_attch`注册回调函数。
* 回调函数中执行：`driver_match_device(drv, dev)`：

```c
drv->bus->match ? drv->bus->match(dev, drv) : 1;
```

* 执行`platform_bus_type`中`platform_match`：

```c
static int platform_match(struct device *dev, struct device_driver *drv)
{
    // 这里使用container_of通过结构体成员变量的地址倒推出整个结构体的地址。
	struct platform_device *pdev = to_platform_device(dev);
	struct platform_driver *pdrv = to_platform_driver(drv);

	/* When driver_override is set, only bind to the matching driver */
	if (pdev->driver_override)
		return !strcmp(pdev->driver_override, drv->name);

	/* Attempt an OF style match first */
	if (of_driver_match_device(dev, drv))
		return 1;

	/* Then try ACPI style match */
	if (acpi_driver_match_device(dev, drv))
		return 1;

	/* Then try to match against the id table */
	if (pdrv->id_table)
		return platform_match_id(pdrv->id_table, pdev) != NULL;

	/* fall-back to driver name match */
	return (strcmp(pdev->name, drv->name) == 0);
}
```


## 六、内核函数

**`module_init`作用：**

宏、执行`insmod`时会自动调用`module_init`宏配置的函数。

**`module_exit`作用：**

宏、执行`rmmod`时会自动执行`module_exit`宏配置的函数。

**`lsmodule`作用**：

显示所有安装的文件

**`demesg`作用**：

显示内核调试信息

**`printk`作用：**

打印内核消息。

**`register_chrdev`作用**：注册字符串设备

```c
/**
 * @brief 字符型设备注册函数
 * @param [in] major 主设备号，传入0时，系统自动分配
 * @param [in] name 设备名称
 * @param [in] file_operations 字符型设备核心结构体的指针
 * @return 分配到主设备号
 */
static inline int register_chrdev(unsigned int major, const char *name,
				  const struct file_operations *fops)
```

`class_create`：类创建。

`device_create`：设备创建。

`iminor`：从`struct inode`中获取获取子节点。

`file_inode`：从`struct file`中获取`struct inode`。

**数据交互**：

`copy_from_user`：从用户态拷贝数据到内核态。

`copy_to_user`：将内核数据拷贝置用户态。

**内核内存分配函数**：

| 函数名    | 说明                                      |
| --------- | ----------------------------------------- |
| `kmalloc` | 分配的内存物理地址是连续的                |
| `kzalloc` | 分配的内存物理地址是连续的，内容清0       |
| `vmalloc` | 分配的内存物理地址不保证是连续的          |
| `vzalloc` | 分配的内存物理地址不保证是连续的，内容清0 |

## X、问题：

### X.1 设备树如何初始化？

* **初始化**：
  * **多路复用和电器参数配置**：
    * 查看电路图，使用`pinctrl: xxxgrp`声明引脚组，多路复用信号并，配置电器参数。
    * 在节点中使用`pinctrl-name`和`pinctrl-0`引用引脚组。
  * **参数配置：**有从机在节点中配置从机。
  * **时钟和使能**：
    * 时钟：`clock = <>` 和`clock-name = "xxx"`;
    * 使能：`statuc = "okay"`;
  * **中断的配置**：
    * `interrupt-parents`
    * `interrupts = <中断类型（SGI，PPI，SPI），对应类型的第几个，触发方式>`;
  
* 

### X.2 更新镜像和设备树后，驱动没有安装。

* 查看.config。
* 查看产物
* 查看设备`ls /proc/device-tree`

### X.3 什么是设备树

### X.4 设备树有那些关键属性

初始化：

多路复用和电器参数配置

* `pinctrl`

参数配置

* `reg`

时钟和使能：

* `clock`
* `status`

中断：

* `interrupt`

兼容性：

* `compatible`

### X.5 应用层和驱动通讯的IO模型

* 阻塞式I/O：`wait_event_interrputible()`，`wake_up_interruptible()`
* 轮询：`read`、`write`;
* 多路IO复用：`poll`
* 异步信号：`signal`
* `ioctl()`：从内核中读取数据，或向内核中写入数据。

### X.6 设备和驱动的匹配方式：

* 强制匹配：设备的驱动名和软件驱动的驱动名匹配
* 基于设备树的`compatible`进行匹配。
* 基于`id_table`匹配。
* 硬件名称和软件驱动名匹配。
