#ifndef SPACACHE_SPHEPOLLER_H
#define SPACACHE_SPHEPOLLER_H
#include<sys/epoll.h>

namespace zhanmm {

class SphEpoller
{
public:
    SphEpoller(int maxConnects = 1024);
    ~SphEpoller();
    void add(int fd, struct epoll_event ev);
    void mod(int fd, struct epoll_event ev);
    void del(int fd, struct epoll_event ev);    
    int wait(int millSeconds = -1);
    struct epoll_event & get(int index);

protected:
    void ctrl(int fd, struct epoll_event ev, int op);

private:
    int                 m_epollFd;
    int                 m_maxConnects;
    struct epoll_event  *m_evs;

};

} // namespace zhanmm
#endif

