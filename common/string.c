//
// Created by christoph on 23.08.24.
//

#include <string.h>
#include <malloc.h>
#include <assert.h>

#include "common/string.h"
#include "utils/dataArena.h"

extern Arena* arena;

String string_create(char *data, size_t length) {

    size_t dataSize = sizeof(char) * length;
    char *stringPtr = (char *) arena_alloc(arena, dataSize);
    memcpy(stringPtr, data, length);

    return (String) {
        .data = stringPtr,
        .length = length
    };
}

String string_clone(String string) {

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

String string_concat(String string1, String string2) {

    if (string1.length == 0) {
        return string_clone(string2);
    } else if (string2.length == 0) {
        return string_clone(string1);
    }

    size_t concatLength = string1.length + string2.length;

    char *stringBuffer = malloc(sizeof(char) * concatLength);
    char *concatPtr = stringBuffer + string1.length;

    memcpy(stringBuffer, string1.data, string1.length);
    memcpy(concatPtr, string2.data, string2.length);

    String string = string_create(stringBuffer, concatLength);

    free(stringBuffer);

    return string;
}

size_t stringBuilder_append(StringBuilder* stringBuilder, String string) {

    char *permissions = "a+";
    if (!stringBuilder) {
        return 0;
    }
    // check whether stream is open yet,
    // ftell will return negative for the case where stream is closed
    if (ftell(stringBuilder->stream) < 0) {
        FILE *stream = open_memstream(&(stringBuilder->streamPtr), &(stringBuilder->bufferSize));
        stringBuilder->stream = stream;
    }

    size_t writtenChars = fwrite(string.data, sizeof(char), string.length, stringBuilder->stream);

    if (writtenChars < string.length) {
        fprintf(stderr, "String Builder extended size of stream. \n");
    }

    return writtenChars;
}

String stringBuilder_build(StringBuilder *stringBuilder) {

    char *permissions = "r";
    if (!stringBuilder || stringBuilder->stream == NULL) {
        fprintf(stderr, "Cannot build string from uninitialized StringBuilder. \n");
        return (String) { 0 };
    }

    size_t currentOffset = ftell(stringBuilder->stream);
    if (currentOffset < 0) {
        // negative return value indicates that stream is not open
        open_memstream(&(stringBuilder->streamPtr), &(stringBuilder->bufferSize));
    } else if (currentOffset > 0) {
        // if offset is positive, set back to start
        fseek(stringBuilder->stream, 0, SEEK_SET);
    }

    // make sure all buffered chars are written to stream
    fflush(stringBuilder->stream);

    // Finally create string from the buffer
    String createdString = string_create(stringBuilder->streamPtr, stringBuilder->bufferSize);

    return createdString;
}



