//
// Created by christoph on 24.08.24.
//

#ifndef LEIBNITZ_TUPLE_H
#define LEIBNITZ_TUPLE_H

#include <stdlib.h>
#include <complex/complex.h>

typedef struct {
    Complex *data;
    size_t numElements;
} ComplexTuple;

ComplexTuple complexTuple_new(size_t numElements);






#endif //LEIBNITZ_TUPLE_H
