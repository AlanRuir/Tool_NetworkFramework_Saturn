#include "thread.h"

Thread::Thread(std::function<void (void)> callback) : isStart(false), isJoin(false), pid(0)
{
    this->callback = std::move(callback);
}

Thread::~Thread()
{

}

void Thread::start()
{
    this->isStart = true;
    pthread_create(&this->pid, nullptr, Thread::runInThread, this);     //创建线程并执行Thread::runInThread
}

void Thread::join()
{
    if(this->isStart)
    {
        this->isJoin = true;
        pthread_join(this->pid, nullptr);
    }
}

pthread_t Thread::getPid()
{
    return this->pid;
}

void * Thread::runInThread(void *arg)
{
    Thread *thread = static_cast<Thread *>(arg);
    if(thread != nullptr)
    {
        thread->callback();     //拿到线程池要求执行的回调函数并执行
    }

    return nullptr;
}