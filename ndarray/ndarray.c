#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <utils/dataArena.h>
#include <memory.h>
#include <stdio.h>

#include "complex/complex.h"
#include "ndarray/ndarray.h"
#include "utils/dataArena.h"

extern Arena* arena;

size_t NDArray_index(size_t numArrayRows, size_t rowIndex, size_t columnIndex) {
    // helper function to calculate index of an element with given row and column indices
    return rowIndex + numArrayRows * columnIndex;
}

NDArray NDArray_create(size_t numRows, size_t numColumns, Complex *values) {

    /**
     * NDArray_create takes
     * @param numColumns
     * @param numRows
     * @param values
     *
     * and returns
     * @return NDArray
     *
     * where values is a row-major-array of complex numbers, that is
     * the first (numColumns) entries of values form the first row
     * of the ndArray and so on
     *
     * The returned NDArray will be column-major i.e. inside its data
     * array, the first numRow elements will make up the first columns
     *
     */

    assert(values != NULL);
    size_t numElements = numRows * numColumns;
    // allocate ndarray space
    Complex *ndArrayData = arena_allocate_count(arena, Complex, numElements);

    // store given complex values into the ndarray, performing a conversion
    // from row-major to column-major in the process
    for (size_t dataIndex = 0; dataIndex < numElements; dataIndex++) {

        // matrix positions of the row major input array are given by
        size_t rowPos = dataIndex / numColumns;
        size_t colPos = dataIndex % numColumns;

        size_t outputIndex = NDArray_index(numRows, rowPos, colPos);
        ndArrayData[outputIndex] = complex_clone(values[dataIndex]);
    }

    // TODO: Calculate CRC checksum

    return (NDArray) {
        .numRows = numRows,
        .numColumns = numColumns,
        .values = ndArrayData,
        .checksum = 0
    };
}

NDArray NDArray_clone(NDArray ndArray) {

    size_t numElements = ndArray.numRows * ndArray.numColumns;

    Complex *clonedValues = arena_allocate_count(arena, Complex, numElements);

    memcpy(clonedValues, ndArray.values, numElements * sizeof(Complex));

    return (NDArray) {
        .numRows = ndArray.numRows,
        .numColumns = ndArray.numColumns,
        .values = clonedValues,
        .checksum = 0
    };
}

NDArray NDArray_resize(NDArray ndArray, size_t numRows, size_t numColumns) {

    assert(ndArray.numColumns * ndArray.numRows == numRows * numColumns);

    if (numRows == ndArray.numRows && numColumns == ndArray.numColumns) {
        return ndArray;
    }

    //size_t numElements = ndArray.numRows * ndArray.numColumns;
    //Complex values[numElements];

    return (NDArray) { 0 };
}

OptComplex NDArray_getElement(NDArray ndArray, size_t rowIndex, size_t columnIndex) {

    if (rowIndex > ndArray.numRows || columnIndex > ndArray.numColumns) {
        return (OptComplex) {
            .value = (Complex) { 0 },
            .valid = false
        };
    }

    size_t dataPosition = NDArray_index(ndArray.numRows, rowIndex, columnIndex);
    // DEBUG Print
    /*printf("Accessing ndarray with dimensions %zu/%zu: At row %zu and column %zu \n",
           ndArray.numRows, ndArray.numColumns, rowIndex, columnIndex);
    Complex value = ndArray.values[dataPosition];
    printf("Combined index is %zu, leading to value: (%f + %fi) \n", dataPosition, value.re, value.im);
    */

    return (OptComplex) {
        .value = ndArray.values[dataPosition],
        .valid = true
    };
}

RefSlice NDArray_getRefSlice(NDArray ndArray,
                             size_t numIndices,
                             size_t rowIndices[static numIndices],
                             size_t columnIndices[static numIndices]) {

    if (ndArray.values == NULL || ndArray.numColumns == 0 || ndArray.numRows == 0 ||
        numIndices == 0) {
        return (RefSlice) {0 };
    }

    // Allocate pointer vector in arena
    Complex **refPtr = arena_alloc(arena, (numIndices * sizeof(Complex *)));

    for (size_t i = 0; i < numIndices; i++) {
        size_t elementIndex = NDArray_index(ndArray.numRows, rowIndices[i], columnIndices[i]);
        refPtr[i] = &(ndArray.values[elementIndex]);
    }

    return (RefSlice) {
        .sliceRefs = refPtr,
        .sliceLength = numIndices
    };
}

Slice NDArray_getSlice(NDArray ndArray,
                       size_t numIndices,
                       size_t rowIndices[static numIndices],
                       size_t columnIndices[static numIndices]) {

   if (ndArray.values == NULL || ndArray.numColumns == 0 || ndArray.numRows == 0 ||
                numIndices == 0) {
            return (Slice) { 0 };
        }

    // Allocate pointer vector in arena
    Complex *valuePtr = arena_alloc(arena, (numIndices * sizeof(Complex)));

    for (size_t i = 0; i < numIndices; i++) {
        size_t elementIndex = NDArray_index(ndArray.numRows, rowIndices[i], columnIndices[i]);
        valuePtr[i] = (ndArray.values[elementIndex]);
    }

    return (Slice) {
        .sliceValues = valuePtr,
        .sliceLength = numIndices
    };
}


bool NDArray_setElement(NDArray ndArray, size_t rowIndex, size_t columnIndex, Complex newElement) {

    if (rowIndex > ndArray.numRows || columnIndex > ndArray.numColumns) {
        return false;
    }

    size_t dataPosition = NDArray_index(ndArray.numRows, rowIndex, columnIndex);
    ndArray.values[dataPosition] = newElement;

    return true;
}

void NDArray_crc(NDArray ndArray) {

    //uint32_t crcCode = 0;


    return;
}
