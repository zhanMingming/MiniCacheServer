#include"SphMutexLock.h"
#include"common.h"

namespace zhanmm {
SphMutexLock::SphMutexLock()
{
    int ret = pthread_mutex_init(&m_mutex, NULL);
    CHECK((ret == -1),"pthread_mutex_init");
}

void SphMutexLock::lock()
{
    int ret = pthread_mutex_lock(&m_mutex);
    CHECK((ret == -1),"pthread_mutex_lock");
}

void SphMutexLock::unlock()
{
    int ret = pthread_mutex_unlock(&m_mutex);
    CHECK((ret == -1), "pthread_mutex_unlock");
}

pthread_mutex_t* SphMutexLock::getMutexPtr()
{
    return &m_mutex;
}
SphMutexLock::~SphMutexLock()
{
    int ret = pthread_mutex_destroy(&m_mutex);
    CHECK((ret == -1),"pthread_mutex_destroy");
    
}
 
} // namespace zhanmm
 


    
