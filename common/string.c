//
// Created by christoph on 23.08.24.
//

#include <string.h>
#include <malloc.h>

#include "common/string.h"
#include "utils/dataArena.h"

String string_create(Arena *arena, char *data, size_t length) {

    size_t dataSize = sizeof(char) * length;
    char *stringPtr = (char *) arena_alloc(arena, dataSize);
    memcpy(stringPtr, data, length);

    return (String) {
        .data = stringPtr,
        .length = length
    };
}

String string_clone(Arena *arena, String string) {

    char *stringPtr = (char *) arena_alloc(arena, string.length);
    memcpy(stringPtr, string.data, string.length);

    return (String) {
        .data = stringPtr,
        .length = string.length
    };
}

size_t string_getLength(String string) {
    return string.length;
}

String string_concat(Arena *arena, String string1, String string2) {

    if (string1.length == 0) {
        return string_clone(arena, string2);
    } else if (string2.length == 0) {
        return string_clone(arena, string1);
    }

    size_t concatLength = string1.length + string2.length;

    char *stringBuffer = malloc(sizeof(char) * concatLength);
    char *concatPtr = stringBuffer + string1.length;

    memcpy(stringBuffer, string1.data, string1.length);
    memcpy(concatPtr, string2.data, string2.length);

    String string = string_create(arena, stringBuffer, concatLength);

    free(stringBuffer);

    return string;
}

