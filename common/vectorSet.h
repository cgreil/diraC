//
// Created by christoph on 11.08.24.
//

#ifndef LEIBNITZ_VECTORSET_H
#define LEIBNITZ_VECTORSET_H

#include <stdlib.h>
#include <stdint.h>

#include "vector/vector.h"
#include "complex/complex.h"
#include "common/doubleLinkedList.h"

typedef struct {
    DoubleLinkedList* vectorList;
    bool (*vectorComparison) (Vector, Vector);
} VectorSet;


VectorSet vectorSet_createEmptySet();

VectorSet vectorSet_fromArray(Vector *vectors, size_t numVectors);

bool vectorSet_destroy(VectorSet vectorSet);

bool vectorSet_addVector(VectorSet vectorSet, Vector newVector);

bool vectorSet_removeVector(VectorSet vectorSet, Vector vector);

bool vectorSet_removeVectorAtIndex(VectorSet vectorSet, size_t setIndex);

size_t vectorSet_size(VectorSet vectorSet);

OptVector vectorSet_getVectorAtIndex(VectorSet vectorSet, size_t index);

VectorSet vectorSet_gramSchmidt(VectorSet vectorSet);

#endif //LEIBNITZ_VECTORSET_H
