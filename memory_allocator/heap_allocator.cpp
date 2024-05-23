#include <algorithm>
#include <assert.h>
#include <exception>
#include <iostream>
#include <stdint.h>

#include "heap_allocator.h"

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

//Alloces new chunk if there is a memory for allocing
void alloc_chunk(void* ptr, size_t size) {
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

//Changes free chunk arguments due to alloc new chunk
void crop_chunk(Chunk* chunk, size_t mem_used) {
    chunk->size -= mem_used;
    chunk->ptr = static_cast<void*>(static_cast<uint8_t*>(chunk->ptr) + mem_used);
}

void* heap_alloc(size_t size) {
    if (alloced_chunks.size == CHUNKS_CAP) {
        std::cerr << "Maximum number of chunks has been reached\n";
        return nullptr;
    }

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

//Erase chunk from alloced
void erase_chunk_from_alloced(size_t chunk_pos) {
    for (size_t i = chunk_pos + 1; i < alloced_chunks.size; ++i) {
        alloced_chunks.chunks[i - 1] = alloced_chunks.chunks[i];
    }

    --alloced_chunks.size;
}

//Trying to merge neighbouring freed chunks after any freeing
void try_to_merge_freed_chunks(size_t pos) {

    //Id of the leftmost chunk, which may be merged
    size_t left_pos_merge = pos;

    Chunk* chunks = freed_chunks.chunks;

    auto is_next_neighbouring = [&](size_t pos){
       return static_cast<void*>(static_cast<uint8_t*>((chunks + pos)->ptr) + (chunks + pos - 1)->size) == (chunks + pos)->ptr;
    };

    if (pos && is_next_neighbouring(pos - 1)) {
        left_pos_merge = pos - 1;
    }

    //Merging chunks from left_pos_merge while can
    for (size_t i = left_pos_merge; i + 1 < freed_chunks.size; ++i) {
        if (!is_next_neighbouring(i)) {
            
            if (i == left_pos_merge) {
                return;
            }

            for(size_t diff = 1; i + diff < freed_chunks.size; ++diff) {
                (chunks + left_pos_merge + diff)->ptr = (chunks + i + diff)->ptr;
                (chunks + left_pos_merge + diff)->size = (chunks + i + diff)->size;
            }

            return;
        } 

        (chunks + left_pos_merge)->size += (chunks + i + 1)->size;
    }

    freed_chunks.size = left_pos_merge + 1;
}

//Add chunk to freed
void add_chunk_to_freed(const Chunk& chunk) {

    if (freed_chunks.size == CHUNKS_CAP) {
        throw std::exception("Error: memory fragmentation");
    }

    free_heap_size += chunk.size;
    freed_chunks.chunks[freed_chunks.size++] = chunk;

    for (size_t i = alloced_chunks.size - 1; i > 0; --i) {

        Chunk& current_chunk = alloced_chunks.chunks[i];
        Chunk& previous_chunk = alloced_chunks.chunks[i - 1];

        if (current_chunk.ptr < previous_chunk.ptr) {
            std::swap(current_chunk, previous_chunk);
            continue;
        }

        try_to_merge_freed_chunks(i);
        return;
    }

    try_to_merge_freed_chunks(0);
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


        const Chunk& chunk = alloced_chunks.chunks[chunk_pos];

        if (chunk_pos >= chunk.size || chunk.ptr != ptr) {
            throw std::exception("Attempt to apply \"heap_free()\" with an invalid argument");
        }

        std::cout << "Freed chunk at " << ptr << " of size [" << chunk.size << "]\n";
        std::cout << "Free memory left: " << free_heap_size << '\n';

        add_chunk_to_freed(chunk);
        erase_chunk_from_alloced(chunk_pos);

    } catch(const std::exception& e){
        std::cerr << e.what() << '\n';
    }
}