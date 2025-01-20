//
// Created by christoph on 24.08.24.
//

#ifndef TUPLE_H_
#define TUPLE_H_

#include <stdlib.h>
#include <complex/complex.h>

typedef struct {
    Complex *data;
    size_t numElements;
} ComplexTuple;

ComplexTuple complexTuple_new(size_t numElements);

#endif
