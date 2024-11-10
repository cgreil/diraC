#ifndef NDARRAY_H_
#define NDARRAY_H_

#include <stdlib.h>
#include <stdint.h>

#include "complex/complex.h"
#include "utils/dataArena.h"

// Datatype to store references to slices of data within the ndarray
typedef struct {
    // pointer to pointer is needed to store an array of
    // pointers to values. Otherwise, only slices of  contiguous
    // memory regions would be possible
    Complex** sliceRefs;
    size_t sliceLength;
}RefSlice;

typedef struct {
    // Slice data structure holding a copy of the elements
    Complex *sliceValues;
    size_t sliceLength;
}Slice;

#define CRC_POLYNOM 0x12345678

typedef struct {
    size_t numColumns;
    size_t numRows;
    Complex *values;
    u_int32_t checksum;
} NDArray;

NDArray NDArray_create(size_t numRows, size_t numColumns, Complex *values);

NDArray NDArray_clone(NDArray ndArray);

NDArray NDArray_resize(NDArray ndArray, size_t numRows, size_t numColumns);

OptComplex NDArray_getElement(NDArray ndArray, size_t rowIndex, size_t columnIndex);

RefSlice NDArray_getRefSlice(NDArray ndArray, size_t numIndices, size_t rowIndices[static numIndices],
                             size_t columnIndices[static numIndices]);

Slice NDArray_getSlice(NDArray ndArray, size_t numIndices, size_t rowIndices[static numIndices],
                       size_t columnIndices[static numIndices]);

bool NDArray_setElement(NDArray ndArray, size_t rowIndex, size_t columnIndex, Complex newElement);

void NDArray_crc(NDArray ndArray);

size_t NDArray_index(size_t numArrayRows, size_t rowIndex, size_t columnIndex);


#endif
