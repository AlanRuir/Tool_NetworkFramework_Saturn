#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <queue>
#include <list>

#include "locker.h"
#include "thread.h"
#include "task.h"

/**
 * 作用：创建和维护一个线程池用于高并发地执行各种任务
*/
class ThreadPool
{
public:
    /**
     * 作用：初始化线程池状态并创建threadCount个子线程，并要求它们都执行this->taskProcFunc();，将这些子线程放到线程列表中
     * 参数：要创建的线程的数量
    */
    ThreadPool(int threadCount);
    /**
     * 作用：清空线程池
    */
    ~ThreadPool();
    /**
     * 作用：向任务队列中添加任务
     * 参数：要添加的任务（struct Task是一个任务结构体，包含任务函数和参数）
     * 返回值：添加任务的结果：成功返回0，失败返回-1
    */
    int addTask(struct Task *task);
    /**
     * 作用：获取当前任务队列中任务的个数
     * 返回值：当前任务队列中任务的个数
    */
    int getTaskSize();
    /**
     * 作用：关闭线程池中所有线程
     * 返回值：关闭结果：已关闭返回-1，开始执行关闭操作返回0
    */
    int stopAll();
protected:
    /**
     * 作用：回调给线程池中线程去执行的函数（要求线程池中线程执行的函数）
    */
    void taskProcFunc();
private:
    int threadCount;        //创建和维护的子线程的数量（线程池中线程的数量）
    bool isExit;        //是否退出
    std::queue<struct Task *> taskQueue;        //任务队列
    std::list<Thread *> threadList;         //线程列表（线程池）
    Locker locker;      //线程数量
};

#endif