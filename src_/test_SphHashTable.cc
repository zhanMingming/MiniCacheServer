#include<iostream>
#include"SphHashTable.h"
#include<string>
#include<time.h>
using namespace std;
struct Node
{
    Node()
    :key(0),value(0)
    {}
    int key;
    int value;
};
const int num = 1000000;
int main(void) {
    time_t t1 = time(0);
    Node *p = new Node;
    zhanmm::SphHashTable<int, Node*> hashTable0(999991);
    zhanmm::SphHashTable<const char *, Node*> hashTable1(999991);
    zhanmm::SphHashTable<std::string, Node*>  hashTable2(999991);
    for (int index= 0; index != num; ++index) {
        hashTable0.add(index, p);
    }
    for (int index= 0; index != num; ++index) {
        std::cout << hashTable0.find(index)->key;
    }
    time_t t2 = time(0);
    std::cout << t2 - t1 << std::endl; 

    std::string s2("iam");
    Node *p2 =new Node;
    p->key = 1;
    p->value = 2;
    std::cout << s2 << std::endl;
    for (int index = 0; index != num; ++index) {
        s2.append(1, '0' + index);
        hashTable2.add(s2, p2);
    }

    time_t t3 = time(0);
    std::cout << t3 -t2 << std::endl; 
    
}


     

