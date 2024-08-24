//
// Created by christoph on 23.08.24.
//

#ifndef LEIBNITZ_STRING_H
#define LEIBNITZ_STRING_H

#include <stddef.h>

#include "common/string.h"
#include "utils/dataArena.h"

typedef struct {
    char *data;
    size_t length;
}String;

typedef struct {
    int a;
    // TODO: implement StringBuilder
    //https://www.reddit.com/r/C_Programming/comments/13zqjmz/string_builders_in_c/
}StringBuilder;

String string_clone(Arena *arena, String string);

String string_create(Arena *arena, char *data, size_t length);

size_t string_getLength(String string);

String string_concat(Arena *arena, String string1, String string2);


#endif //LEIBNITZ_STRING_H
