#ifndef NDARRAY_H_
#define NDARRAY_H_

#include <stdlib.h>
#include <stdint.h>

#include "complex/complex.h"


typedef struct {
    size_t numColumns;
    size_t numRows;
    Complex *values;
    u_int32_t checksum;
} NDArray;

NDArray NDArray_create(size_t numColumns, size_t numRows, Complex *values);

NDArray NDArray_clone(NDArray ndArray);

void NDArray_resize(NDArray ndArray, size_t numRows, size_t numColumns);

Complex NDArray_getElement(NDArray ndArray, size_t rowIndex, size_t columnIndex);

bool NDArray_setElement(NDArray ndArray, size_t rowIndex, size_t columnIndex, Complex newElement);



#endif
