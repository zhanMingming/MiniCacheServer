namespace zhanmm {
#include<cassert>

template<class Key, class Value>
SphCache<Key, Value>::SphCache(Session *sess)
:m_sess(sess),
m_size(100),
m_hashTable(sess, m_size - 2, sizeof(SphListNode))
{ 
    m_entry = (SphListNode*)m_sess->shared;
    m_head = m_entry;
    m_tail = m_entry + 1;
    if (m_head->m_next == NULL) {
        m_head->m_next = m_tail;
    }

    if (m_tail->m_prev == NULL) {
        m_tail->m_prev = m_head;
    } 
    for (int index = 0; index < m_size; ++index) {
        SphListNode *tmp = m_entry + index;
        if (tmp->m_isUse == false) {
            m_allocator.push_back(tmp);
        }
    }
}


template<class Key, class Value>
void SphCache<Key, Value>::insertIntoList(SphListNode * node)
{
    assert(node != NULL);
    node->m_next = m_head->m_next;
    node->m_next->m_prev = node;
    node->m_prev = m_head;
    m_head->m_next = node;
}

template<class Key, class Value>
void SphCache<Key, Value>::deletefromList(SphListNode *node)
{
    assert(node != NULL);
    node->m_prev->m_next = node->m_next;
    node->m_next->m_prev = node->m_prev;
}

template<class Key, class Value>
SphCache<Key, Value>::~SphCache()
{}

template<class Key, class Value>
Value SphCache<Key, Value>::get(const Key &k)
{
    SphListNode *node = m_hashTable.find(k);
    if (node) {
        deletefromList(node);
        insertIntoList(node);
        return node->m_value;
    } else {
        return Value();
    } 
}

template<class Key, class Value>
void  SphCache<Key, Value>::set(const Key &k, const Value &v)
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
        // node = new SphListNode(k, v);
        char *tmp = (char*)node;
        Key *t = new (tmp)Key;
        node->m_key = k;
        tmp += sizeof(Key);
        Value *t2 = new (tmp)Value;
        node->m_value = v;
        
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

