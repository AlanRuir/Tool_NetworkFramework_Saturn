#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <iostream>
#include <vector>
#include <map>
#include <fcntl.h>

#include "epoller.h"
#include "handler.h"

/**
 * 作用：事件循环，通过epoll监听epoll树上的事件，根据不同的事件响应不同的函数
*/
class EventLoop : public Epoller
{
public:
    EventLoop();
    ~EventLoop();
    /**
     * 作用：不停调用Epoller的runLoop()，不断让epoll去监听事件的发生
    */
    void runLoop();
    /**
     * 作用：将参数中的文件描述符设置为非阻塞
     * 参数：要操作的文件描述符
    */
    int setNonBlock(int fd);
    /**
     * 作用：注册服务事件
     * 参数：要注册的文件描述符，文件描述符对应的监听对象
     * 返回值：注册状态
    */
    bool registerServerEvent(int fd, Handler *handler);
    /**
     * 作用：注册通信事件
     * 参数：要注册的文件描述符，文件描述符对应的监听对象
     * 返回值：注册状态
    */
    bool registerSocketEvent(int fd, Handler *handler);
    /**
     * 作用：判断参数中的文件描述符是否为一个监听文件描述符
     * 返回值：判断结果
    */
    bool isListenFd(int fd);
    /**
     * 作用：当epoll返回监听结果后挨个去处理这些事件
     * 参数：epoll返回的监听到的事件数
    */
    void handlerEvents(int size);
private:
    std::vector<int> listenFdVector;        //监听文件描述符数组
    //监听对象映射（当epoll返回监听结果数组后，EventLoop可通过结果数组中的fd找到对应的监听对象，进而调用监听对象中的函数来处理事件）
    std::map<int, Handler *> handlerMap;        
};

#endif