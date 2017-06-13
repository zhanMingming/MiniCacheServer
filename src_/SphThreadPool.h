#ifndef SPHCACHE_SPHTHREADPOOL_H
#define SPHCACHE_SPHTHREADPOOL_H
#include<climits>
#include<vector>
#include"common.h"
#include"SphBuffer.h"
#include"SphCache.h"
#include"SphEpoller.h"

class Session;
namespace zhanmm {

class SphMyThread;
class SphThread;
class SphListNode;

struct ThreadType
{
    enum {
        WorkerThread = 1,
        SyncThread
    };
};

class SphThreadPool
{
public:
    SphThreadPool(Session *sess, SphEpoller &epoll);
    ~SphThreadPool();
    
    void start();
    void stop();
    void doWorker();
    void doSync();
    void add(int fd);
private:
    Session*                            m_sess;
    SphBuffer<int>                      m_queFd;
    SphBuffer<SphListNode *>            m_listNode;
    std::vector<SphThread *>            m_vecThreads;
    SphCache<std::string, std::string>  cache;
    SphEpoller                          &m_epoll;
    bool                                m_isExit;
};

} // namespace zhanmm
    
#endif

