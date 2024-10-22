#ifndef VECTOR_H_
#define VECTOR_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "complex/complex.h"
#include "ndarray/ndarray.h"

struct VectorSet;

typedef struct {
    size_t size;
    NDArray dataArray;
} Vector;

typedef struct {
    Vector data;
    bool isValid;
}OptVector;

Vector vector_zeros(size_t vecSize);

Vector vector_ones(size_t vecSize);

Vector vector_clone(Vector vector);

Vector vector_fromArray(Complex *complexArray, size_t vecSize);

OptComplex vector_getElement(Vector vector, size_t index);

OptVector vector_addition(Vector vec1, Vector vec2);

OptComplex vector_innerProduct(Vector vec1, Vector vec2);

Complex vector_norm(Vector vec);

Vector vector_scaleINP(Vector vec, Complex factor);

Vector vector_conjugateINP(Vector vec);

Vector vector_transposeINP(Vector vec);

Vector vector_adjointINP(Vector vec);

Vector vector_resize(Arena *arena, Vector vec, size_t newLength);

Vector vector_normalize(Vector vec);

struct VectorSet vector_gramSchmidt();

bool vector_isColumn(Vector vec);



#endif