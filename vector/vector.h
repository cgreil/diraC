#ifndef VECTOR_H_
#define VECTOR_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "complex/complex.h"
#include "ndarray/ndarray.h"

typedef struct Matrix Matrix;

typedef struct Vector {
    size_t size;
    NDArray dataArray;
} Vector;

typedef struct OptVector {
    Vector data;
    bool isValid;
}OptVector;


Vector vector_zeros(size_t vecSize);

Vector vector_ones(size_t vecSize);

Vector vector_clone(Vector vector);

Vector vector_fromArray(Complex *complexArray, size_t vecSize);

OptComplex vector_getElement(Vector vector, size_t index);

bool vector_setElement(Vector vector, size_t index, Complex newElement);

OptVector vector_addition(Vector vec1, Vector vec2);

OptVector vector_subtraction(Vector vec1, Vector vec2);

OptComplex vector_innerProduct(Vector vec1, Vector vec2);

Matrix vector_outerProduct(Vector vec1, Vector vec2);

Complex vector_norm(Vector vec);

Vector vector_scaleINP(Vector vec, Complex factor);

Vector vector_conjugateINP(Vector vec);

Vector vector_transposeINP(Vector vec);

Vector vector_adjointINP(Vector vec);

Vector vector_resize(Vector vec, size_t newLength);

Vector vector_normalize(Vector vec);

Vector vector_matrixMultiplication(Vector vector, Matrix matrix);

bool vector_equal(Vector vec1, Vector vec2);

bool vector_isZeroVector(Vector vec);

bool vector_isColumn(Vector vec);



#endif