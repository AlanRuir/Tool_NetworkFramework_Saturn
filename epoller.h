#ifndef EPOLLER_H
#define EPOLLER_H

#include <sys/epoll.h>
#include <unistd.h>
#include <iostream>

/**
 * 作用：用来实现Epoll的各种操作（创建epoll树，上树，开启Epoll监听，下树，改变监听事件）
*/
class Epoller
{
public:
    /**
     * 作用：创建epoll树，初始化epoll树根节点，初始化监听结果存储数组
    */
    Epoller();
    /**
     * 作用：释放结果数组，释放epoll树根节点
    */
    ~Epoller();
    /**
     * 作用：开启epoll监听，并返回监听到的事件数
     * 返回值：监听到的事件数
    */
    int waitEvent();
    /**
     * 作用：上树
     * 参数：上树的文件描述符，委托epoll监听的事件类型
     * 返回值：上树结果
    */
    bool addEvent(int fd, int status);
    /**
     * 作用：下树
     * 参数：下树的文件描述符，委托epoll监听的事件类型
     * 返回值：下树结果
    */
    bool delEvent(int fd, int status);
    /**
     * 作用：改变监听类型
     * 参数：需要改变监听类型的文件描述符，新的委托epoll监听的事件类型
     * 返回值：改变监听类型结果
    */
    bool mdfEvent(int fd, int status);
protected:
    static int maxEventCount;       //监听结果存储数组的最大大小
    int epfd;       //epoll树的根节点
    struct epoll_event *events;     //监听结果存储数组
};

#endif