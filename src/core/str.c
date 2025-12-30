#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "str.h"

string string_from_cstring(const char *str) {
    if(str == NULL) {
        fprintf(stderr, "Cannot create string from a null pointer\n");
        exit(EXIT_FAILURE);
    }
    string s = {0};

    // get length
    size_t len = 0;
    while(str[len] != '\0') {
        len += 1;
    }
    s.length = len;
    s.capacity = s.length + 1;
    s.data = malloc(sizeof(s.capacity));
    if(!s.data) {
        fprintf(stderr, "Malloc failed\n");
        exit(EXIT_FAILURE);
    }
    size_t i = 0;
    while(str[i] != '\0') {
        s.data[i] = str[i];
        i += 1;
    }
    s.data[s.capacity] = '\0';
    return s;
}

string string_slice(string *str, u8 start, u8 end) {
    string slice = {0};
    if(!string_is_valid(str) || start >= end || end > str->length) {
        return slice;
    }
    slice.data = str->data + start;
    slice.length = end;
    slice.capacity = end + 1;
    return slice;
}

rune string_at(string *str, u8 at) {
    if(at > str->length) {
        fprintf(stderr, "index out of bounds\n");
        exit(EXIT_FAILURE);
    }
    return str->data[at];
}

bool string_eq(string *str1, string *str2) {
    if(str1->length != str2->length || str1->capacity != str2->capacity) {
        return false;
    }
    u8 curr = 0;
    while(curr < str1->length) {
        if(string_at(str1, curr) != string_at(str2, curr)) {
            return false;
        }
    }
    return true;
}

bool string_is_valid(string *str) {
    return str->data != NULL && str->length > 0;
}
