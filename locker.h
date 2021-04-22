#ifndef LOCKER_H
#define LOCKER_H

#include <pthread.h>

/**
 * 作用：负责线程池内各个线程之间的同步（通过给临界资源（任务队列）加锁解锁，以及通过条件变量保证先生产再消费）
*/
class Locker
{
public:
    /**
     * 作用：初始化互斥锁和条件变量
    */
    Locker();
    /**
     * 作用：销毁互斥锁和条件变量
    */
    ~Locker();
    /**
     * 作用：加锁
    */
    void addLock();
    /**
     * 作用：解锁
    */
    void unLock();
    /**
     * 作用：等待条件变量
    */
    void wait();
    /**
     * 作用：唤醒一个线程
    */
    void wakeOne();
    /**
     * 作用：唤醒全部线程
    */
    void wakeAll();
private:
    pthread_mutex_t lock;
    pthread_cond_t cond;
};

#endif