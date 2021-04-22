#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <iostream>
#include <functional>

#include "handler.h"
#include "eventloop.h"
#include "threadpool.h"

/**
 * 作用：负责与客户端进行通信
*/
class TcpSocket : public Handler
{
public:
    TcpSocket(int fd, EventLoop *eventLoop, ThreadPool *threadPool = nullptr);
    ~TcpSocket();
    /**
     * 作用：获取文件描述符
    */
    const int getSocketFd();
    /**
     * 作用：由应用层类来调用以注册接收信息回调函数
    */
    void registerCallbackReadMesg(std::function<void (void)> callbackReadMesg);
    /**
     * 作用：由应用层类来调用以注册发送信息回调函数
    */
    void registerCallbackSendMesg(std::function<void (void)> callbackSendMesg);
    /**
     * 作用：当事件循环监听到读事件时触发本函数，调用应用层注册的接收信息回调函数来实现数据的接收
    */
    void handlerRead() override;
    /**
     * 作用：当事件循环监听到写事件时触发本函数，调用应用层注册的发送信息回调函数来实现数据的发送
    */
    void handlerWrite() override;
    EventLoop *eventLoop;
    ThreadPool *threadPool;
private:
    //客户端发来消息用来接收的回调函数，应用层类会调用registerCallbackReadMesg来注册
    std::function<void (void)> callbackReadMesg;    
    //给客户端发送消息的回调函数，应用层类会调用registerCallbackSendMesg来注册
    std::function<void (void)> callbackSendMesg;
};

#endif