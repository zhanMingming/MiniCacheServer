#include"hash.h"
#include<iostream>
int main(void) {
    zhanmm::Hash<int> hs;
    std::cout << hs(10) << std::endl;
    zhanmm::Hash<std::string> hstr;
    std::cout << hstr("iam goos") << std::endl;
}

