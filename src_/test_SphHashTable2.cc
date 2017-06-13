#include"common.h"
#include"SphCache.h"
#include<cstdio>
#include<cstdlib>
#include<string>
#include<iostream>
#include<string.h>

int main(void)
{
    Session *sess = new Session;
    sess->shared = malloc(100000);
    memset(sess->shared, 0, 100000);
    DEBUG("sherd");
    sess->size = 100000;
    sess->threadNums = 4;
    std::string s2 = "iamzhan";
    std::string s1 = "1234";
    zhanmm::SphCache<std::string, std::string> cache(sess);
    cache.set(s1, s2);
    std::cout << cache.get(s1) << std::endl;
}

 
