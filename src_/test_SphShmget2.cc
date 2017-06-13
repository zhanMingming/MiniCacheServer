#include"SphShmget.h"
#include<iostream>
int main(void) {
    sleep(3);
    zhanmm::SphShmget s1(1234, 10000);
    void *dep = s1.attach();
    int *r = static_cast<int*>(dep);
    std::cout << *r << std::endl;
    while(1);
}
