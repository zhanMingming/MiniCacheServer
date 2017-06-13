#include"SphEpoller.h"
#include"common.h"

namespace zhanmm {

SphEpoller::SphEpoller(int maxConnects)
:m_maxConnects(maxConnects),
m_epollFd(-1),
m_evs(0)
{
    m_epollFd = epoll_create(m_maxConnects + 1);
    m_evs = new epoll_event[m_maxConnects + 1];
}

SphEpoller::~SphEpoller()
{
    if (m_evs != NULL) {
        delete []m_evs;
        m_evs = NULL;
    }
    if (m_epollFd > 0) {
        close(m_epollFd);
        m_epollFd = -1;
    }    
}

void SphEpoller::add(int fd, struct epoll_event ev)
{
    ctrl(fd, ev, EPOLL_CTL_ADD);
}

void SphEpoller::mod(int fd, struct epoll_event ev)
{
    ctrl(fd, ev, EPOLL_CTL_MOD);
}

void SphEpoller::del(int fd, struct epoll_event ev)
{
    ctrl(fd, ev, EPOLL_CTL_DEL);
}

void SphEpoller::ctrl(int fd, struct epoll_event ev, int op)
{
    epoll_ctl(m_epollFd, op, fd, &ev);
}

int SphEpoller::wait(int millSeconds)
{
    return epoll_wait(m_epollFd, m_evs, m_maxConnects+1, millSeconds);
}

struct epoll_event & SphEpoller::get(int index)
{
    if (index >= 0 && index <= m_maxConnects) {
        return m_evs[index];
    } else {
        ERR_EXIT("Epoller::get(index)");
    }
}


} // namespace zhanmm



