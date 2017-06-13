#include<sys/epoll.h>
#include"SphEpoller.h"
int main(void)
{
    zhanmm::SphEpoller epoll(true);
    struct epoll_event ev;
    ev.data.fd = 1;
    ev.events = EPOLLIN | EPOLLET;
    epoll.add(1,ev);
    epoll.mod(1,ev);
}

