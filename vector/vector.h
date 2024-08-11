#ifndef VECTOR_H_
#define VECTOR_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "complex/complex.h"
#include "ndarray/ndarray.h"

typedef struct {
    size_t size;
    NDArray dataArray;
} Vector;

typedef struct {
    Vector data;
    bool isValid;
}OptVector;

Vector vector_Zeros(size_t vecSize);

Vector vector_Ones(size_t vecSize);

Vector vector_fromArray(size_t vecSize, Complex *complexArray);

Complex vector_getElement(Vector vector, size_t index);

bool vector_setElement(Vector vector, size_t index, Complex newElement);

OptVector vector_addition(Vector vec1, Vector vec2);

Complex vector_innerProduct(Vector vec1, Vector vec2);

void vector_scaleINP(Vector vec1, Complex factor);

void vector_conjugateINP(Vector vec);

void vector_transposeINP(Vector vec);

void vector_adjointINP(Vector vec);

void vector_resize(Vector vec, size_t newLength);

bool vector_isColumn(Vector vec);







#endif