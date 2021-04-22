#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <iostream>
#include <map>
#include <functional>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "handler.h"
#include "tcpsocket.h"
#include "eventloop.h"
#include "threadpool.h"

/**
 * 作用：负责监听新的客户端连接，并提供监听事件（有新客户端连接）的处理函数给事件循环，在这个处理函数中会创建接收新客户端并将通信事件对象注册给事件循环
*/
class TcpServer : public Handler
{
public:
    /**
     * 作用：创建监听文件描述符并将this注册到事件循环服务监听事件中
     * 参数：程序中唯一的事件循环对象和程序中唯一的线程池对象，若不传线程池则默认全部操作在主线程中执行
    */
    TcpServer(int port, EventLoop *eventLoop, ThreadPool *threadPool = nullptr);
    ~TcpServer();
    /**
     * 作用：开启监听
     * 返回值：开启监听结果
    */
    bool startListen(int listenCount);
    /**
     * 作用：注册新客户端加入的回调函数
    */
    void registerCallbackNewClientConnection(std::function<void (TcpSocket *)> callbackNewClientConnection);
    /**
     * 作用：重写事件接口的接收新连接处理函数，供事件循环在监听到新客户端连接后调用
    */
    void handlerAccept() override;
private:
    int port;       //监听的端口
    EventLoop *eventLoop;       //程序中唯一的事件循环对象
    ThreadPool *threadPool;     //程序中唯一的线程池对象
    std::map<int, TcpSocket *> clientMap;       //通信对象（客户端对象）映射
    //新客户端加入的回调函数，应用层类会调用registerCallbackNewClientConnection来注册的
    std::function<void (TcpSocket *)> callbackNewClientConnection;      
};

#endif