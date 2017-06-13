#include"SphThreadPool.h"
#include"common.h"
int main(void)
{
    Session *sess = new Session;
    sess->threadNums = 5;
    sess->shared = 0;
    sess->size = 100000;
    zhanmm::SphThreadPool threadpool(sess);
    threadpool.start();
    while(1) {
        threadpool.add(1);
    }
}

