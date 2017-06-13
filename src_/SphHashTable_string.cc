#include<cstring>
#include<cstdio>
#include<iostream>
#include<cstdlib>
#include"common.h"
#include"SphHashTable_string.h"

namespace zhanmm {

SphHashTable::SphHashTable(Session *sess, int len, int size)
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

SphHashTable::~SphHashTable()
{}

SphHashTable::SphHashNode** SphHashTable::getBucket(const std::string &k)
{
    size_t bucket = m_hashFunc(k) % m_len;
    if (bucket >= m_len) {
        std::cout << "bad bucket lookup" << std::endl;
        exit(-1);
    }    
    return &m_nodes[bucket];
}
       
SphHashTable::SphHashNode *SphHashTable::getNodeByValue(const std::string &k)
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

std::string SphHashTable::lookupEntry(const std::string & k) 
{
    SphHashNode *node = getNodeByValue(k);
    if (node == NULL) {
        return "";
    }
    return node->m_value;
}

void  SphHashTable::addEntry(const std::string &k,const std::string &v)
{
    std::string ret = lookupEntry(k);
    if (!ret.empty()) {
        std::cout << "duplicate key" << std::endl;
        return;
    }

    SphHashNode *node = m_allocator.back();
    m_allocator.back();
   // node->m_isUse = true;
    char *tmp = (char*)node; 
    String *key = new (tmp)std::string;
    tmp += sizeof(std::string);

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

void SphHashTable::freeEntry(const std::string &k) {
    SphHashNode *node = getNodeByValue(k);
    if (node == NULL) {
        std::cout << "no this string" << std::endl;
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

void SphHashTable::erase(const std::string &k) 
{
    freeEntry(k);
}

void SphHashTable::add(const std::string &k, const std::string &v)
{
    addEntry(k, v);
}

std::string SphHashTable::find(const std::string &k)
{
    return lookupEntry(k);
}

} // namespace zhanmm
