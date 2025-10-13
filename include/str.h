#include <stdint.h>

typedef struct {
    const char *data;
    uint8_t length;
    uint8_t capacity;
} string;

string string_create(const char *str);
