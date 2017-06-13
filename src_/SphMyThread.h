#ifndef SPHCACHE_SPHMYTHREAD_H
#define SPHCACHE_SPHMYTHREAD_H

#include"SphThread.h"
namespace zhanmm {
class SphThreadPool;
class SphThread;

class SphMyThread : public SphThread
{
public:
    SphMyThread(SphThreadPool &threadpool, int type);
    void run();

private:
    SphThreadPool &m_threadpool;
    int m_type;
};

} // namespace zhanmm

#endif

