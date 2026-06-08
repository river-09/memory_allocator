#include "allocate2.h"

#include <cstddef>
#include <sys/mman.h>
#include <unistd.h>

const size_t SIZE_CLASSES[] = {
    8,
    16,
    32,
    64,
    128,
    256,
    512,
    1024
};

MyAllocator::MyAllocator() {

    for(size_t i = 0; i < NUM_CLASSES; i++) {
        free_lists[i] = nullptr;
    }
}

size_t MyAllocator::getSizeClassIndex(size_t size) {

    for(size_t i = 0; i < NUM_CLASSES; i++) {

        if(size <= SIZE_CLASSES[i])
            return i;
    }

    return NUM_CLASSES;
}

size_t MyAllocator::getClassSizeFromIndex(size_t index) {

    if(index >= NUM_CLASSES)
        return 0;

    return SIZE_CLASSES[index];
}

void* MyAllocator::requestMem(size_t size) {

    void* ptr = mmap(
        nullptr,
        size,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1,
        0
    );

    if(ptr == MAP_FAILED)
        return nullptr;

    return ptr;
}

void MyAllocator::refill(size_t classIndex) {

    size_t payloadSize =getClassSizeFromIndex(classIndex);

    size_t blockSize =sizeof(BlockHeader) + payloadSize;

    const size_t NUM_BLOCKS = 64;

    char* chunk =(char*)requestMem(blockSize * NUM_BLOCKS);

    if(chunk == nullptr)return;

    for(size_t i = 0; i < NUM_BLOCKS; i++) {

        FreeBlockHeader* block =(FreeBlockHeader*)(chunk + i * blockSize);

        block->next =free_lists[classIndex];

        free_lists[classIndex] =block;
    }
}

void* MyAllocator::allocate(size_t size) {

    size_t classIndex =getSizeClassIndex(size);

    if(classIndex >= NUM_CLASSES)
        return nullptr;

    if(free_lists[classIndex] == nullptr)
        refill(classIndex);

    if(free_lists[classIndex] == nullptr)
        return nullptr;

    FreeBlockHeader* block =free_lists[classIndex];

    free_lists[classIndex] =block->next;

    BlockHeader* header =(BlockHeader*)block;

    header->classIndex =classIndex;

    return
        (char*)header +sizeof(BlockHeader);
}

void MyAllocator::deallocate(void* ptr) {

    if(ptr == nullptr)
        return;

    BlockHeader* header =(BlockHeader*)((char*)ptr -sizeof(BlockHeader));

    size_t classIndex =header->classIndex;

    FreeBlockHeader* block =(FreeBlockHeader*)header;

    block->next =free_lists[classIndex];

    free_lists[classIndex] =block;
}