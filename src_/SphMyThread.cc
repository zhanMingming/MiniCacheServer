#include"SphMyThread.h"
#include"SphThreadPool.h"

namespace zhanmm {

SphMyThread::SphMyThread(SphThreadPool &threadpool, int type)
:m_threadpool(threadpool),
m_type(type)
{}

void SphMyThread::run()
{
    switch(m_type) {
        case ThreadType::WorkerThread:
        {
            m_threadpool.doWorker();
            break;
        }
        case ThreadType::SyncThread:
        {
            m_threadpool.doSync();
            break;
        }
    }
}

} // namespace zhanmm
    
