#include <stdlib.h>
#include <stdbool.h>

#include "complex/complex.h"
#include "ndarray.h"

NDArray NDArray_create(size_t numColumns, size_t numRows, Complex *values) {
    return (NDArray) {};
}

NDArray NDArray_clone(NDArray ndArray) {
    return (NDArray) {};
}

void NDArray_resize(NDArray ndArray, size_t numRows, size_t numColumns) {
    return;
}

Complex NDArray_getElement(NDArray ndArray, size_t rowIndex, size_t columnIndex) {
    return (Complex) {};
}

bool NDArray_setElement(NDArray ndArray, size_t rowIndex, size_t columnIndex, Complex newElement) {
    return false;
}


