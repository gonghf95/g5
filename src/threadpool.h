#ifndef THREADPOOL_H_
#define THREADPOLL_H_

#include <pthread.h>
#include <queue>

#include "task.h"

class threadpool
{
public:
    threadpool(int thread_count = 8, int timeout = 1);
    virtual ~threadpool();

    void add_task(task *newtask); // 添加任务

private:
    friend void *exec_task(void *arg); // 工作线程回调函数

    int mutex_lock();               // 互斥量加锁
    int mutex_unlock();             // 互斥量解锁
    int cond_wait();                // 条件等待
    int cond_timedwait(int second); // 条件等待
    int cond_signal();              // 发送条件信号
    int cond_broadcast();           // 广播信号

    int m_max_thread_count; // 最大工作线程个数
    int m_thread_count;     // 工作线程个数
    int m_idle;             // 当前工作线程空闲个数
    int m_stop;             // 是否退出标志 1
    int m_timeout;          // 工作线程空闲超时退出时间

    std::queue<task *> m_taskq; // 任务队列

    pthread_mutex_t m_mutex; // 互斥量
    pthread_cond_t m_cond;   // 条件变量
};

#endif