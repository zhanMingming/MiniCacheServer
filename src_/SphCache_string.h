#ifndef SPHCACHE_SPHCACHE_H
#define SPHCACHE_SPHCACHE_H
#include<vector>
#include<string>
#include"SphHashTable.h"

namespace zhanmm {

class SphCache
{
public:
    SphCache(Session *sess);
    ~SphCache();
    void set(std::string &k, std::string &v);
    std::string get(const std::string &k);

protected:
    struct SphListNode 
    {
        SphListNode(const std::string k, const std::string v)
        :m_key(k),m_value(v)
        {}
        std::string  m_key;
        std::string  m_value;
        SphListNode  *m_prev;
        SphListNode  *m_next;
        bool         m_tag;
        bool         m_isUse;
    };
private:
    void insertIntoList(SphListNode *node);
    void deletefromList(SphListNode *node);

    int                                m_size;
    Session                            *m_sess;
    SphHashTable<std::string, SphListNode *>   m_hashTable;
    std::vector<SphListNode*>          m_allocator;
    SphListNode                        *m_head;
    SphListNode                        *m_tail;
    SphListNode                        *m_entry;
};

} // namespace zhanmm
#endif



