#ifndef SPHCACHE_SPHHASHTABLE_H
#define SPHCACHE_SPHHASHTABLE_H
#include<cstring>
#include<string>
#include<cstdio>
#include<iostream>
#include<cstdlib>
#include"hash.h"
#include"common.h"

namespace zhanmm {

typedef unsigned int size_t;
template<class Key, class Value>
class SphHashTable 
{
public:
    SphHashTable(Session *sess, int len, int size);
    ~SphHashTable();
    void  erase(const Key &k);
    void  add(const Key &k, const  Value &v);
    Value find(const Key &k); 

public:
    struct SphHashNode
    {
        SphHashNode(const Key &k, const Value & v)
        :m_key(k),m_value(v),m_prev(NULL),m_next(NULL),m_isUse(false)
        {}
        Key          m_key;
        Value        m_value;
        SphHashNode  *m_prev;
        SphHashNode  *m_next;
        bool         m_isUse;
    }; 

private:
    SphHashNode **getBucket(const Key&  k);
    SphHashNode *getNodeByKey(const Key &k);
    Value lookupEntry(const Key &k);
    void  addEntry(const Key &k, const Value & v);
    void  freeEntry(const Key &k);

    Session                  *m_sess; 
    std::vector<SphHashNode *> m_allocator;
    Hash<Key>                  m_hashFunc; 
    int                        m_len; //length of hashtable
    int                        m_size;
    SphHashNode**              m_nodes;
   //CompareKey<Key> m_compare; //compare the Key
};

template<class Key, class Value>
SphHashTable<Key, Value>::SphHashTable(Session *sess, int len, int size)
:m_sess(sess),m_len(len),m_size(size)
{
    m_nodes = (SphHashNode **)((char*)m_sess->shared + (m_len + 2)*m_size);
//    memset(m_nodes, 0, m_len * sizeof(SphHashNode*));
    SphHashNode * tmp = (SphHashNode*)((char*)m_nodes + m_len * sizeof(SphHashNode*));
    for (int index = 0; index != m_len; ++index) {
        SphHashNode *t = tmp + index;
        if (t->m_isUse == false) {
            m_allocator.push_back(tmp +index);
        }
    } 
}

template<class Key, class Value>
SphHashTable<Key, Value>::~SphHashTable()
{}

template<class Key, class Value>
typename SphHashTable<Key, Value>::SphHashNode** SphHashTable<Key, Value>::getBucket(const Key &k)
{
    size_t bucket = m_hashFunc(k) % m_len;
    if (bucket >= m_len) {
        std::cout << "bad bucket lookup" << std::endl;
        exit(-1);
    }    
    return &m_nodes[bucket];
}
       

template<class Key, class Value>
typename SphHashTable<Key, Value>::SphHashNode *SphHashTable<Key, Value>::getNodeByKey(const Key &k)
{
    SphHashNode **bucket = getBucket(k);
    SphHashNode *node = *bucket;
    if (node == NULL) {
        return NULL;
    }
    while(node != NULL && node->m_key != k) {
        node = node->m_next;
    }
    return node;
}

template<class Key, class Value>
Value SphHashTable<Key, Value>::lookupEntry(const Key & k) 
{
    SphHashNode *node = getNodeByKey(k);
    if (node == NULL) {
        return NULL;
    }
    return node->m_value;
}

template<class Key, class Value>
void  SphHashTable<Key, Value>::addEntry(const Key &k,const Value &v)
{
    if (lookupEntry(k)) {
        std::cout << "duplicate key" << std::endl;
        return;
    }

    SphHashNode *node = m_allocator.back();
    m_allocator.back();
    char *tmp = (char*)node;
    Key *p = new (tmp)Key;
     
    node->m_key   = k;
    node->m_value = v;
/*    SphHashNode* tmp = new SphHashNode(k, v);
    node->m_key = tmp->m_key;
    node->m_value = tmp->m_value;
    delete tmp;
*/
 //   SphHashNode *node = new SphHashNode(k, v);
    SphHashNode **bucket = getBucket(k);
    if (*bucket == NULL) {  // the list is NULL
        *bucket = node;
    } else {
        node->m_next = *bucket;
        (*bucket)->m_prev = node;
        *bucket = node;
    }
}

template<class Key, class Value>
void SphHashTable<Key, Value>::freeEntry(const Key &k) {
    SphHashNode *node = getNodeByKey(k);
    if (node == NULL) {
        std::cout << "no this Key" << std::endl;
        return;
    }
    if (node->m_prev) {
        node->m_prev->m_next = node->m_next;
    } else {
        SphHashNode **bucket = getBucket(k);
        *bucket = node->m_next;
    }
    if (node->m_next) {
        node->m_next->m_prev = node->m_prev;
    }
    node->m_isUse = false;
    m_allocator.push_back(node);
}

template<class Key, class Value>
void SphHashTable<Key, Value>::erase(const Key &k) 
{
    freeEntry(k);
}

template<class Key, class Value>
void SphHashTable<Key, Value>::add(const Key &k, const Value &v)
{
    addEntry(k, v);
}

template<class Key, class Value>
Value SphHashTable<Key, Value>::find(const Key &k)
{
    return lookupEntry(k);
}

} // namespace zhanmm
#endif    
