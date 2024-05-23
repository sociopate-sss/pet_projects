#include <iostream>

#include "heap_allocator.h"

int main(){
    void* ptrs[10];
    for (size_t i = 0; i < 10; i++) {
        ptrs[i] = heap_alloc(i);
        if(i > 1 && i % 2 == 0) {
            heap_free(ptrs[i / 2]);
        }
    }
    
    //Wrong arguments cases
    heap_alloc(10000000000);
    heap_free(nullptr);
    heap_free(static_cast<void*>(static_cast<uint8_t*>(ptrs[9]) - 1));

    return 0;
}