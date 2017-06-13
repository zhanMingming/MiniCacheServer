#ifndef SPHCACHE_HASH_H
#define SPHCACHE_HASH_H
#include<string>
namespace zhanmm {

typedef unsigned int size_t;
template<class T>
class Hash
{ 
public:
    size_t operator()(const T &lhs) { return 0;}
};

template<class Key>
class CompareKey
{    
public:
    bool operator()(const Key* &lhs, const Key* &rhs) { return 0;}
};

template<>
class CompareKey<std::string>
{
public:
    bool operator()(const std::string * &lhs, const std::string * &rhs) {
        if ( 0 ==  lhs->compare(*rhs)) {
            return true;
        } else {
            return false;
        }
    }
};

template<>
class CompareKey<int>
{
public:
    bool operator()(const int* & lhs, const int* & rhs) {
        return  *lhs == *rhs;
    }
};

 

template<>
class Hash<std::string>
{
public:
    typedef size_t result_type;
    typedef std::string argument_type;
    result_type operator()(const argument_type &lhs) {
        result_type h = 0;
        for (int index = 0; index != lhs.size(); ++index) {
            h = lhs[index] + (h << 6) + (h << 16) - h;
        }
        return h;
    }
};
template<>
class Hash<int>
{
public:
    typedef size_t result_type;
    typedef int argument_type;
    result_type operator()(const argument_type &lhs) {
        result_type h = lhs;
        return h;
    }
};


} // namespace zhanmm
#endif 

