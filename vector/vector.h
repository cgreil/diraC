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

Vector vector_zeros(Arena *arena, size_t vecSize);

Vector vector_ones(Arena *arena, size_t vecSize);

Vector vector_clone(Arena *arena, Vector vector);

Vector vector_fromArray(Arena *arena, Complex *complexArray, size_t vecSize);

OptComplex vector_getElement(Vector vector, size_t index);

OptVector vector_addition(Arena *arena, Vector vec1, Vector vec2);

OptComplex vector_innerProduct(Vector vec1, Vector vec2);

void vector_scaleINP(Vector vec, Complex factor);

void vector_conjugateINP(Vector vec);

void vector_transposeINP(Vector vec);

void vector_adjointINP(Vector vec);

void vector_resize(Arena *arena, Vector vec, size_t newLength);

bool vector_isColumn(Vector vec);






#endif