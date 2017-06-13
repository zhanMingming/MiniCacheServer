#include<sys/ipc.h>
#include<sys/shm.h>
#include<iostream>
#include<unistd.h>
#include<cstdlib>
#include"SphShmget.h"
#include"common.h"

namespace zhanmm {

SphShmget::SphShmget(key_t key, size_t size, const void *ptr)
:m_exist(false),
m_key(key),
m_ptr(NULL)
{
    m_id = shmget(static_cast<key_t>(m_key), size, 0666|IPC_CREAT);
    CHECK((m_id == -1), "shmget");
    m_exist = true;
}

void * SphShmget::attach()
{
    void *ret = shmat(m_id, m_ptr, 0);
    CHECK((ret == (void*)-1), "shmat");
    return ret;
}
void  SphShmget::detach()
{
    int ret = shmdt(m_ptr);
    CHECK((ret == -1), "shmdt");
}
void SphShmget::delShmget()
{
    int ret = shmctl(m_id, IPC_RMID, 0);
    CHECK((ret==-1), "shmctl");
    m_exist = false;
}

SphShmget::~SphShmget()
{
    if (m_exist) {
        delShmget();
        m_exist = false;
    }
}

} // namespace zhanmm



