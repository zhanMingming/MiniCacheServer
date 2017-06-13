#include<iostream>
#include"SphCache.h"
#include<string>
#include<cstdio>

int main(void) {
    
    zhanmm::SphCache<std::string, std::string>  cache(9991);
    time_t t1 = time(0);
    std::string s1("iamgoddsa");
    std::string s2("iamgoodasjdpoaidpoiooooooooooooooooooooooooaidpoaidpoasipodpoasidpoiaspodiasp");
    cache.set(s1, s2);
    std::cout << cache.get(s1) << std::endl;
    char ctr[10]={0};
    for(int index = 0; index <= 1000000; ++index) {
        sprintf(ctr,"%d", index);
        std::string s1(ctr); 
        cache.set(s1, s2);
    }
    time_t t2 = time(0);
    std::cout << t2 -t1 << std::endl;
}

