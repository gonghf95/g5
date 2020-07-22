#include "threadpool.h"

#include <errno.h>

void *exec_task(void *arg)
{
    threadpool *pool = (threadpool *)arg;
    while (true)
    {
        pool->mutex_lock();
        pool->m_idle++;                            // 空闲线程 +1
        bool timeout = false;                      // 空闲等待超时标志
        while (!pool->m_stop && pool->m_idle <= 0) // 没有空闲线程就等待
        {
            // pool->cond_wait();
            int ret = pool->cond_timedwait(pool->m_timeout);
            if (ret == ETIMEDOUT)
            {
                timeout = true;
                break;
            }
        }

        pool->m_idle--; // 消耗线程 -1

        if (!pool->m_taskq.empty() && pool->m_stop) // 线程池销毁结束
        {
            pool->m_thread_count--; // 线程数量减少
            if (pool->m_thread_count <= 0)
                pool->cond_signal(); // 发送线程退出通知
            pool->mutex_unlock();    // 解锁
            break;
        }

        if (!pool->m_taskq.empty() && timeout) // 空闲超时
        {
            pool->m_thread_count--; // 线程数量减少
            if (pool->m_thread_count <= 0)
                pool->cond_signal(); // 发送线程退出通知
            pool->mutex_unlock();    // 解锁
            break;
        }

        if (!pool->m_taskq.empty())
        {
            task *task = pool->m_taskq.front();
            pool->m_taskq.pop();
            pool->mutex_unlock(); // 先解锁，再执行任务
            if (task != NULL)
                task->exec(); // 执行任务
        }
        else
            pool->mutex_unlock(); // 解锁
    }
    return NULL;
}

threadpool::threadpool(int thread_count, int timeout)
    : m_max_thread_count(thread_count),
      m_thread_count(0),
      m_idle(0),
      m_stop(0),
      m_timeout(timeout)
{
    pthread_mutex_init(&m_mutex, NULL);
    pthread_cond_init(&m_cond, NULL);
}

threadpool::~threadpool()
{
    mutex_lock();

    if (m_stop)
        return;

    m_stop = true;
    if (m_thread_count > 0)
    {
        if (m_idle > 0)
            cond_broadcast(); // 广播退出

        while (m_thread_count > 0) // 等待所有执行工作线程退出
            cond_wait();
    }
    mutex_unlock();

    pthread_cond_destroy(&m_cond);
    pthread_mutex_destroy(&m_mutex);
}

void threadpool::add_task(task *newtask)
{
    mutex_lock();

    m_taskq.push(newtask);

    if (m_idle > 0)
        pthread_cond_signal(&m_cond); // 通知工作线程
    else if (m_thread_count < m_max_thread_count && !m_stop)
    {
        pthread_t tid;
        pthread_create(&tid, NULL, exec_task, (void *)this);
        m_thread_count++;
    }
    mutex_unlock();
}

int threadpool::mutex_lock()
{
    return pthread_mutex_lock(&m_mutex);
}

int threadpool::mutex_unlock()
{
    return pthread_mutex_unlock(&m_mutex);
}

int threadpool::cond_wait()
{
    return pthread_cond_wait(&m_cond, &m_mutex);
}

int threadpool::cond_timedwait(int second)
{
    timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += second;
    return pthread_cond_timedwait(&m_cond, &m_mutex, &ts);
}

int threadpool::cond_signal()
{
    return pthread_cond_signal(&m_cond);
}

int threadpool::cond_broadcast()
{
    return pthread_cond_broadcast(&m_cond);
}