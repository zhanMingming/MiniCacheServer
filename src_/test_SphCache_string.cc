#include<cstdio>
#include<cstdlib>
#include"common.h"
#include"SphCache_string.h"
#include"SphHashTable_string.h"
int main(void) 
{
    Session *sess = new Session;
    sess->shared = malloc(100000);
    sess->size = 100000;
    sess->threadNums = 4;
    zhanmm::SphCache  cache(sess);
    std::string s1= "1234";
    std::string s2= "iamaking";
    cache.set(s1, s2);
    std::cout << cache.get(s1) << std::endl;
}
 
