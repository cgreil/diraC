//
// Created by christoph on 16.08.24.
//

#include <string.h>
#include "common/doubleLinkedList.h"
#include "common/vectorSet.h"

static uint32_t calculateCRC(Vector vector) {
    return 0;
}

static bool vectorsEqual (Vector vector1, Vector vector2) {
    if (calculateCRC(vector1) == calculateCRC(vector2)) {
        return true;
    } else {
        return false;
    }
}

VectorSet vectorSet_createEmptySet() {
    VectorSet vectorSet = {};

    vectorSet.vectorList = dll_create();
    vectorSet.vectorComparison = vectorsEqual;

    return vectorSet;
}

VectorSet vectorSet_fromArray(Vector *vectors, size_t numVectors){

    VectorSet vectorSet = vectorSet_createEmptySet();

    for(size_t vectorIndex = 0; vectorIndex < numVectors; vectorIndex++) {
        vectorSet_addVector(vectorSet, vectors[vectorIndex]);
    }

    vectorSet.numVectors = numVectors;

    return vectorSet;
}

bool vectorSet_destroy(VectorSet vectorSet){

    size_t numDeleted = 0;
    for (size_t setIndex = 0; setIndex < vectorSet.numVectors; setIndex++) {
       if (vectorSet_removeVectorAtIndex(vectorSet, setIndex)) {
           numDeleted++;
       }
   }

    // invalidate memory
    memset(&vectorSet, 0, sizeof(VectorSet));

    return numDeleted == vectorSet.numVectors;
}

bool vectorSet_addVector(VectorSet vectorSet, Vector newVector) {

    // TODO: Replace by some iterator-like functionality
    for(size_t setIndex = 0; setIndex < vectorSet.numVectors; setIndex++) {
        Vector comparisonVector = vectorSet_getVectorAtIndex(vectorSet, setIndex);
        if (vectorSet.vectorComparison(comparisonVector, newVector) == false) {
            return false;
        }
    }

    // If execution reaches this point, vector is not yet in set
    dll_insertElement(vectorSet.vectorList, (void *) &newVector, sizeof(Vector));

    return true;
}

bool vectorSet_removeVector(VectorSet vectorSet, Vector vector) {

    bool removedVector = false;

    //TODO: REPLACE by some iterator-like functionality
    for (size_t setIndex = 0; setIndex < vectorSet.numVectors; setIndex++) {
        Vector comparisonVector = vectorSet_getVectorAtIndex(vectorSet, setIndex);
        if (vectorSet.vectorComparison(vector, comparisonVector)) {
            dll_removeElementAtIndex(vectorSet.vectorList, setIndex);
            removedVector = true;
            vectorSet.numVectors--;
            break;
        }
    }

    return removedVector;
}

bool vectorSet_removeVectorAtIndex(VectorSet vectorSet, size_t setIndex) {

    bool removedVector = dll_removeElementAtIndex(vectorSet.vectorList, setIndex);
    vectorSet.numVectors--;

    return removedVector;
}

OptVector vectorSet_getVectorAtIndex(VectorSet vectorSet, size_t setIndex) {

    if (vectorSet.numVectors == 0 || setIndex >= vectorSet.numVectors) {
        return (OptVector) {
            .data = (Vector){},
            .isValid = false
        };
    }

    Vector *vectorRef = (Vector *) dll_getElementAtIndex(vectorSet.vectorList, setIndex);
    Vector vector;
    memcpy(&vector, vectorRef, sizeof(Vector));

    return (OptVector){
        .data = (Vector) vector,
        .isValid = true
    };
}
