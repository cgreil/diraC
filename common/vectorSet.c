//
// Created by christoph on 16.08.24.
//

#include <string.h>
#include "common/doubleLinkedList.h"
#include "common/vectorSet.h"

static uint32_t calculateCRC(Vector vector) {
    return 0;
}

//static int compare(Vector vector1, Vector vector2);


static bool vectorsEqual (Vector vector1, Vector vector2) {
    if (calculateCRC(vector1) == calculateCRC(vector2)) {
        return true;
    } else {
        return false;
    }
}

VectorSet vectorSet_createEmptySet(void) {
    VectorSet vectorSet = { 0 };

    DoubleLinkedList* dll_ptr = calloc(1, sizeof(DoubleLinkedList));

    vectorSet.vectorList = dll_ptr;
    vectorSet.vectorComparison = vectorsEqual;

    return vectorSet;
}

VectorSet vectorSet_fromArray(Vector *vectors, size_t numVectors){

    VectorSet vectorSet = vectorSet_createEmptySet();

    for(size_t vectorIndex = 0; vectorIndex < numVectors; vectorIndex++) {
        vectorSet_addVector(vectorSet, vectors[vectorIndex]);
    }

    return vectorSet;
}

bool vectorSet_destroy(VectorSet vectorSet){

    size_t numDeleted = 0;
    while (vectorSet_size(vectorSet) > 0) {
        // we can simply keep removing the first entry until all
        // vectors have been removed
       if (vectorSet_removeVectorAtIndex(vectorSet, 0)) {
           numDeleted++;
       }
   }

    // free dll struct and afterwards also ptr to it
    dll_free(vectorSet.vectorList);
    free(vectorSet.vectorList);
    // invalidate memory
    memset(&vectorSet, 0, sizeof(VectorSet));

    return (vectorSet_size(vectorSet) == 0);
}

bool vectorSet_addVector(VectorSet vectorSet, Vector newVector) {

    // TODO: Replace by some iterator-like functionality
    for(size_t setIndex = 0; setIndex < vectorSet_size(vectorSet); setIndex++) {
        OptVector comparisonVector = vectorSet_getVectorAtIndex(vectorSet, setIndex);
        if (vectorSet.vectorComparison(comparisonVector.data, newVector) == false) {
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
    for (size_t setIndex = 0; setIndex < vectorSet_size(vectorSet); setIndex++) {
        OptVector comparisonVector = vectorSet_getVectorAtIndex(vectorSet, setIndex);
        if (vectorSet.vectorComparison(vector, comparisonVector.data)) {
            removedVector = dll_removeElementAtIndex(vectorSet.vectorList, setIndex);
            break;
        }
    }

    return removedVector;
}

bool vectorSet_removeVectorAtIndex(VectorSet vectorSet, size_t setIndex) {

    bool removedVector = dll_removeElementAtIndex(vectorSet.vectorList, setIndex);

    return removedVector;
}

size_t vectorSet_size(VectorSet vectorSet) {

    if (vectorSet.vectorList == NULL) {
        return 0;
    }

    return vectorSet.vectorList->numElements;
}

OptVector vectorSet_getVectorAtIndex(VectorSet vectorSet, size_t setIndex) {

    if (vectorSet_size(vectorSet) == 0 || setIndex >= vectorSet_size(vectorSet)) {
        return (OptVector) {
            .data = (Vector){ 0 },
            .isValid = false
        };
    }

    Vector *vectorRef = (Vector *) dll_getElementAtIndex(vectorSet.vectorList, setIndex);
    Vector vector;
    memcpy(&vector, vectorRef, sizeof(Vector));

    return (OptVector){
        .data = vector,
        .isValid = true
    };
}
