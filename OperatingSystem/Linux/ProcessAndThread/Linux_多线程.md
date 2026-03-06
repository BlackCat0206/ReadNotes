# Linux_多线程

---- Chime Lee

## 一、概述

Linux多线程测试题中内容

## 二、内容

### 2.1 `pthread_equal()`

```c
#include <pthread.h>
int pthread_equal(pthread_t t1, pthread_t t2);
```

* 功能：比较两个线程ID（`pthread_create()`不保证线程ID的唯一性，如果保证了，`pthread_equal()`就没必要存在了）。
* 结果：相同返回非0的数，不同返回0

### 2.2 `pthread_once`

```c
#include <pthread.h>
int pthread_once(pthread_once_t *once_control, void (*init_routine)(void));
```

* 功能：初始化函数在多线程环境中**只执行一次**，无论有多少线程同时调用它，都能确保线程安全且高效地完成一次性初始化。第一个线程调用时，它会获得互斥锁执行初始化函数，然后设置一个标志，表示初始已完成。其他线程调用pthread_once时，它们会尝试获取互斥锁，但由于初始化已经完成，它们会进行阻塞状态等待互斥锁释放。
  * *once_control* 必须是静态存储期变量，并初始化为 *PTHREAD_ONCE_INIT*
  * *init_routine* 是无参、无返回值的初始化函数。

### 2.3 `pthread_sigqueue`

```c
#include <signal.h>
#include <pthread.h>

int pthread_sigqueue(pthread_t thread, int sig, const union sigval value);
```

* 功能：在同一进程中的指定线程发消息
* 参数：
  * thread：同一进程中的线程ID。
  * sig：要发送的信号
  * value：同时发送的数据
* 结果：成功返回0，失败返回错误码。

### 2.4 `pthread_testcancel`

```c
#include <pthread.h>
void pthread_testcancel(void);
```

* 功能：为线程添加一个取消点，当收到取消命令后，立刻停止线程。

#### 场景 1：无取消点的线程（无法被取消）

```c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* thread_func(void* arg) {
    // 纯计算逻辑，无任何内置取消点函数
    while (1) {
        int a = 1 + 1; // 无取消点，pthread_cancel() 无效
    }
    return NULL;
}

int main() {
    pthread_t tid;
    pthread_create(&tid, NULL, thread_func, NULL);
    sleep(1);
    pthread_cancel(tid); // 发送取消请求，但线程无取消点，不会响应
    pthread_join(tid, NULL); // 主线程会一直阻塞，因为子线程无法被取消
    return 0;
}
```

#### 场景 2：插入 pthread_testcancel () 后（可响应取消）

```c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* thread_func(void* arg) {
    while (1) {
        int a = 1 + 1;
        pthread_testcancel(); // 手动插入取消点
    }
    return NULL;
}

int main() {
    pthread_t tid;
    pthread_create(&tid, NULL, thread_func, NULL);
    sleep(1);
    pthread_cancel(tid); // 发送取消请求
    pthread_join(tid, NULL); // 子线程会在取消点响应，主线程正常退出
    return 0;
}
```

### 2.5 `__thread`线程局部存储类限定符

* 作用：声明线程局部变量。让每个线程都拥有独立的变量实例，每个线程之间独立。

```c
#include <pthread>
#include <stdio.h>
static __thread int meng;
static void* thread_start(void* arg)
{
    meng = 10;
    printf("%d", meng);		// 10
    pthread_exit(NULL);
}

int main(void){
    pthread_t tid;
    int ret;
    meng = 20;
    pthread_create(&tid, NULL, tread_start, NULL);
    
    int ret = pthread_join(tid, NULL)
    if (0 == ret)
    {
        printf("error!");
        exit(-1);
    }
    printf("%d", meng);	// 20
    exit(0);
}
```

### 2.6 `pthread_sigmask()`

* 功能：改变线程的掩码。
* Linux下：刚创建的线程，其信号掩码从**创建该线程的线程**中继承。

### 2.7 `pthread_self()`

```c
#include <pthread.h>
pthread_t pthread_self(void);
```

* 获取调用线程的线程ID。
* 这个函数调用一定会成功。

### 2.8 `pthread_cleanup_pop`

```c
#include <pthread.h>
void pthread_cleanup_push(void (*routine)(void *), void *arg);
void pthread_cleanup_pop(int execute);
Compile and link with -pthread.
```

*pthread_cleanup_push* ：

功能：将清理函数 *cleanup_handler* 压入清理栈中。

*pthread_cleanup_pop* ：

功能：

* 不为0，pop弹出清理函数，执行。
* 为0，pop弹出清理函数，不执行。

### 2.9 `pthread_setcancelstate()`

```c
#include <pthread.h>
int pthread_setcancelstate(int state, int *oldstate);
int pthread_setcanceltype(int type, int *oldtype);
```

功能：设置取消类型和获取旧类型操作

参数：

* type：PTHREAD_CANCLE_DEFERRED：取消请求到来时，线程继续执行，取消请求被挂起，知道线程到达某个取消点为止，这是所有新建线程包括主线程默认的取消类型。
* type：PTHREAD_CANCLE_ASYNCHRONOUS：可能会在任何时间点取消线程。
* oldtype：线程之前的取消类型会保存在参数oldtype锁指向的缓冲区中，若对之前的类型没兴趣，Linux下允许将参数oldtype设置为NULL.
