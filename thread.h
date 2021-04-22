#ifndef THREAD_H
#define THREAD_H

#include <iostream>
#include <functional>
#include <unistd.h>
#include <pthread.h>

/**
 * 作用：负责执行线程池中任务队列中的任务
*/
class Thread
{
public:
    /**
     * 作用：初始化线程类的状态
     * 参数：线程池要求执行的回调函数
    */
    Thread(std::function<void (void)> callback);
    ~Thread();
    /**
     * 作用：创建线程，执行函数是static void * runInThread(void *arg);
    */
    void start();
    /**
     * 作用：等待线程退出
    */
    void join();
    /**
     * 作用：获取线程ID
     * 返回值：线程ID
    */
    pthread_t getPid();
    /**
     * 线程的执行函数，参数是this
    */
    static void * runInThread(void *arg);
private:
    bool isStart;       //是否被创建
    bool isJoin;        //是否进入join状态
    pthread_t pid;      //线程ID
    std::function<void (void)> callback;        //线程池要求执行的回调函数
};

#endif