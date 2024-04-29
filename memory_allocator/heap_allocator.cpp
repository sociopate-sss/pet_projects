#include "heap_allocator.h"

#include <algorithm>
#include <assert.h>
#include <stdint.h>

uint8_t heap[ALLOC_CAP];

Chunk_list alloced_chunks{
    .size = 0,
    .chunks = {}
};

Chunk_list freed_chunks{
    .size = 1,
    .chunks = {
        {
            .ptr = heap,
            .size = ALLOC_CAP
        }
    }
};

void alloc_chunk(void* ptr, size_t size){
    // Allocs new chunk
    alloced_chunks.chunks[alloced_chunks.size] = {ptr, size};
    ++alloced_chunks.size;

    // Restores the correct order
    for (size_t i = alloced_chunks.size - 1; i > 0; --i){
        if (alloced_chunks.chunks[i - 1].ptr < ptr) 
            break;

        std::swap(alloced_chunks.chunks[i], alloced_chunks.chunks[i - 1]);
    }
}

void crop_chunk(Chunk* chunk, size_t mem_used){
    chunk->size -= mem_used;
    chunk->ptr = static_cast<void*>(static_cast<uint8_t*>(chunk->ptr) + mem_used);
}

void* heap_alloc(size_t size){
    /*
        TODO: find the smallest by size correct chunk
    */

    if (size == 0){
        return nullptr;
    }

    for (size_t i = 0; i < freed_chunks.size; ++i) {
        if (freed_chunks.chunks[i].size >= size) {

            void* ptr = freed_chunks.chunks[i].ptr;

            crop_chunk(&freed_chunks.chunks[i], size);
            if (freed_chunks.chunks[i].size == 0) {
                for (size_t j = i + 1; j < freed_chunks.size; ++j) {
                    freed_chunks.chunks[j] = freed_chunks.chunks[j + 1];
                }
                --freed_chunks.size;
            }

            alloc_chunk(ptr, size);

            return ptr;
        }
    }

    return nullptr;
}

// Finds right chunk with binary search and frees it
void heap_free(void* ptr){
    size_t chunk_pos = static_cast<size_t>(
        std::lower_bound(alloced_chunks.chunks,
                         alloced_chunks.chunks + alloced_chunks.size, ptr) -
        alloced_chunks.chunks);

    assert(chunk_pos < alloced_chunks.size);



    for (size_t i = chunk_pos + 1; i < alloced_chunks.size; ++i) {
        alloced_chunks.chunks[i - 1] = alloced_chunks.chunks[i];
    }

    --alloced_chunks.size;
}