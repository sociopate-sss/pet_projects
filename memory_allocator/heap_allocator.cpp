#include "heap_allocator.h"

#include <algorithm>
#include <assert.h>
#include <exception>
#include <iostream>
#include <stdint.h>

bool operator<(const Chunk& lhs, const Chunk& rhs) {
    if (lhs.ptr == rhs.ptr) 
        return lhs.size < rhs.size;
    return 0 < static_cast<uint8_t*>(rhs.ptr) - static_cast<uint8_t*>(lhs.ptr);
}

uint32_t free_heap_size = ALLOC_CAP;

uint8_t heap[ALLOC_CAP];

Chunk_list alloced_chunks{
    0,
    {}
};

Chunk_list freed_chunks{
    1,
    {
        {
            heap,
            ALLOC_CAP
        }
    }
};

void alloc_chunk(void* ptr, size_t size){
    // Allocs new chunk
    alloced_chunks.chunks[alloced_chunks.size] = {ptr, size};
    ++alloced_chunks.size;

    std::cout << "New chunk at " << ptr << " of size [" << size << "]\n";
    std::cout << "Free memory left: " << free_heap_size << '\n';

    // Restores the correct order
    for (size_t i = alloced_chunks.size - 1; i > 0; --i) {
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

    if (size == 0 || size > free_heap_size) {
        std::cerr << "Incorrect size value\n";
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

            free_heap_size -= size;
            alloc_chunk(ptr, size);

            return ptr;
        }
    }

    return nullptr;
}

// Finds right chunk with binary search and frees it
void heap_free(void* ptr){

    try{
        if (ptr == nullptr) {
            throw std::exception("Attempt to apply \"heap_free()\" to a nullptr");
        }

        size_t chunk_pos = static_cast<size_t>(
            std::lower_bound(alloced_chunks.chunks,
                            alloced_chunks.chunks + alloced_chunks.size, Chunk{ptr, 0}) -
            alloced_chunks.chunks);


        if (chunk_pos < alloced_chunks.size || alloced_chunks.chunks[chunk_pos].ptr != ptr) {
            throw std::exception("Attempt to apply \"heap_free()\" with an invalid argument");
        }

        free_heap_size += alloced_chunks.chunks[chunk_pos].size;

        std::cout << "Freed chunk at " << ptr << " of size [" << alloced_chunks.chunks[chunk_pos].size << "]\n";
        std::cout << "Free memory left: " << free_heap_size << '\n';

        for (size_t i = chunk_pos + 1; i < alloced_chunks.size; ++i) {
            alloced_chunks.chunks[i - 1] = alloced_chunks.chunks[i];
        }

        --alloced_chunks.size;
    } catch(const std::exception& e){
        std::cerr << e.what() << '\n';
    }
}