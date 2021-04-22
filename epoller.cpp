#include "epoller.h"

int Epoller::maxEventCount = 1024;

Epoller::Epoller()
{
    this->epfd = epoll_create(Epoller::maxEventCount);      
    this->events = new struct epoll_event[Epoller::maxEventCount];
}

Epoller::~Epoller()
{
    close(this->epfd);
    delete[] this->events;
}

int Epoller::waitEvent()
{
    return epoll_wait(this->epfd, this->events, Epoller::maxEventCount, -1);
}

bool Epoller::addEvent(int fd, int status)
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = status;
    return !epoll_ctl(this->epfd, EPOLL_CTL_ADD, fd, &event);
}

bool Epoller::delEvent(int fd, int status)
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = status;
    return !epoll_ctl(this->epfd, EPOLL_CTL_DEL, fd, &event);
}

bool Epoller::mdfEvent(int fd, int status)
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = status;
    return !epoll_ctl(this->epfd, EPOLL_CTL_MOD, fd, &event);
}