#ifndef MESGTCPSOCKET_H
#define MESGTCPSOCKET_H

#include <stdio.h>
#include <iostream>
#include <functional>
#include <string.h>
#include <jsoncpp/json/json.h>
#include <errno.h>
#include <queue>
#include <memory>
#include <sstream>

#include "tcpsocket.h"
#include "threadpool.h"
#include "task.h"

/**
 * 应用层通信类示例：由用户编写：负责将接收数据处理函数（负责解决粘包和半包问题，解析数据包等）注册给通信事件类
 * 将发送数据处理函数注册给通信事件类
*/
class MesgTcpSocket
{
public:
    MesgTcpSocket(TcpSocket *tcpClient);
    void readMesg();
    void sendMesg();
    void recvMesg(void *);
    void sendData(std::string mesg);
    void sendMesges(void *);
private:
    TcpSocket *tcpClient;
    char recvBuffer[BUFSIZ];
    int cacheSeek;
    std::queue<int> mesgLenQueue;
    std::queue<std::string> sendMesgQueue;
};

#endif