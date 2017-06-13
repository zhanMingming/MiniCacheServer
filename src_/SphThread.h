#ifndef SPACACHE_SPHTHREAD_H
#define SPACACHE_SPHTHREAD_H

#include<pthread.h>
#include"Noncopyable.h"

namespace zhanmm {
class SphThread : private zhanmm::Noncopyable
{
public:
    SphThread();
    ~SphThread();
    
    void start();
    void join();
    virtual void run() = 0;
    static void * runInThread(void *arg);


private:
    pthread_t m_pthId;
    bool     m_isRunning;
};

} // namespace zhanmm
#endif

  
