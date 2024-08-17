//
// Created by christoph on 17.08.24.
//

#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include "dataArena.h"

// By default, allocate 100 MB for the arena
#define ARENA_DEFAULT_SIZE (1024 * 1024 * 100)


Arena* arena_init(void) {

    Arena *arena = malloc(sizeof(Arena));

    arena->arena_buffer = malloc(ARENA_DEFAULT_SIZE);
    arena->arena_offset = 0;
    arena->arena_buffer_length = ARENA_DEFAULT_SIZE;

    return arena;
}

void arena_destroy(Arena *arena) {

    memset(arena->arena_buffer, 0, ARENA_DEFAULT_SIZE);
    free(arena->arena_buffer);

    arena->arena_offset = 0;
    arena->arena_buffer_length = 0;

    free(arena);
}

void *arena_alloc(Arena *arena, size_t size) {

    // check if size is available
    if (arena->arena_offset + size > arena->arena_buffer_length) {
        fprintf(stderr, "FATAL ERROR: Arena exceeded maximal available memory size. Exiting ... \n");
        exit(EXIT_SUCCESS);
    }
    // return pointer to the current (buffer + offset)
    void *dataPtr = (void * ) (arena->arena_buffer + arena->arena_offset);
    // increment offset by size
    arena->arena_offset += size;

    memset(arena->arena_buffer, 0, size);
    return dataPtr;
}