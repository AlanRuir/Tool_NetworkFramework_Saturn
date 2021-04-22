#ifndef TASK_H
#define TASK_H

#include <iostream>
#include <functional>

/**
 * 作用：用来存放任务回调函数，存放在线程池的任务队列中
*/
struct Task
{
    std::function<void (void *)> callback;      //任务函数
    void *arg;      //函数参数
};

#endif