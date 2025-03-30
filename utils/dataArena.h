//
// Created by christoph on 17.08.24.
//

#ifndef DATAARENA_H
#define DATAARENA_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Custom data allocation macros
#define arena_allocate(arena, type) (type *) arena_alloc(arena, sizeof(type))
#define arena_allocate_count(arena, type, count) (type *) arena_alloc(arena, (count) * sizeof(type))

typedef struct {
    unsigned char *arena_buffer;
    size_t arena_buffer_length;
    size_t arena_offset;
} Arena;

Arena* arena_init(void);

void arena_destroy(Arena *arena);

void *arena_alloc(Arena *arena, size_t size);



#endif //ATAARENA_H
