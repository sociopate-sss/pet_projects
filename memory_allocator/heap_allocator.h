#pragma once

#include <stdint.h>

#define ALLOC_CAP 1024 * 1024 * 2
#define CHUNKS_CAP 1024

typedef struct {
    void* ptr;
    size_t size;
} Chunk;

typedef struct {
    size_t size;
    Chunk chunks[CHUNKS_CAP];
} Chunk_list;

void* heap_alloc(size_t size);

void heap_free(void* ptr);

