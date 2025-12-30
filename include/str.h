#ifndef STR_H
#define STR_H
#include <X11/Xlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "typedefs.h"

typedef struct {
    char *data;
    size_t length;
    size_t capacity;
} string;

string string_from_cstring(const char *str);
string string_slice(string *str, u8 start, u8 end);

rune string_at(string *str, u8 at);
bool string_is_valid(string *str);
bool string_eq(string *str1, string *str2);

#endif // STR_H
