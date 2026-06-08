#pragma once

#include <cstddef> 


class MyAllocator {

public:
    MyAllocator();
    void* allocate(size_t size);
    void deallocate(void* ptr);

    struct BlockHeader {
        size_t classIndex;
    };

    struct FreeBlockHeader {
        FreeBlockHeader* next;
    };

    static size_t getSizeClassIndex(size_t size);
    static size_t getClassSizeFromIndex(size_t index);
private:

    static constexpr size_t NUM_CLASSES = 8;

    FreeBlockHeader* free_lists[NUM_CLASSES];

    void refill(size_t classIndex);

    void* requestMem(size_t size);
    
};