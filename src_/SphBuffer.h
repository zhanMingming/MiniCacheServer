#ifndef SPHCACHE_SPHBUFFER_H
#define SPHCACHE_SPHBUFFER_H

#include"SphMutexLock.h"
#include"SphCondition.h"
#include<queue>

namespace zhanmm {

template<class T>
class SphBuffer
{
public:
    SphBuffer(size_t size);
    ~SphBuffer(){}
    void push(const T & task);
    T pop();

    bool empty();
    bool full();
private:
    SphMutexLock   m_mutex;
    SphCondition   m_notFull;
    SphCondition   m_notEmpty;
    size_t            m_size;
    std::queue<T>  m_que;
};

} // namespace zhanmm
#include"SphBuffer.cc"
#endif

