#include "allocate.h"
#include <iostream>
using namespace std;
int main(){
    MyAllocator alloc;
    void* a = alloc.allocate(100);
    void* b = alloc.allocate(200);
    void* c = alloc.allocate(300);

    std::cout << "A = " << a << '\n';
    std::cout << "B = " << b << '\n';
    std::cout << "C = " << c << '\n';

    return 0;
}