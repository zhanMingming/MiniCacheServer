#include<cassert>
#include"SphCache_string.h"
#include"common.h"

namespace zhanmm {
/*
SphCache<std::string, std::string>::SphCache(Session *sess)
:m_size(sess->size / (sizeof(SphCache<std::string, std::string>::SphListNode) + sizeof(SphHashTable<std::string, SphListNode*>::SphHashNode) +  LENOFPTR)),
*/

SphCache::SphCache(Session *sess)
:m_sess(sess),
m_size(100),
m_hashTable(sess, m_size - 2, sizeof(SphListNode))
{ 
    m_entry = (SphListNode*)m_sess->shared;
    m_head = m_entry;
    if (m_head->m_next == NULL) {
        m_head->m_next = m_tail;
    }
    m_tail = m_entry + 1;
    if (m_tail->m_prev == NULL) {
        m_tail->m_prev = m_head;
    } 
    for (int index = 2; index < m_size; ++index) {
        SphListNode *tmp = m_entry + index;
        if (tmp->m_isUse == false) {
            m_allocator.push_back(tmp);
        }
    }
/*
    m_head = m_allocator.back();
    m_allocator.pop_back(); 
    m_tail = m_allocator.back();
    m_allocator.pop_back();
*/

 //   m_head->m_prev = NULL;
 //   if (m_head->m_next == NULL) {
 //       m_head->m_next = m_tail;
 //   }
//
//    m_tail->m_next = NULL;
//    if (m_tail->m_prev == NULL) {
//        m_tail->m_prev = m_head;
//    }

}

void SphCache::insertIntoList(SphListNode * node)
{
    assert(node != NULL);
    node->m_next = m_head->m_next;
    node->m_next->m_prev = node;
    node->m_prev = m_head;
    m_head->m_next = node;
}

void SphCache::deletefromList(SphListNode *node)
{
    assert(node != NULL);
    node->m_prev->m_next = node->m_next;
    node->m_next->m_prev = node->m_prev;
}

SphCache::~SphCache()
{
    delete m_head;
    m_head = NULL;
    delete m_tail;
    m_tail = NULL;
    delete []m_entry;
    m_entry = NULL;
}

std::string SphCache::get(const std::string &k)
{
    SphListNode *node = m_hashTable.find(k);
    if (node) {
        deletefromList(node);
        insertIntoList(node);
        return node->m_value;
    } else {
        return std::string();
    } 
}

void  SphCache::set(std::string &k, std::string &v)
{
    SphListNode *node = m_hashTable.find(k);
    if (node == NULL) {
        if (m_allocator.empty()) {
            // LRU algorithm
            node = m_tail->m_prev;
            deletefromList(node);
            m_hashTable.erase(node->m_key);
        } else {
            node = m_allocator.back();
            node->m_isUse = true;
            node->m_tag = true;
            m_allocator.pop_back();
        }
        std::cout << node->m_key << std::endl;
        node->m_key  =  k;
        node->m_value = v;
        /*node = new SphListNode(k, v);
        node->m_key.assign(k.c_str(), strlen(k.c_str()));
        //node->m_value = v.c_str();
        node->m_value.assign(v.c_str(), strlen(v.c_str())); 
        */
        m_hashTable.add(k, node);
        insertIntoList(node);
    } else {
        deletefromList(node);
        node->m_value = v;
        node->m_tag = true;
        insertIntoList(node);
    }
}

} // namespace zhanmm

