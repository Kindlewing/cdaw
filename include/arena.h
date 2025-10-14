#ifndef ARENA_H
#define ARENA_H
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#ifndef DEFAULT_ALIGNMENT
#define DEFAULT_ALIGNMENT (2 * sizeof(void *))
#endif

typedef struct {
    size_t capacity;
    size_t offset;
    void *memory;
} arena;

bool is_power_of_two(uintptr_t x);
void arena_init(arena *a, size_t capacity);
void *arena_alloc(size_t size);

#endif // !ARENA_H
