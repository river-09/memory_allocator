#pragma once

#include <cstddef>

class MyAllocator{

    struct Header{  // every block  has size and pointer to next block
        size_t size;
        Header* next;
    };

    void* memstart;  //mmap return will be stored here
    Header* free_list; // points to first empty block
public:
    MyAllocator();
    ~MyAllocator();
    void* allocate(size_t size); //return pointer to allocated block
    void free(void *ptr); //free the allocatedblock


private:
    void* requestMem(size_t size);





};