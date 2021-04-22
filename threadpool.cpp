#include "threadpool.h"

ThreadPool::ThreadPool(int threadCount) : isExit(false), threadCount(threadCount)
{
    for(int i = 0; i < threadCount; ++i)
    {
        Thread *thread = new Thread(std::bind(&ThreadPool::taskProcFunc, this));        //创建Thread对象
        thread->start();        //创建线程     
        threadList.push_back(thread);       //将线程对象放入线程列表
    }

    std::cout << "创建了" << this->threadCount << "个子线程" << std::endl;
}

ThreadPool::~ThreadPool()
{
    for(std::list<Thread *>::iterator iter = this->threadList.begin(); iter != this->threadList.end(); ++iter)
    {
        delete *iter;       //释放线程对象资源
        this->threadList.erase(iter);
    }
}

int ThreadPool::addTask(struct Task *task)
{
    if(task == nullptr)
    {
        return -1;
    }

    this->locker.addLock();         //对任务队列（临界资源）加锁
    this->taskQueue.push(task);     //将任务加入任务队列
    this->locker.unLock();          //对任务队列（临界资源）解锁
    this->locker.wakeOne();         //若线程列表中有线程因为任务队列为空而挂起等待的，将被唤醒一个（通过条件变量）

    return 0;
}

int ThreadPool::getTaskSize()
{
    return this->taskQueue.size();
}

int ThreadPool::stopAll()
{
    if(this->isExit)
    {
        return -1;      //若已经停止则返回-1
    }

    std::cout << "现在将停止所有子线程" << std::endl;
    this->isExit = true;
    this->locker.wakeAll();         //唤醒所有线程，使得现在正因为任务队列中没有任务而挂起的线程也执行分离操作

    for(std::list<Thread *>::iterator iter = this->threadList.begin(); iter != this->threadList.end(); ++iter)
    {
        pthread_detach((*iter)->getPid());      //将线程列表中的所有线程均设置为分离状态
    }

    return 0;
}

void ThreadPool::taskProcFunc()
{
    pthread_t pid = pthread_self();     //获取当前线程ID
    while(!this->isExit)        //若线程池没有销毁则一直从任务队列中取任务并执行，若任务队列为空则挂起等待任务
    {
        this->locker.addLock();     //对任务队列（临界资源）加锁
        
        while(this->taskQueue.empty())      
        {
            this->locker.wait();        //若任务队列为空则解锁并阻塞等待（等待条件变量通知自己任务队列中有任务了）
        }

        //当条件变量通知自己后解除阻塞，并尝试加锁

        Task *task = nullptr;
        //当加锁成功后再尝试去任务队列里去取任务，可这时可能别的线程更快一步，刚刚生产的任务又被别的线程取走了，
        //这时任务队列又空了，再去任务队列中去取就会报错，因此这里需要再判断一下任务队列是否为空
        if(!this->taskQueue.empty())        
        {
            std::cout << "任务队列中现在有" << this->taskQueue.size() << "个任务" << std::endl;
            task = this->taskQueue.front();
            this->taskQueue.pop();
            std::cout << "任务队列中现在还有" << this->taskQueue.size() << "个任务" << std::endl;
        }

        this->locker.unLock();      //对任务队列（临界资源）解锁

        std::cout << "子线程：" << pid << "正在运行" << std::endl;

        if(task != nullptr)     //若成功取到任务则执行任务
        {
            task->callback(task->arg);
            delete task;        //执行完成后释放任务指针
        }

        std::cout << "子线程：" << pid << "执行结束" << std::endl;
    }

    std::cout << "子线程：" << pid << "执行销毁" << std::endl;
    pthread_exit(nullptr);      //若线程池退出（这时线程池可能已经把当前子线程设置为分离状态了）则退出当前线程
}