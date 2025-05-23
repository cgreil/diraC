//
// Created by christoph on 23.08.24.
//

#include <string.h>
#include <malloc.h>
#include <assert.h>
#include <errno.h>

#include "common/string.h"

// 16 chars for digits, 1 for sign, 1 for complex i
#define COMPLEX_NUM_STRING_BUFSIZE 18

#define STRING_SIZE(str) sizeof((str)) / sizeof(char)

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

String string_fromStream(FILE *stream, size_t length) {

    if (stream == NULL || length == 0) {
        return (String) { 0 };
    }

    size_t dataSize = sizeof(char) * length;
    char *stringPtr = (char *) arena_alloc(arena, dataSize);

    // set stream ptr to start of file
    fseek(stream, 0L, SEEK_SET);
    // Get access to filestream
    size_t numRead = fread(stringPtr, sizeof(char), length, stream);

    // check if end of readable filestream was hit before reading <length> values
    if (feof(stream)) {
        fprintf(stderr, "Encountered EOF before reading end of stream \n");
    }
    // check if error occurred during read
    else if (ferror(stream)) {
        fprintf(stderr, "Error happened while reading stream \n");
    }
    clearerr(stream);

    return (String) {
        .data = stringPtr,
        .length = numRead
    };
}


String string_fromCString(char* data) {

    /**
     * Creates a String struct from a C like string, 
     * i.e. a null-terminated char array as passed by
     * a char pointer. 
     * It is the (functions) users responsibility to make sure
     * a null terminator exists
     */

     size_t stringSize = strlen(data);
     return string_create(data, stringSize);
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

StringBuilder* stringBuilder_create(void) {

    StringBuilder* stringBuilder = calloc(1, sizeof(StringBuilder));
    stringBuilder->streamPtr = "";
    stringBuilder->floatAccuracy = 4;

    return stringBuilder;
}

void stringBuilder_destroy(StringBuilder* stringBuilder) {
    free(stringBuilder);
}

size_t stringBuilder_appendCharArray(StringBuilder* stringBuilder, const char *charArray, size_t arraySize) {

    if (!stringBuilder) {
        return 0;
    }
    // check whether stream is open yet,
    // ftell will return negative for the case where stream is closed
    if (stringBuilder->stream == NULL) { //|| ftell(stringBuilder->stream) < 0) {
        FILE *stream = open_memstream(&(stringBuilder->streamPtr), &(stringBuilder->bufferSize));
        if (stream == NULL) {
            fprintf(stderr, "Could not open memstream \n");
        }
        stringBuilder->stream = stream;
    }

    size_t writtenChars = fwrite(charArray, sizeof(char), arraySize, stringBuilder->stream);

    if (writtenChars < arraySize) {
        fprintf(stderr, "String Builder extended size of stream. \n");
    }
    stringBuilder->bufferSize += writtenChars;

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
    }

    // make sure all buffered chars are written to stream
    int flushRet = fflush(stringBuilder->stream);
    if (flushRet == EOF) {
        if (errno == EBADF) {
            fprintf(stderr, "Could not flush stream as it is not open for writing \n");
            return (String) { 0 };
        }
        else {
            fprintf(stderr, "Could not flush stream due to unknown reason \n");
            return (String) { 0 };
        }
    }

    // Finally create string from the buffer
    String createdString = string_fromStream(stringBuilder->stream, stringBuilder->bufferSize);

    return createdString;
}

size_t stringBuilder_appendString(StringBuilder* stringBuilder, const String string){
    return stringBuilder_appendCharArray(stringBuilder, string.data, string.length);
}

size_t stringBuilder_appendComplex(StringBuilder* stringBuilder, const Complex complex) {


    size_t bufferSize = COMPLEX_NUM_STRING_BUFSIZE;
    size_t numWritten = 0;
    // for now, simply use a buffer and assume size is large enough
    char complexBuffer[bufferSize];
    int formatSize =  snprintf(complexBuffer, bufferSize, "%.3f + %.3fi",complex.re, complex.im);    
    numWritten += stringBuilder_appendCharArray(stringBuilder, complexBuffer, formatSize);

    return numWritten;
}

size_t stringBuilder_appendVector(StringBuilder* stringBuilder, const Vector vector) {

    size_t numWritten = 0;
    // [2 + 4i, 3 - 9i, ...]
    const char bracketStart[1] = "[";
    const char bracketEnd[1] = "]";
    const char separator[2] = ", ";

    numWritten += stringBuilder_appendString(stringBuilder, string_fromCString("Vector\n"));
    numWritten += stringBuilder_appendCharArray(stringBuilder, bracketStart, STRING_SIZE(bracketStart));
    for (size_t i = 0; i < vector.size; i++) {
        const Complex complex = vector_getElement(vector, i).value;
        numWritten += stringBuilder_appendComplex(stringBuilder, complex);

        // Append separator only if we are not yet at the final element
        if (i < vector.size - 1) {
            numWritten += stringBuilder_appendCharArray(stringBuilder, separator, STRING_SIZE(separator));
        }
    }
    numWritten += stringBuilder_appendCharArray(stringBuilder, bracketEnd, STRING_SIZE(bracketEnd));

    return numWritten;
}

size_t stringBuilder_appendMatrix(StringBuilder* stringBuilder, const Matrix matrix) {

    size_t numWritten = 0;
    const char bracketStart[1] = "[";
    const char bracketEnd[1] = "]";
    const char separatorComma[1] = ",";
    const char separatorSpace[1] = " ";
    const char separatorNewline[1] = "\n";

    numWritten += stringBuilder_appendString(stringBuilder, string_fromCString("Matrix\n"));
    // Matrix are displayed in the form [[2 + 1i, 8 - 2i]; [9 + 2i, 1 + 3i]]
    // where each inner bracket corresponds to a row within the matrix
    numWritten += stringBuilder_appendCharArray(stringBuilder, bracketStart, STRING_SIZE(bracketStart));
    for (size_t outerIndex = 0; outerIndex < matrix.numRows; outerIndex++) {
        numWritten += stringBuilder_appendCharArray(stringBuilder, bracketStart, STRING_SIZE(bracketStart));
        for (size_t innerIndex = 0; innerIndex < matrix.numColumns; innerIndex++) {
            const Complex complex = matrix_getElement(matrix, outerIndex, innerIndex);
            numWritten += stringBuilder_appendComplex(stringBuilder, complex);

            // append sperators only if it is not the last element in the row
            if(innerIndex < matrix.numColumns - 1) {
                numWritten += stringBuilder_appendCharArray(stringBuilder, separatorComma, STRING_SIZE(separatorComma));
                numWritten += stringBuilder_appendCharArray(stringBuilder, separatorSpace, STRING_SIZE(separatorSpace));
            }
        }
        numWritten += stringBuilder_appendCharArray(stringBuilder, bracketEnd, STRING_SIZE(bracketEnd));

        // append end of row separator only if it is not yet the last row
        if (outerIndex < matrix.numRows - 1) {
            numWritten += stringBuilder_appendCharArray(stringBuilder, separatorSpace, STRING_SIZE(separatorSpace));
            numWritten += stringBuilder_appendCharArray(stringBuilder, separatorNewline, STRING_SIZE(separatorNewline));
        }
    }
    numWritten += stringBuilder_appendCharArray(stringBuilder, bracketEnd, STRING_SIZE(bracketEnd));
    numWritten += stringBuilder_appendCharArray(stringBuilder, separatorNewline, STRING_SIZE(separatorNewline));

    return numWritten;
}


size_t stringBuilder_appendVectorSet(StringBuilder* stringBuilder, const VectorCollection vectorSet) {

    size_t numWritten = 0;
    const char braceStart[1] = "{";
    const char braceEnd[1] = "}";
    const char separatorComma[1] = ",";
    const char separatorSpace[1] = " ";

    // Vectorsets are displayed in the form {[1 + 2i, 3 + 4i], [8 + 2i, 4 + 3i]}
    // where each vector element is started and ended by brackets

    numWritten += stringBuilder_appendCharArray(stringBuilder, braceStart, STRING_SIZE(braceStart));
    size_t setSize = vectorCollection_size(vectorSet);
    for (size_t vectorIndex = 0; vectorIndex < setSize; vectorIndex++) {
        const Vector vector = vectorCollection_getVectorAtIndex(vectorSet, vectorIndex).data;
        numWritten += stringBuilder_appendVector(stringBuilder, vector);

        if (vectorIndex < setSize - 1) {
            numWritten += stringBuilder_appendCharArray(stringBuilder, separatorComma, STRING_SIZE(separatorComma));
            numWritten += stringBuilder_appendCharArray(stringBuilder, separatorSpace, STRING_SIZE(separatorSpace));
        }
    }
    numWritten += stringBuilder_appendCharArray(stringBuilder, braceEnd, STRING_SIZE(braceEnd));

    return numWritten;
}


size_t stringBuilder_appendQuantumRegister(StringBuilder* stringBuilder, const QuantumRegister qureg) {

    size_t numWritten = 0;
    const char vertLine[1] = "|";
    //const char langle[1] = "<";
    const char rangle[1] = ">";
    const char plus[1] = "+";
    const char seperatorSpace[1] = " ";
    const char parenthOpen[1] = "(";
    const char parenthClose[1] = ")";

    // Quantum Registers are displayed in the form 
    // (alpha_1 + beta_1 i) |00...0> + ... + (alpha_n + beta_n i) |11...1>
    // where n is number of amplitudes i.e. number of qubits and 
    // the words inside the kets correspond to the binary represnation 
    // of each individual qubit  
   
    
    int numBits = (int) log2((double) qureg.numQubits); 

    for (size_t qubit = 0; qubit < qureg.numQubits; qubit++) {
        
        char bitArray[numBits];
        size_t qubitDecimal = qubit;

        for (size_t bitIndex = numBits; qubitDecimal > 0 && bitIndex >= 0; bitIndex -= 1) {
            bitArray[bitIndex] = qubitDecimal % 2;
            qubitDecimal /= 2; 
        }
    
        numWritten += stringBuilder_appendCharArray(stringBuilder, parenthOpen, STRING_SIZE(parenthOpen));
        numWritten += stringBuilder_appendComplex(stringBuilder, qureg.stateVector.dataArray.values[qubit]);
        numWritten += stringBuilder_appendCharArray(stringBuilder, parenthClose, STRING_SIZE(parenthClose));
        numWritten += stringBuilder_appendCharArray(stringBuilder, seperatorSpace, STRING_SIZE(seperatorSpace));

        numWritten += stringBuilder_appendCharArray(stringBuilder, vertLine, STRING_SIZE(vertLine));
        numWritten += stringBuilder_appendCharArray(stringBuilder, bitArray, numBits);
        numWritten += stringBuilder_appendCharArray(stringBuilder, rangle, STRING_SIZE(rangle));

        if (qubit != qureg.numQubits - 1) {
            numWritten += stringBuilder_appendCharArray(stringBuilder, seperatorSpace, STRING_SIZE(seperatorSpace));
            numWritten += stringBuilder_appendCharArray(stringBuilder, plus, STRING_SIZE(plus));
            numWritten += stringBuilder_appendCharArray(stringBuilder, plus, STRING_SIZE(plus));
        } 
    }
    return numWritten;
}




