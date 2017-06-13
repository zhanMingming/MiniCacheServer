namespace zhanmm {

template<class Key, class Value>
SphHashTable<Key, Value>::SphHashTable<Key, Value>(Session *sess, int len, int size)
:m_sess(sess),m_len(len),m_size(size)
{
    m_nodes = (SphHashNode **)((char*)sess->shared + (m_len + 2)*m_size);
    memset(m_nodes, 0, m_len * sizeof(SphHashNode*));
    SphHashNode * tmp = (char*)m_nodes + m_len * sizeof(SphHashNode*);
    for (int index = 0; index != m_len; ++index) {
        m_allocator.push_back(tmp +index);
    } 
}

template<class Key, class Value>
SphHashTable<Key, Value>::~SphHashTable<Key, Value>()
{}

template<class Key, class Value>
SphHashNode** SphHashTable<Key, Value>::getBucket(const Key &k)
{
    size_t bucket = m_hashFunc(k) % m_size;
    if (bucket >= m_size) {
        std::cout << "bad bucket lookup" << std::endl;
        exit(-1);
    }    
    return &m_nodes[bucket];
}
       

template<class Key, class Value>
SphHashNode *SphHashTable<Key, Value>::getNodeByKey(const Key &k)
{
    SpHashNode **bucket = getBucket(k);
    SpHashNode *node = *bucket;
    if (node == NULL) {
        return NULL;
    }
    while(node != NULL && Compare) {
        node = node->next;
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
void  SphHashTable<Key, Value>::addEntry(const Key &k, const Value &v)
{
    if (lookupEntry(k)) {
        std::cout << "duplicate key" << std::endl;
        return;
    }
    
    SphHashNode *node = new SphHashNode(k, v);
    SphHashNode **bucket = getBucket(k);
    if (*bucket == NULL) {  // the list is NULL
        *bucket = node;
    } else {
        node->next = *bucket;
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
    free(node);
    node = NULL;
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
    
