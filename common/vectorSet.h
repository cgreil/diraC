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
    DoubleLinkedList vectorList;
    size_t numVectors;
    bool (*vectorComparison) (Vector, Vector);
}VectorSet;

static int compare(Vector vector1, Vector vector2);

VectorSet vectorSet_createEmptySet();

VectorSet vectorSet_fromArray(Vector *vectors, size_t numVectors);

bool vectorSet_destroy();

bool vectorSet_addVector(VectorSet vectorSet, Vector newVector);

bool vectorSet_removeVector(VectorSet vectorSet, Vector vector);

bool vectorSet_removeVectorAtIndex(VectorSet vectorSet, size_t setIndex);

OptVector vectorSet_getVectorAtIndex(VectorSet vectorSet, size_t index);









#endif //LEIBNITZ_VECTORSET_H
