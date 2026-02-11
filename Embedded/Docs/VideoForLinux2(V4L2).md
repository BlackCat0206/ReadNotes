# VideoForLinux2(V4L2)

---- Chime Lee

## 一、概述

**核心概念**：

* Linux V4L2（Video for Linux 2）是Linux内核中视频采集、处理和输出的标准**驱动框架**和**用户层通过`ioctl`系统调用与驱动交互**；
* **核心操作对象**：视频设备节点（如`/dev/video0`）;
* **关节数据结构**：`struct v4l2_capability`（设备能力）、`struct v4l2_format`（视频格式）、`struct v4l2_requestbuffers`（缓冲区请求）等；
* **核心流程**：打开设备->查询能力->配置格式->申请缓冲区->映射内存->启动流->采集数据->停止流->释放资源

## 二、示例代码

摄像头实时采集YUV格式数据：

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

// 机器狗摄像头设备节点（根据实际硬件调整）
#define DEVICE_PATH "/dev/video0"
// 视频采集参数（机器狗视觉常用分辨率/帧率）
#define WIDTH 640
#define HEIGHT 480
#define FRAME_RATE 30

// 缓冲区结构体：保存映射后的内存地址和长度
typedef struct {
    void *start;
    size_t length;
} Buffer;

Buffer *buffers;
int buffer_count;
int fd;

// 错误处理函数
static void err_exit(const char *s) {
    perror(s);
    exit(EXIT_FAILURE);
}

// 初始化V4L2设备
static int v4l2_init() {
    struct v4l2_capability cap;
    struct v4l2_format fmt;
    struct v4l2_requestbuffers req;

    // 1. 打开视频设备
    fd = open(DEVICE_PATH, O_RDWR | O_NONBLOCK, 0);
    if (fd < 0)
        err_exit("open device failed");

    // 2. 查询设备能力（确认是视频捕获设备）
    if (ioctl(fd, VIDIOC_QUERYCAP, &cap) < 0) {
        err_exit("VIDIOC_QUERYCAP failed");
    }
    // 检查设备是否支持视频捕获
    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
        fprintf(stderr, "设备不支持视频捕获\n");
        exit(EXIT_FAILURE);
    }
    // 检查设备是否支持MMAP（内存映射，高效采集）
    if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
        fprintf(stderr, "设备不支持流模式\n");
        exit(EXIT_FAILURE);
    }

    // 3. 配置视频格式（YUV420P，机器狗视觉处理常用）
    memset(&fmt, 0, sizeof(fmt));
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = WIDTH;
    fmt.fmt.pix.height = HEIGHT;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV; // YUYV格式，兼容性好
    fmt.fmt.pix.field = V4L2_FIELD_NONE; // 逐行扫描

    if (ioctl(fd, VIDIOC_S_FMT, &fmt) < 0)
        err_exit("VIDIOC_S_FMT failed");

    // 4. 申请缓冲区（使用MMAP方式）
    memset(&req, 0, sizeof(req));
    req.count = 4; // 申请4个缓冲区，平衡性能和内存
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    if (ioctl(fd, VIDIOC_REQBUFS, &req) < 0)
        err_exit("VIDIOC_REQBUFS failed");

    buffer_count = req.count;
    // 分配缓冲区结构体
    buffers = calloc(buffer_count, sizeof(Buffer));
    if (!buffers)
        err_exit("calloc buffers failed");

    // 5. 映射缓冲区到用户空间
    for (int i = 0; i < buffer_count; i++) {
        struct v4l2_buffer buf;
        memset(&buf, 0, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;

        // 查询缓冲区信息
        if (ioctl(fd, VIDIOC_QUERYBUF, &buf) < 0)
            err_exit("VIDIOC_QUERYBUF failed");

        // 内存映射
        buffers[i].length = buf.length;
        buffers[i].start = mmap(NULL, buf.length,
                               PROT_READ | PROT_WRITE,
                               MAP_SHARED,
                               fd, buf.m.offset);
        if (buffers[i].start == MAP_FAILED)
            err_exit("mmap failed");

        // 将缓冲区放入队列
        if (ioctl(fd, VIDIOC_QBUF, &buf) < 0)
            err_exit("VIDIOC_QBUF failed");
    }

    // 6. 启动视频流
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(fd, VIDIOC_STREAMON, &type) < 0)
        err_exit("VIDIOC_STREAMON failed");

    printf("V4L2初始化完成，开始采集视频（%dx%d@%dfps）\n", WIDTH, HEIGHT, FRAME_RATE);
    return 0;
}

// 采集一帧数据（机器狗项目中可循环调用）
static int capture_frame() {
    fd_set fds;
    struct timeval tv;
    int ret;

    FD_ZERO(&fds);
    FD_SET(fd, &fds);

    // 设置超时时间（避免阻塞）
    tv.tv_sec = 2;
    tv.tv_usec = 0;

    // 等待设备就绪
    ret = select(fd + 1, &fds, NULL, NULL, &tv);
    if (ret < 0) {
        if (errno == EINTR)
            return -1;
        err_exit("select failed");
    }
    if (ret == 0) {
        fprintf(stderr, "select timeout\n");
        return -1;
    }

    // 读取缓冲区数据
    struct v4l2_buffer buf;
    memset(&buf, 0, sizeof(buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;

    if (ioctl(fd, VIDIOC_DQBUF, &buf) < 0) { // 取出缓冲区
        if (errno == EAGAIN)
            return -1;
        err_exit("VIDIOC_DQBUF failed");
    }

    // 核心处理：buf.index对应缓冲区，buffers[buf.index].start是数据地址
    // 机器狗项目中，此处可将数据传给视觉算法（如避障、目标检测）
    printf("采集到第%d帧，数据长度：%d字节\n", buf.index, buf.bytesused);

    // 处理完后将缓冲区放回队列
    if (ioctl(fd, VIDIOC_QBUF, &buf) < 0)
        err_exit("VIDIOC_QBUF failed");

    return 0;
}

// 释放资源
static void v4l2_cleanup() {
    // 停止视频流
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ioctl(fd, VIDIOC_STREAMOFF, &type);

    // 解除内存映射
    for (int i = 0; i < buffer_count; i++)
        munmap(buffers[i].start, buffers[i].length);

    free(buffers);
    close(fd);
    printf("V4L2资源已释放\n");
}

// 主函数：机器狗视频采集示例
int main() {
    if (v4l2_init() != 0) {
        fprintf(stderr, "V4L2初始化失败\n");
        return -1;
    }

    // 模拟机器狗持续采集（实际项目中可结合线程/定时器）
    for (int i = 0; i < 100; i++) { // 采集100帧后退出
        capture_frame();
        usleep(1000000 / FRAME_RATE); // 按帧率控制采集间隔
    }

    v4l2_cleanup();
    return 0;
}
```

代码编译与测试：

```bash 
# 编译命令（链接V4L2库）
gcc v4l2_capture.c -o v4l2_capture -lv4l2
# 运行（需要摄像头权限，普通用户需加入video组）
sudo ./v4l2_capture
```

## 三、后记

### 3.1 IOCTL的功能是什么？

`ioctl`系统调用，作用从内核中获取数据或向内核中配置一些数据。

### 3.2 `mmap`的功能是什么，为什么使用`mmap`？

`mmap`系统调用，将文件或设备的内容直接映射到进程的虚拟内存中，无数据拷贝，效率更高；

### 3.3 应用层开发`V4L2`的摄像头驱动一般流程是什么？

打开设备->查询能力->格式配置->申请缓冲区->内存映射->启动流->采集数据->停止流->关闭设备。

### 3.4 `MJPEG`和`YUYV`格式，指的是什么？

`YUYV`：是未压缩的YUV格式，实时性高，CPU占用低，但数据流大；

`MJPEG`：是压缩格式，每一帧都是JPEG，数据小、省带宽、但需要解码、占用CPU；