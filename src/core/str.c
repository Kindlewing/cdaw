#include "str.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

string string_create(const char *str) {
    if(str == NULL) {
        perror("Cannot create string from a null pointer");
        exit(EXIT_FAILURE);
    }
    string s = {0};
    size_t i = 0;
    // get length
    while(str[i] != '\0') {
        i += 1;
    }
    s.length = i;
    s.capacity = s.length + 1;
}
