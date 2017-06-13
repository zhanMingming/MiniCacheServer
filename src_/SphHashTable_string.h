#ifndef SPHCACHE_SPHHASHTABLE_H
#define SPHCACHE_SPHHASHTABLE_H
#include<string>
#include<vector>
#include"hash.h"
#include"common.h"

namespace zhanmm {

class SphHashTable 
{
public:
    SphHashTable(Session *sess, int len, int size);
    ~SphHashTable();
    void  erase(const std::string &k);
    void  add(const std::string &k, const  std::string &v);
    std::string find(const std::string &k); 

public:
    struct SphHashNode
    {
        SphHashNode(const std::string &k, const std::string & v)
        :m_key(k),m_value(v),m_prev(NULL),m_next(NULL),m_isUse(false)
        {}
        std::string   m_key;
        std::string   m_value;
        SphHashNode  *m_prev;
        SphHashNode  *m_next;
        bool          m_isUse;
    }; 

private:
    SphHashNode **getBucket(const std::string&  k);
    SphHashNode *getNodeByValue(const std::string &k);
    std::string lookupEntry(const std::string &k);
    void  addEntry(const std::string &k, const std::string & v);
    void  freeEntry(const std::string &k);

    Session                    *m_sess; 
    std::vector<SphHashNode *> m_allocator;
    Hash<std::string>          m_hashFunc; 
    int                        m_len; //length of hashtable
    int                        m_size;
    SphHashNode**              m_nodes;
   //Comparestd::string<std::string> m_compare; //compare the std::string
};

} // namespace zhanmm

#endif

