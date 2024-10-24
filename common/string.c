//
// Created by christoph on 23.08.24.
//

#include <string.h>
#include <malloc.h>
#include <assert.h>

#include "common/string.h"
// include headers of forwards defined objects
#include "complex/complex.h"
#include "matrix/matrix.h"
#include "vector/vector.h"



#define COMPLEX_NUM_STRING_BUFSIZE 16

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

size_t stringBuilder_appendCharArray(StringBuilder* stringBuilder, const char *charArray, size_t arraySize) {

    if (!stringBuilder) {
        return 0;
    }
    // check whether stream is open yet,
    // ftell will return negative for the case where stream is closed
    if (ftell(stringBuilder->stream) < 0) {
        FILE *stream = open_memstream(&(stringBuilder->streamPtr), &(stringBuilder->bufferSize));
        stringBuilder->stream = stream;
    }

    size_t writtenChars = fwrite(charArray, sizeof(char), arraySize, stringBuilder->stream);

    if (writtenChars < arraySize) {
        fprintf(stderr, "String Builder extended size of stream. \n");
    }

    return writtenChars;
}

String stringBuilder_build(StringBuilder *stringBuilder) {

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

size_t stringBuilder_appendString(StringBuilder* stringBuilder, const String string){
    return stringBuilder_appendCharArray(stringBuilder, string.data, string.length);
}

size_t stringBuilder_appendComplex(StringBuilder* stringBuilder, const Complex complex) {

    size_t bufferSize = COMPLEX_NUM_STRING_BUFSIZE;
    // for now, simply use a buffer and assume size is large enough
    char complexBuffer[bufferSize];
    size_t numWritten = snprintf(complexBuffer, bufferSize, "%.4f + %.4fi",complex.re, complex.im);
    if (numWritten <= 0) {
        fprintf(stderr, "Appending complex array to stringBuilder exceeded buffer size \n");
        return numWritten;
    }

    return stringBuilder_appendCharArray(stringBuilder, complexBuffer, numWritten);
}

size_t stringBuilder_appendVector(StringBuilder* stringBuilder, const Vector vector) {

    size_t numWritten = 0;
    // [2 + 4i, 3 - 9i, ...]
    const char bracketStart[1] = "[";
    const char bracketEnd[1] = "]";
    const char separator[2] = ", ";

    numWritten += stringBuilder_appendCharArray(stringBuilder, bracketStart, sizeof(bracketStart));
    for (size_t i = 0; i < vector.size; i++) {
        const Complex complex = vector_getElement(vector, i).value;
        numWritten += stringBuilder_appendComplex(stringBuilder, complex);

        // Append separator only if we are not yet at the final element
        if (i < vector.size - 1) {
            numWritten += stringBuilder_appendCharArray(stringBuilder, separator, sizeof(separator));
        }
    }
    numWritten += stringBuilder_appendCharArray(stringBuilder, bracketEnd, sizeof(bracketEnd));

    return numWritten;
}


size_t stringBuilder_appendMatrix(StringBuilder* stringBuilder, const Matrix matrix) {

    size_t numWritten = 0;
    const char bracketStart[1] = "[";
    const char bracketEnd[1] = "]";
    const char separatorComma[1] = ",";
    const char separatorSemicolon[1] = ";";
    const char separatorSpace[1] = " ";

    // Matrix are displayed in the form [[2 + 1i, 8 - 2i]; [9 + 2i, 1 + 3i]]
    // where each inner bracket corresponds to a row within the matrix

    numWritten += stringBuilder_appendCharArray(stringBuilder, bracketStart, sizeof(bracketStart));
    for (size_t outerIndex = 0; outerIndex < matrix.numRows; outerIndex++) {
        numWritten += stringBuilder_appendCharArray(stringBuilder, bracketStart, sizeof(bracketStart));
        for (size_t innerIndex = 0; innerIndex < matrix.numColumns; innerIndex++) {
            const Complex complex = matrix_getElement(matrix, innerIndex, outerIndex);
            numWritten += stringBuilder_appendComplex(stringBuilder, complex);

            // append sperators only if it is not the last element in the row
            if(innerIndex < matrix.numColumns - 1) {
                numWritten += stringBuilder_appendCharArray(stringBuilder, separatorComma, sizeof(separatorComma));
                numWritten += stringBuilder_appendCharArray(stringBuilder, separatorSpace, sizeof(separatorSpace));
            }
        }
        numWritten += stringBuilder_appendCharArray(stringBuilder, bracketEnd, sizeof(bracketEnd));

        // append end of row separator only if it is not yet the last row
        if (outerIndex < matrix.numRows - 1) {
            numWritten += stringBuilder_appendCharArray(stringBuilder, separatorSemicolon, sizeof(separatorSemicolon));
            numWritten += stringBuilder_appendCharArray(stringBuilder, separatorSpace, sizeof(separatorSpace));
        }
    }
    numWritten += stringBuilder_appendCharArray(stringBuilder, bracketEnd, sizeof(bracketEnd));

    return numWritten;
}


size_t stringBuilder_appendVectorSet(StringBuilder* stringBuilder, const VectorSet vectorSet) {

    size_t numWritten = 0;
    const char braceStart[1] = "{";
    const char braceEnd[1] = "}";
    const char separatorComma[1] = ",";
    const char separatorSpace[1] = " ";

    // Vectorsets are displayed in the form {[1 + 2i, 3 + 4i], [8 + 2i, 4 + 3i]}
    // where each vector element is started and ended by brackets

    numWritten += stringBuilder_appendCharArray(stringBuilder, braceStart, sizeof(braceStart));
    size_t setSize = vectorSet_size(vectorSet);
    for (size_t vectorIndex = 0; vectorIndex < setSize; vectorIndex++) {
        const Vector vector = vectorSet_getVectorAtIndex(vectorSet, vectorIndex).data;
        numWritten += stringBuilder_appendVector(stringBuilder, vector);

        if (vectorIndex < setSize - 1) {
            numWritten += stringBuilder_appendCharArray(stringBuilder, separatorComma, sizeof(separatorComma));
            numWritten += stringBuilder_appendCharArray(stringBuilder, separatorSpace, sizeof(separatorSpace));
        }
    }
    numWritten += stringBuilder_appendCharArray(stringBuilder, braceEnd, sizeof(braceEnd));

    return numWritten;
}




