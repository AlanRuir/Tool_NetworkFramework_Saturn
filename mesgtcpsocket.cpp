#include "mesgtcpsocket.h"

MesgTcpSocket::MesgTcpSocket(TcpSocket *tcpClient)
{
    this->cacheSeek = 0;
    this->tcpClient = tcpClient;
    this->tcpClient->registerCallbackReadMesg(std::bind(&MesgTcpSocket::readMesg, this));
    this->tcpClient->registerCallbackSendMesg(std::bind(&MesgTcpSocket::sendMesg, this));
}

void MesgTcpSocket::readMesg()
{
    if(this->tcpClient->threadPool != nullptr)      //若有线程池则往线程池的任务队列中加任务
    {
        struct Task *task = new struct Task;;
        task->callback = std::bind(&MesgTcpSocket::recvMesg, this, std::placeholders::_1);
        task->arg = nullptr;
        this->tcpClient->threadPool->addTask(task);
    }else       //若无线程池则在主线程中执行
    {
        this->recvMesg(nullptr);
    }
}

void MesgTcpSocket::recvMesg(void *)
{
    int recvLen = read(this->tcpClient->getSocketFd(), this->recvBuffer, BUFSIZ - this->cacheSeek);
    if(recvLen < 0)
    {
        if((EAGAIN == errno) && (EWOULDBLOCK == errno))
        {

        }else
        {
            std::cerr << "客户端断开连接" << std::endl;
            close(this->tcpClient->getSocketFd());
            this->tcpClient->eventLoop->delEvent(this->tcpClient->getSocketFd(), EPOLLIN | EPOLLET);
        }
    }else if(0 == recvLen)
    {
        std::cerr << "客户端断开连接" << std::endl;
        close(this->tcpClient->getSocketFd());
        this->tcpClient->eventLoop->delEvent(this->tcpClient->getSocketFd(), EPOLLIN | EPOLLET);
    }else
    {
        this->cacheSeek += recvLen;
        while((this->cacheSeek) > 8)       //从二级缓冲区里接收接收所有能接收的包
        {
            char mesgLenBuffer[8];
            memset(mesgLenBuffer, 0x00, sizeof(mesgLenBuffer));
            memcpy(mesgLenBuffer, this->recvBuffer, 8);
            int mesgLen = std::stoi(mesgLenBuffer);
            std::cout << "客户端数据大小" << mesgLen << std::endl;
            if(this->cacheSeek >= (8 + mesgLen))
            {
                memcpy(this->recvBuffer, this->recvBuffer + 8, this->cacheSeek - 8);
                this->cacheSeek -= 8;     //更新二级缓存尾标

                char mesgJsonBuffer[mesgLen];
                memcpy(mesgJsonBuffer, this->recvBuffer, mesgLen);      //解决粘包问题
                memcpy(this->recvBuffer, this->recvBuffer + mesgLen, this->cacheSeek - mesgLen);
                this->cacheSeek -= mesgLen;     //更新二级缓存尾标

                Json::Value mesgJson;
                Json::CharReaderBuilder readerFactory;
                std::unique_ptr<Json::CharReader> readerPtr(readerFactory.newCharReader());
                readerPtr->parse(mesgJsonBuffer, &mesgJsonBuffer[mesgLen - 1], &mesgJson, nullptr);
        
                std::cout << mesgJson << std::endl;

                //解析数据。。。

                std::cout << "还剩下" << this->cacheSeek << "个" << std::endl;
            }
            break;
        }
    }
}

void MesgTcpSocket::sendData(std::string mesg)
{
    std::cout << mesg << std::endl;
    this->sendMesgQueue.push(mesg);
    this->tcpClient->eventLoop->mdfEvent(this->tcpClient->getSocketFd(), EPOLLOUT | EPOLLET);
}

void MesgTcpSocket::sendMesg()
{
    if(this->tcpClient->threadPool != nullptr)
    {
        struct Task *task = new struct Task;
        task->callback = std::bind(&MesgTcpSocket::sendMesges, this, std::placeholders::_1);
        task->arg = nullptr;
        this->tcpClient->threadPool->addTask(task);
    }else
    {
        this->sendMesges(nullptr);
    }
}

void MesgTcpSocket::sendMesges(void *)
{
    while(!this->sendMesgQueue.empty())
    {
        std::cout << this->sendMesgQueue.front().c_str() << std::endl;
        write(this->tcpClient->getSocketFd(), this->sendMesgQueue.front().c_str(), strlen(this->sendMesgQueue.front().c_str()));
        this->sendMesgQueue.pop();
    }

    this->tcpClient->eventLoop->mdfEvent(this->tcpClient->getSocketFd(), EPOLLIN | EPOLLET);
}