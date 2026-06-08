#include "allocate2.h"
#include <iostream>

int main() {

    MyAllocator alloc;

    void* a = alloc.allocate(25);

    std::cout << a << '\n';

    alloc.deallocate(a);

    void* b = alloc.allocate(20);

    std::cout << b << '\n';
}