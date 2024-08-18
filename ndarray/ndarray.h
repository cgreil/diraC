#ifndef NDARRAY_H_
#define NDARRAY_H_

#include <stdlib.h>
#include <stdint.h>

#include "complex/complex.h"
#include "utils/dataArena.h"

typedef struct {
    size_t numColumns;
    size_t numRows;
    Complex *values;
    u_int32_t checksum;
} NDArray;

NDArray NDArray_create(Arena *arena, size_t numRows, size_t numColumns, Complex *values);

NDArray NDArray_clone(Arena *arena, NDArray ndArray);

NDArray NDArray_resize(Arena *arena, NDArray ndArray, size_t numRows, size_t numColumns);

OptComplex NDArray_getElement(NDArray ndArray, size_t rowIndex, size_t columnIndex);

bool NDArray_setElement(NDArray ndArray, size_t rowIndex, size_t columnIndex, Complex newElement);



#endif
