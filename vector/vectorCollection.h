//
// Created by christoph on 11.08.24.
//

#ifndef LEIBNITZ_VECTORCOLLECTION_H
#define LEIBNITZ_VECTORCOLLECTION_H

#include <stdlib.h>
#include <stdint.h>

#include "vector/vector.h"
#include "complex/complex.h"
#include "common/doubleLinkedList.h"


typedef enum {
    List,
    Set
}CollectionType;

typedef struct {
    DoubleLinkedList* vectors;
    CollectionType collectionType;
} VectorCollection;


VectorCollection vectorCollection_createEmpty(CollectionType collectionType);

VectorCollection vectorCollection_fromArray(Vector *vectors, size_t numVectors, CollectionType collectionType);

bool vectorCollection_destroy(VectorCollection vectorCollection);

bool vectorCollection_addVector(VectorCollection vectorCollection, Vector newVector);

bool vectorCollection_removeVector(VectorCollection vectorCollection, Vector vector);

bool vectorCollection_removeVectorAtIndex(VectorCollection vectorCollection, size_t index);

size_t vectorCollection_size(VectorCollection vectorSet);

OptVector vectorCollection_getVectorAtIndex(VectorCollection vectorCollection, size_t index);

VectorCollection vectorCollection_gramSchmidt(VectorCollection vectorCollection);




#endif //LEIBNITZ_VECTORCOLLECTION_H
