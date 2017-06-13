#ifndef SPACACHE_SPHMUTEXLOCK_H
#define SPACACHE_SPHMUTEXLOCK_H

#include<pthread.h>
#include"Noncopyable.h"
namespace zhanmm {

class SphMutexLock : private zhanmm::Noncopyable
{
public:
    SphMutexLock();
    ~SphMutexLock();

    void lock();
    void unlock();
    pthread_mutex_t *getMutexPtr();
private:
    pthread_mutex_t m_mutex;
};

} // namespace zhanmm

#endif
