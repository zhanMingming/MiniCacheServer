#ifndef SPHCACHE_SPHCONDITION_H
#define SPHCACHE_SPHCONDITION_H
#include<pthread.h>
#include"Noncopyable.h"
#include"SphMutexLock.h"

namespace zhanmm {

class SphCondition : public zhanmm::Noncopyable
{
public:
    SphCondition(SphMutexLock &mutex);
    ~SphCondition();

    void notify();
    void wait();
    void notifyall();
private:
    pthread_cond_t m_cond;
    SphMutexLock &m_mutex;
};

} // namespace zhanmm
#endif

