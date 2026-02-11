# USB

---- Chime Lee

## 一、概述

**USB拓扑结构**：**主机（Host）- 集线器（Hub）-设备（Device）三级结构**，主机是唯一的控制方。

**USB设备层次**：

* **设备（Device）**：整个USB设备，对应驱动中`struct usb_deivce`;
* **配置（Configuration）**：设备的**工作模式**（如高速/低速），一个**设备可有多配置**，**同一时间仅激活一个**，对应`struct usb_host_config`;
* **接口（Inertface）**：设备的**功能单元**（鼠标的输入输出接口），一个**配置可有多个接口**，对应`struct usb_interface`;
* **端点（Endpoint）**：数据传出的**最小单元**，每个**接口有多个端点**，分为：
  * **控制端点（Endpoint 0）** ：双向，用于**设备枚举**，**命令交互**，所有设备必带；
  * **批量端点（Bulk）**：用于**大量数据传输（U盘）**，可靠但无实时性；
  * **中断端点（Interrupt）**：用于**小批量**，**实时性数据**（鼠标、键盘）；
  * **等待端点（Isochronous）**：用于**实时数据（摄像头、音频）**，**不保证可靠但保证时序**；

* **USB枚举过程**：主机通过`Endpoint 0`给设备下发命令，获取**设备描述符、配置描述符、接口描述符、端点描述符**，最终**分配地址并加载驱动**。

## 二、Linux USB驱动框架

### 2.1 驱动类型

* 设备驱动
* 主机控制驱动
* Hub驱动

### 2.2 核心数据结构

```c
// 1. USB设备驱动结构体（核心）
struct usb_driver {
    const char *name;                  // 驱动名称（面试会问：与设备匹配的关键）
    int (*probe)(struct usb_interface *intf, const struct usb_device_id *id);  // 设备匹配成功后执行（必写）
    void (*disconnect)(struct usb_interface *intf);  // 设备断开时执行（必写）
    const struct usb_device_id *id_table;            // 设备匹配表（必写，否则驱动不生效）
    // 其他可选成员
};

// 2. 设备匹配表（示例）
static const struct usb_device_id my_usb_id_table[] = {
    { USB_DEVICE(0x1234, 0x5678) },  // 匹配厂商ID=0x1234，产品ID=0x5678的设备
    { }  // 结束符
};
MODULE_DEVICE_TABLE(usb, my_usb_id_table);  // 必须声明，否则系统无法识别匹配表

// 3. 注册/注销驱动（模块入口/出口）
static int __init my_usb_driver_init(void) {
    return usb_register(&my_usb_driver);  // 注册驱动
}
static void __exit my_usb_driver_exit(void) {
    usb_deregister(&my_usb_driver);  // 注销驱动
}
module_init(my_usb_driver_init);
module_exit(my_usb_driver_exit);
```

### 2.3 数据传输接口

```cpp
// 1. 分配URB
struct urb *usb_alloc_urb(int iso_packets, gfp_t mem_flags);

// 2. 填充URB（以批量传输为例）
void usb_fill_bulk_urb(
    struct urb *urb,                // 待填充的URB
    struct usb_device *dev,         // 目标USB设备
    unsigned int pipe,              // 传输管道（由端点方向/类型生成）
    void *transfer_buffer,          // 数据缓冲区
    int buffer_length,              // 缓冲区长度
    usb_complete_t complete_fn,     // 传输完成回调函数
    void *context                   // 回调函数的上下文
);

// 3. 提交URB（发起传输）
int usb_submit_urb(struct urb *urb, gfp_t mem_flags);

// 4. 释放URB
void usb_free_urb(struct urb *urb);
```

### 2.4 `probe`函数

```cpp
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/slab.h>  // 用于kmalloc/kfree

// 自定义私有数据结构（面试高频：保存设备相关资源，避免全局变量）
struct usb_dev_private {
    struct usb_device *udev;          // 指向USB设备
    struct usb_interface *intf;       // 指向USB接口
    struct urb *bulk_in_urb;          // 批量输入URB
    struct urb *bulk_out_urb;         // 批量输出URB
    unsigned char *bulk_in_buf;       // 输入数据缓冲区
    unsigned char *bulk_out_buf;      // 输出数据缓冲区
    size_t bulk_in_size;              // 输入端点最大数据包长度
    size_t bulk_out_size;             // 输出端点最大数据包长度
    __u8 bulk_in_endpointAddr;        // 输入端点地址
    __u8 bulk_out_endpointAddr;       // 输出端点地址
};

// URB传输完成回调函数（中断上下文，面试注意：不能阻塞）
static void usb_bulk_complete(struct urb *urb) {
    struct usb_dev_private *dev_priv = urb->context;
    
    // 检查传输状态
    if (urb->status) {
        printk(KERN_ERR "URB传输失败: %d\n", urb->status);
        return;
    }

    // 示例：打印接收到的数据（仅演示，实际按需处理）
    if (urb == dev_priv->bulk_in_urb) {
        printk(KERN_INFO "接收到数据: %.*s\n", urb->actual_length, dev_priv->bulk_in_buf);
        // 重新提交URB，持续接收数据
        usb_submit_urb(dev_priv->bulk_in_urb, GFP_ATOMIC);
    }
}

// 解析端点描述符（面试高频：如何找到对应类型的端点）
static int parse_endpoints(struct usb_interface *intf, struct usb_dev_private *dev_priv) {
    struct usb_host_interface *interface_desc;
    struct usb_endpoint_descriptor *endpoint;
    int i;

    // 获取当前激活的接口描述符
    interface_desc = intf->cur_altsetting;

    // 遍历所有端点（Endpoint 0是控制端点，无需解析）
    for (i = 0; i < interface_desc->desc.bNumEndpoints; i++) {
        endpoint = &interface_desc->endpoint[i].desc;

        // 判断是否为批量输入端点（IN：设备→主机）
        if (usb_endpoint_is_bulk_in(endpoint)) {
            dev_priv->bulk_in_endpointAddr = endpoint->bEndpointAddress;
            dev_priv->bulk_in_size = usb_endpoint_maxp(endpoint);
            printk(KERN_INFO "找到批量输入端点: 0x%02x, 最大包长: %zd\n",
                   dev_priv->bulk_in_endpointAddr, dev_priv->bulk_in_size);
        }
        // 判断是否为批量输出端点（OUT：主机→设备）
        else if (usb_endpoint_is_bulk_out(endpoint)) {
            dev_priv->bulk_out_endpointAddr = endpoint->bEndpointAddress;
            dev_priv->bulk_out_size = usb_endpoint_maxp(endpoint);
            printk(KERN_INFO "找到批量输出端点: 0x%02x, 最大包长: %zd\n",
                   dev_priv->bulk_out_endpointAddr, dev_priv->bulk_out_size);
        }
    }

    // 检查是否找到必要的端点
    if (!dev_priv->bulk_in_endpointAddr || !dev_priv->bulk_out_endpointAddr) 	 {
        printk(KERN_ERR "未找到批量端点\n");
        return -ENODEV;
    }
    return 0;
}

// 核心probe函数（面试重点：逐行讲解）
static int usb_probe(struct usb_interface *intf, const struct usb_device_id *id) {
    struct usb_device *udev = interface_to_usbdev(intf);
    struct usb_dev_private *dev_priv;
    int ret;

    printk(KERN_INFO "USB设备匹配成功！厂商ID: 0x%04x, 产品ID: 0x%04x\n",
           le16_to_cpu(udev->descriptor.idVendor),
           le16_to_cpu(udev->descriptor.idProduct));

    // 1. 分配私有数据结构（GFP_KERNEL：进程上下文，可阻塞）
    dev_priv = kzalloc(sizeof(struct usb_dev_private), GFP_KERNEL);
    if (!dev_priv) {
        printk(KERN_ERR "分配私有数据失败\n");
        return -ENOMEM;
    }

    // 2. 初始化私有数据
    dev_priv->udev = usb_get_dev(udev);  // 引用计数+1，防止设备被释放
    dev_priv->intf = intf;
    usb_set_intfdata(intf, dev_priv);   // 将私有数据绑定到接口，方便disconnect获取

    // 3. 解析端点描述符
    ret = parse_endpoints(intf, dev_priv);
    if (ret) {
        goto err_free_priv;
    }

    // 4. 分配数据缓冲区（按端点最大包长分配）
    dev_priv->bulk_in_buf = kmalloc(dev_priv->bulk_in_size, GFP_KERNEL);
    dev_priv->bulk_out_buf = kmalloc(dev_priv->bulk_out_size, GFP_KERNEL);
    if (!dev_priv->bulk_in_buf || !dev_priv->bulk_out_buf) {
        printk(KERN_ERR "分配缓冲区失败\n");
        ret = -ENOMEM;
        goto err_free_buf;
    }

    // 5. 分配并初始化批量输入URB
    dev_priv->bulk_in_urb = usb_alloc_urb(0, GFP_KERNEL);  // 0：非等时传输
    if (!dev_priv->bulk_in_urb) {
        printk(KERN_ERR "分配输入URB失败\n");
        ret = -ENOMEM;
        goto err_free_buf;
    }
    // 填充URB（核心API，面试必问）
    usb_fill_bulk_urb(
        dev_priv->bulk_in_urb,          // 待填充的URB
        udev,                           // 目标USB设备
        usb_rcvbulkpipe(udev, dev_priv->bulk_in_endpointAddr),  // 批量输入管道
        dev_priv->bulk_in_buf,          // 接收缓冲区
        dev_priv->bulk_in_size,         // 缓冲区大小
        usb_bulk_complete,              // 完成回调函数
        dev_priv                        // 回调上下文（私有数据）
    );

    // 6. 分配并初始化批量输出URB（示例）
    dev_priv->bulk_out_urb = usb_alloc_urb(0, GFP_KERNEL);
    if (!dev_priv->bulk_out_urb) {
        printk(KERN_ERR "分配输出URB失败\n");
        ret = -ENOMEM;
        goto err_free_in_urb;
    }
    usb_fill_bulk_urb(
        dev_priv->bulk_out_urb,
        udev,
        usb_sndbulkpipe(udev, dev_priv->bulk_out_endpointAddr),  // 批量输出管道
        dev_priv->bulk_out_buf,
        dev_priv->bulk_out_size,
        usb_bulk_complete,
        dev_priv
    );

    // 7. 提交输入URB，开始接收数据（GFP_ATOMIC：可能在中断上下文）
    ret = usb_submit_urb(dev_priv->bulk_in_urb, GFP_ATOMIC);
    if (ret) {
        printk(KERN_ERR "提交输入URB失败: %d\n", ret);
        goto err_free_out_urb;
    }

    printk(KERN_INFO "USB驱动probe函数执行完成\n");
    return 0;

// 错误处理（面试加分项：资源释放的完整性）
err_free_out_urb:
    usb_free_urb(dev_priv->bulk_out_urb);
err_free_in_urb:
    usb_free_urb(dev_priv->bulk_in_urb);
err_free_buf:
    kfree(dev_priv->bulk_in_buf);
    kfree(dev_priv->bulk_out_buf);
err_free_priv:
    usb_put_dev(dev_priv->udev);  // 引用计数-1
    kfree(dev_priv);
    usb_set_intfdata(intf, NULL);
    return ret;
}

// disconnect函数（配套probe，释放资源）
static void usb_disconnect(struct usb_interface *intf) {
    struct usb_dev_private *dev_priv = usb_get_intfdata(intf);

    if (!dev_priv) return;

    // 1. 取消未完成的URB
    usb_kill_urb(dev_priv->bulk_in_urb);
    usb_kill_urb(dev_priv->bulk_out_urb);

    // 2. 释放URB
    usb_free_urb(dev_priv->bulk_in_urb);
    usb_free_urb(dev_priv->bulk_out_urb);

    // 3. 释放缓冲区
    kfree(dev_priv->bulk_in_buf);
    kfree(dev_priv->bulk_out_buf);

    // 4. 释放设备引用和私有数据
    usb_put_dev(dev_priv->udev);
    kfree(dev_priv);
    usb_set_intfdata(intf, NULL);

    printk(KERN_INFO "USB设备断开，资源已释放\n");
}

// 设备匹配表（面试必问：匹配规则）
static const struct usb_device_id usb_id_table[] = {
    { USB_DEVICE(0x1234, 0x5678) },  // 替换为实际的厂商ID/产品ID
    { }  // 结束符
};
MODULE_DEVICE_TABLE(usb, usb_id_table);

// USB驱动结构体
static struct usb_driver usb_drv = {
    .name = "my_usb_driver",         // 驱动名称，可通过lsmod查看
    .probe = usb_probe,              // 匹配成功回调
    .disconnect = usb_disconnect,    // 设备断开回调
    .id_table = usb_id_table,        // 设备匹配表
};

// 模块入口/出口
module_usb_driver(usb_drv);  // 简化版注册，等价于usb_register/usb_deregister

// 模块信息（规范要求）
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("示例USB驱动（probe函数重点）");
MODULE_AUTHOR("Test");
```

## 三、V4L2

Video for Linux 2：Linux内核中视频采集、处理和输出的标准**驱动框架**和**用户空间API**；

