#ifndef SPHCACHE_SPHSHMGET_H
#define SPHCACHE_SPHSHMGET_H
#include<sys/ipc.h>
#include<sys/shm.h>
namespace zhanmm {

typedef unsigned int size_t;
class SphShmget
{
public:
    SphShmget(key_t key, size_t size, const void *ptr = 0);
    ~SphShmget();
    void* attach();
    void  detach();
    void delShmget();
     
private:
    void *m_ptr;
    key_t m_key;
    bool  m_exist;
    int m_id;
};
} // namespace zhanmm

#endif





        
