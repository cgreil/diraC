//
// Created by christoph on 23.08.24.
//

#ifndef LEIBNITZ_STRING_H
#define LEIBNITZ_STRING_H

#include <stddef.h>
#include <stdio.h>

#include "utils/dataArena.h"
#include "common/string.h"
#include "complex/complex.h"
#include "matrix/matrix.h"
#include "vector/vector.h"
#include "common/vectorSet.h"


typedef struct {
    char *data;
    size_t length;
}String;

typedef struct {
    FILE *stream;
    char *streamPtr;
    size_t bufferSize;
    size_t floatAccuracy;
    // https://www.reddit.com/r/C_Programming/comments/13zqjmz/string_builders_in_c/
}StringBuilder;

String string_clone(String string);

String string_create(char *data, size_t length);

size_t string_getLength(String string);

String string_concat(String string1, String string2);

size_t stringBuilder_appendString(StringBuilder* stringBuilder, String string);

String stringBuilder_build(StringBuilder* stringBuilder);

size_t stringBuilder_appendCharArray(StringBuilder* stringBuilder, const char* charArray, size_t arraySize);

size_t stringBuilder_appendVector(StringBuilder* stringBuilder, Vector vector);

size_t stringBuilder_appendMatrix(StringBuilder* stringBuilder, Matrix matrix);

size_t stringBuilder_appendComplex(StringBuilder* stringBuilder, Complex complex);

size_t stringBuilder_appendVectorSet(StringBuilder* stringBuilder, VectorSet vectorSet);

void stringBuilder_setFloatPrecision(StringBuilder* stringBuilder, size_t floatAccuracy);


#endif //LEIBNITZ_STRING_H
