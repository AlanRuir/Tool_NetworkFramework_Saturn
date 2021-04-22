#include "locker.h"

Locker::Locker()
{
    pthread_mutex_init(&this->lock, nullptr);
    pthread_cond_init(&this->cond, nullptr);
}

Locker::~Locker()
{
    pthread_mutex_destroy(&this->lock);
    pthread_cond_destroy(&this->cond);
}

void Locker::addLock()
{
    pthread_mutex_lock(&this->lock);
}

void Locker::unLock()
{
    pthread_mutex_unlock(&this->lock);
}

void Locker::wait()
{
    pthread_cond_wait(&this->cond, &this->lock);
}

void Locker::wakeOne()
{
    pthread_cond_signal(&this->cond);
}

void Locker::wakeAll()
{
    pthread_cond_broadcast(&this->cond);
}