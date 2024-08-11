#ifndef VECTOR_H_
#define VECTOR_H_

#include <stdlib.h>
#include <stdint.h>

#include "complex/complex.h"
#include "ndarray/ndarray.h"

typedef struct {
    size_t size;
    NDArray dataArray;
} Vector;

Vector vectorZeros(size_t vecSize);

Vector vectorOnes(size_t vecSize);

Vector vectorFromArray(size_t vecSize, Complex *complexArray);

Complex vectorGetElement(Vector vector, size_t index);

bool vectorSetElement(Vector vector, size_t index, Complex newElement);

Vector vectorAddition(Vector vec1, Vector vec2);

Vector vectorInnerProduct(Vector vec1, Vector vec2);

void vectorScale(Vector vec1, Complex factor);

void vectorConjugate(Vector vec);

void vectorTranspose(Vector vec);

void vectorAdjoint(Vector vec);

void vectorResize(Vector vec, size_t newLength);

bool vectorIsColumn(Vector vec);







#endif