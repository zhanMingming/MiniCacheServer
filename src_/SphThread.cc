#include"SphThread.h"
namespace zhanmm {

SphThread::SphThread()
:m_pthId(0),
m_isRunning(false)
{}

void SphThread::start()
{
    pthread_create(&m_pthId, NULL, runInThread,this);
    m_isRunning = true;
}

void SphThread::join()
{
    pthread_detach(m_pthId);
    m_isRunning = false;
}

void * SphThread::runInThread(void *arg)
{
    SphThread *pThread =static_cast<SphThread*>(arg);
    pThread->run();
    return NULL;
}

SphThread::~SphThread()
{
    if (m_isRunning) {
        pthread_detach(m_pthId);
        m_isRunning = false;
    }
}

} // namespace zhanmm

