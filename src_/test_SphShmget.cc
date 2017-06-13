#include"SphShmget.h"
#include<iostream>
int main(void) {
    zhanmm::SphShmget s1(1234, 10000);
    void *dep = s1.attach();
    int *r = static_cast<int*>(dep);
    *r = 1;
    while(1);

}

