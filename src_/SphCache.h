#ifndef SPHCACHE_SPHCACHE_H
#define SPHCACHE_SPHCACHE_H
#include<vector>
#include"SphHashTable.h"
#include"common.h"

namespace zhanmm {

typedef unsigned int size_t;
template<class Key, class Value>
class SphCache
{
public:
    SphCache(Session *sess);
    ~SphCache();
    void set(const Key &k, const Value &v);
    Value get(const Key &k);

protected:
    struct SphListNode 
    {   
        Key          m_key;
        Value        m_value;
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
    SphHashTable<Key, SphListNode *>   m_hashTable;
    std::vector<SphListNode*>          m_allocator;
    SphListNode                        *m_head;
    SphListNode                        *m_tail;
    SphListNode                        *m_entry;
};

} // namespace zhanmm
#include"SphCache.cc"
#endif



