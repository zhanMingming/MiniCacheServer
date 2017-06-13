#include"SphCondition.h"

namespace zhanmm {

SphCondition::SphCondition(SphMutexLock &mutex)
:m_mutex(mutex)
{
    pthread_cond_init(&m_cond, NULL);
}

SphCondition::~SphCondition()
{
    pthread_cond_destroy(&m_cond);
}

void SphCondition::wait()
{
    pthread_cond_wait(&m_cond, m_mutex.getMutexPtr());
}

void SphCondition::notify()
{
    pthread_cond_signal(&m_cond);
}

void SphCondition::notifyall()
{
    pthread_cond_broadcast(&m_cond);
}

} // namespace zhanmm

