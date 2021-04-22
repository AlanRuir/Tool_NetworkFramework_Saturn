#ifndef MESGTCPSERVER_H
#define MESGTCPSERVER_H

#include <iostream>
#include <functional>
#include <list>

#include "tcpserver.h"
#include "tcpsocket.h"
#include "mesgtcpsocket.h"

/**
 * 应用层服务类示例：由用户编写：负责维护一个监听事件类对象并注册一个新连接处理函数给监听事件类的回调函数
*/
class MesgTcpServer
{
public:
    /**
     * 作用：创建并维护一个监听事件类对象并将this->newClientConnnection注册给监听事件类
    */
    MesgTcpServer(int port, EventLoop *eventLoop, ThreadPool *threadPool = nullptr);
    ~MesgTcpServer();
    /**
     * 作用：创建一个应用层通信类对象，它会负责将与客户端之间的通信的两个回调函数注册进通信事件类
    */
    void newClientConnnection(TcpSocket *client);
    /**
     * 调用维护的监听事件类对象的开启监听
    */
    bool startListen(int listenCount);
private:
    TcpServer *tcpServer;
    std::list<MesgTcpSocket *> mesgTcpSocketList;
};

#endif