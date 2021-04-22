#include "mesgtcpserver.h"

MesgTcpServer::MesgTcpServer(int port, EventLoop *eventLoop, ThreadPool *threadPool)
{
    this->tcpServer = new TcpServer(port, eventLoop, threadPool);
    this->tcpServer->registerCallbackNewClientConnection(std::bind(&MesgTcpServer::newClientConnnection, this, std::placeholders::_1));
}

MesgTcpServer::~MesgTcpServer()
{
    delete this->tcpServer;
    for(std::list<MesgTcpSocket *>::iterator iter = this->mesgTcpSocketList.begin(); iter != this->mesgTcpSocketList.end(); ++iter)
    {
        delete (*iter);
        this->mesgTcpSocketList.erase(iter);
    }
}

void MesgTcpServer::newClientConnnection(TcpSocket *client)
{
    MesgTcpSocket *mesgTcpClient = new MesgTcpSocket(client);
    this->mesgTcpSocketList.push_back(mesgTcpClient);
}

bool MesgTcpServer::startListen(int listenCount)
{
    return this->tcpServer->startListen(listenCount);
}