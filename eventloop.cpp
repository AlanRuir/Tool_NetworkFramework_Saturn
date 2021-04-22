#include "eventloop.h"

EventLoop::EventLoop()
{

}

EventLoop::~EventLoop()
{

}

void EventLoop::runLoop()
{
    while(1)
    {
        this->handlerEvents(this->waitEvent());     //epoll树一直监听事件并交给handlerEvents去处理
    }
}

int EventLoop::setNonBlock(int fd)
{
    int oldOption = fcntl(fd, F_GETFL);
    int newOption = fcntl(fd, oldOption | O_NONBLOCK);
    fcntl(fd, F_SETFL, newOption);

    return oldOption;
}

bool EventLoop::registerServerEvent(int fd, Handler *handler)
{
    this->listenFdVector.push_back(fd);     //将文件描述符加入监听文件描述符数组
    this->handlerMap[fd] = handler;         //将对应的监听对象放入监听对象映射中，供handlerEvents去查找并执行对应的处理函数
    if(this->addEvent(fd, EPOLLIN | EPOLLET))       //将监听文件描述符上树，设置为边沿触发
    {
        this->setNonBlock(fd);      //将文件描述符设置为非阻塞
        return true;
    }

    std::cout << "注册服务socket：" << fd << "失败" << std::endl;
    return false;
}

bool EventLoop::registerSocketEvent(int fd, Handler *handler)
{
    this->handlerMap[fd] = handler;     //将对应的监听对象放入监听对象映射中，供handlerEvents去查找并执行对应的处理函数
    if(this->addEvent(fd, EPOLLIN | EPOLLET))       //将通信文件描述符上树，设置为边沿触发
    {
        this->setNonBlock(fd);      //将文件描述符设置为非阻塞
        return true;
    }

    std::cout << "注册客户socket：" << fd << "失败" << std::endl;
    return false;
}

bool EventLoop::isListenFd(int fd)
{
    for(int i = 0; i < this->listenFdVector.size(); ++i)        //在监听文件描述符数组中找这个文件描述符
    {
        if(fd == this->listenFdVector.at(i))        
        {
            return true;
        }
    }

    return false;
}

//runLoop中不断监听新的事件并提交给handlerEvents来做处理
void EventLoop::handlerEvents(int size)
{
    int socketfd = 0;

    for(int i = 0; i < size; ++i)       //遍历监听结果数组，处理每一个事件
    {
        socketfd = this->events[i].data.fd;

        if((this->isListenFd(socketfd)) && (EPOLLIN & this->events[i].events))      //若为监听文件描述符且为输入事件
        {
            std::cout << "触发监听事件" << std::endl;
            this->handlerMap[socketfd]->handlerAccept();        //在监听对象映射中找到对应的监听对象并执行其handlerAccept(); 
        }else if(EPOLLIN & this->events[i].events)      //若为通信文件描述符且事件为输入事件
        {
            std::cout << "触发读事件" << std::endl;
            this->handlerMap[socketfd]->handlerRead();      //在监听对象映射中找到对应的监听对象并执行其handlerRead(); 
        }else if(EPOLLOUT & this->events[i].events)     //若为通信文件描述符且事件为输出事件
        {
            std::cout << "触发写事件" << std::endl;
            this->handlerMap[socketfd]->handlerWrite();     //在监听对象映射中找到对应的监听对象并执行其handlerWrite(); 
        }
    }
}