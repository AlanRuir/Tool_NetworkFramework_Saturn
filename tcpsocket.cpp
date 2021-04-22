#include  "tcpsocket.h"

TcpSocket::TcpSocket(int fd, EventLoop *eventLoop, ThreadPool *threadPool)
{
    this->socketfd = fd;
    this->eventLoop = eventLoop;
    this->threadPool = threadPool;
}

TcpSocket::~TcpSocket()
{

}

const int TcpSocket::getSocketFd()
{
    return this->socketfd;
}

void TcpSocket::registerCallbackReadMesg(std::function<void (void)> callbackReadMesg)
{
    this->callbackReadMesg = std::move(callbackReadMesg);
}

void TcpSocket::registerCallbackSendMesg(std::function<void (void)> callbackSendMesg)
{
    this->callbackSendMesg = std::move(callbackSendMesg);
}

void TcpSocket::handlerRead()
{
    this->callbackReadMesg();
}

void TcpSocket::handlerWrite()
{
    this->callbackSendMesg();
}