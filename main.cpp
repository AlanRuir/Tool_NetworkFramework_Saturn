#include "mesgtcpserver.h"
#include "eventloop.h"
#include "threadpool.h"

int main(int argc, char *argv[])
{
    EventLoop *eventLoop = new EventLoop();
    ThreadPool *threadPool = new ThreadPool(5);
    MesgTcpServer *mesgTcpServer = new MesgTcpServer(8081, eventLoop, threadPool);

    mesgTcpServer->startListen(128);
    eventLoop->runLoop();

    return 0;
}