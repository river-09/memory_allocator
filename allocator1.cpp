#include "allocate.h"
#include <cstddef>
#include <sys/mman.h>
#include <unistd.h>
using namespace std;
constexpr size_t pool_size=128*1024*1024;

MyAllocator::MyAllocator(){
    memstart=requestMem(pool_size);

    if(memstart==nullptr){
        free_list=nullptr;
        return;
    }

    free_list=(Header*)memstart;
    free_list->size=pool_size;
    free_list->next=nullptr;
}


MyAllocator::~MyAllocator(){
    if(memstart!=nullptr){
        munmap(memstart, pool_size);
    }
}

void* MyAllocator::requestMem(size_t size){
    void* ptr=mmap(
        nullptr,
        size,
        PROT_READ| PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1, 
        0
    );

    if(ptr==MAP_FAILED)return nullptr;

    return ptr;

}


void* MyAllocator::allocate(size_t size){
    if(size==0)return nullptr;

    size_t total_size=size+sizeof(Header);
    Header* curr=free_list;
    Header* prev=nullptr;

    while(curr){
        if(curr->size >= total_size){
            if(curr->size>=total_size+sizeof(Header)){
                Header* new_free=(Header*)((char*)curr+total_size);
                new_free->size=curr->size-total_size;
                new_free->next=curr->next;
                if(prev){
                    prev->next=new_free;
                    
                }else{
                    free_list=new_free;
                }

                curr->size=total_size;
            }
            else{
                if(prev){
                    prev->next=curr->next;
                    
                }else{
                    free_list=curr->next;
                }
            }


            return (char*)curr+sizeof(Header);//skip headerand move touser area
        }
        prev=curr;
        curr=curr->next;
    }
    return nullptr;

}
void MyAllocator::free(void* ptr){
    if(ptr==nullptr)return;

    Header* block=(Header*)((char*)ptr-sizeof(Header));

    if(free_list == nullptr || block < free_list){

        block->next = free_list;
        free_list = block;
    }
    else{

        Header* curr = free_list;

        while(curr->next && curr->next < block){
            curr = curr->next;
        }

        block->next = curr->next;
        curr->next = block;
    }

    // coalesce

    Header* curr = free_list;

    while(curr && curr->next){

        if((char*)curr + curr->size ==
           (char*)curr->next){

            curr->size += curr->next->size;
            curr->next = curr->next->next;
        }
        else{
            curr = curr->next;
        }
    }

}

