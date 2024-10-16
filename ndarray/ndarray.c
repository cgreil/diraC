#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <utils/dataArena.h>
#include <memory.h>

#include "complex/complex.h"
#include "ndarray/ndarray.h"
#include "utils/dataArena.h"

NDArray NDArray_create(Arena *arena, size_t numRows, size_t numColumns, Complex *values) {

    /**
     * NDArray_create takes
     * @param arena
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

        size_t outputIndex = rowPos + numRows * colPos;
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

NDArray NDArray_clone(Arena *arena, NDArray ndArray) {

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

NDArray NDArray_resize(Arena *arena, NDArray ndArray, size_t numRows, size_t numColumns) {

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

    size_t dataPosition = columnIndex + rowIndex * ndArray.numColumns;

    return (OptComplex) {
        .value = ndArray.values[dataPosition],
        .valid = true
    };
}

bool NDArray_setElement(NDArray ndArray, size_t rowIndex, size_t columnIndex, Complex newElement) {

    if (rowIndex > ndArray.numRows || columnIndex > ndArray.numColumns) {
        return false;
    }

    size_t dataPosition = columnIndex + rowIndex * ndArray.numColumns;
    ndArray.values[dataPosition] = newElement;

    return true;
}

void NDArray_crc(NDArray ndArray) {

    //uint32_t crcCode = 0;


    return;


}