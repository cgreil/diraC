//
// Created by christoph on 16.08.24.
//

#include <string.h>
#include <assert.h>

#include "common/doubleLinkedList.h"
#include "vectorCollection.h"

/* SUPPRESS Unused func
static uint32_t calculateCRC(Vector vector) {
    return 0;
}
*/

// static int compare(Vector vector1, Vector vector2);

static bool vectorsEqual (Vector vector1, Vector vector2) {
    // TODO: Implement CRC
    //return calculateCRC(vector1) == calculateCRC(vector2);

    // For now, iterate over elements for comparison
    if (vector1.size != vector2.size) {
        return false;
    }

    for (size_t i = 0; i < vector1.size; i++) {
        OptComplex complex1 = vector_getElement(vector1, i);
        OptComplex complex2 = vector_getElement(vector2, i);

        if (!complex_nearlyEqual(complex1.value, complex2.value)) {
            return false;
        }
    }
    return true;
}

VectorCollection vectorCollection_createEmpty(CollectionType collectionType) {
    VectorCollection vectorCollection = { 0 };

    DoubleLinkedList* dll_ptr = calloc(1, sizeof(DoubleLinkedList));

    vectorCollection.vectors = dll_ptr;
    vectorCollection.collectionType = List;

    return vectorCollection;
}

VectorCollection vectorCollection_fromArray(Vector *vectors, size_t numVectors, CollectionType collectionType) {

    VectorCollection vectorCollection = vectorCollection_createEmpty(collectionType);

    for(size_t vectorIndex = 0; vectorIndex < numVectors; vectorIndex++) {
        vectorCollection_addVector(vectorCollection, vectors[vectorIndex]);
    }

    return vectorCollection;
}

bool vectorCollection_destroy(VectorCollection vectorCollection){

    while (vectorCollection_size(vectorCollection) > 0) {
        // we can simply keep removing the first entry until all
        // vectors have been removed
        vectorCollection_removeVectorAtIndex(vectorCollection, 0);
   }

    // free dll struct and afterward also ptr to it
    dll_free(vectorCollection.vectors);
    free(vectorCollection.vectors);
    // invalidate memory
    memset(&vectorCollection, 0, sizeof(VectorCollection));

    return true;
}

bool vectorCollection_addVector(VectorCollection vectorCollection, Vector newVector) {

    switch (vectorCollection.collectionType) {
        case List: {
            dll_insertElement(vectorCollection.vectors, (void *) &newVector, sizeof(Vector));
        }
        break;

        case Set: {
            for(size_t setIndex = 0; setIndex < vectorCollection_size(vectorCollection); setIndex++) {
                OptVector comparisonVector = vectorCollection_getVectorAtIndex(vectorCollection, setIndex);
                if (vectorsEqual(comparisonVector.data, newVector)) {
                    // An equivalent vector is already in the set
                    return true;
                }
            }
            dll_insertElement(vectorCollection.vectors, (void *) &newVector, sizeof(Vector));
        }
        break;

        default:
            // Not yet implemented
            break;
    }
    return false;
}

bool vectorCollection_removeVector(VectorCollection vectorCollection, Vector vector) {

    bool removedVector = false;

    for (size_t collectionIndex = 0; collectionIndex < vectorCollection_size(vectorCollection); collectionIndex++) {
        OptVector comparisonVector = vectorCollection_getVectorAtIndex(vectorCollection, collectionIndex);
        if (vectorsEqual(vector, comparisonVector.data)) {
            removedVector = dll_removeElementAtIndex(vectorCollection.vectors, collectionIndex);
            break;
        }
    }

    return removedVector;
}

bool vectorCollection_removeVectorAtIndex(VectorCollection vectorCollection, size_t index) {

    bool removedVector = dll_removeElementAtIndex(vectorCollection.vectors, index);

    return removedVector;
}

size_t vectorCollection_size(VectorCollection vectorSet) {

    if (vectorSet.vectors == NULL) {
        return 0;
    }
    return vectorSet.vectors->numElements;
}

OptVector vectorCollection_getVectorAtIndex(VectorCollection vectorCollection, size_t index) {

    if (vectorCollection_size(vectorCollection) == 0 || index >= vectorCollection_size(vectorCollection)) {
        return (OptVector) {
            .data = (Vector){ 0 },
            .isValid = false
        };
    }

    Vector *vectorRef = (Vector *) dll_getElementAtIndex(vectorCollection.vectors, index);
    Vector vector;
    memcpy(&vector, vectorRef, sizeof(Vector));

    return (OptVector){
        .data = vector,
        .isValid = true
    };
}

VectorCollection vectorCollection_gramSchmidt(VectorCollection vectorCollection) {

    if (vectorCollection_size(vectorCollection) == 0) {
        return (VectorCollection) {0 };
    }

    VectorCollection onbSet = vectorCollection_createEmpty(List);

    for (int vecIndex = 0; vecIndex < vectorCollection_size(vectorCollection); vecIndex++) {

        // iterate over all vectors from initial collection
        Vector vector = vectorCollection_getVectorAtIndex(vectorCollection, vecIndex).data;

        // calculate projection on all vectors in onbSet and subtract this projection
        for (int onbIndex = 0; onbIndex < vectorCollection_size(onbSet); onbIndex++) {
            Vector onbVec = vectorCollection_getVectorAtIndex(vectorCollection, onbIndex).data;
            Complex projection = vector_innerProduct(vector, onbVec).value;
            Vector projectedVector = vector_scaleINP(onbVec, projection);

            // is projection nonzero?
            if (vector_isZeroVector(projectedVector)) {
                continue;
            }
            vector = vector_subtraction(vector, projectedVector).data;
        }
        // check if vector is 0-vector (i.e. initial vector was linearCombination of onbSet-vecs
        // if yes, proceed with next vector
        if (vector_isZeroVector(vector)) {
            continue;
        }
        // normalize remaining vector
        Vector normalizedVec = vector_normalize(vector);
        // add to onbSet
        vectorCollection_addVector(onbSet, normalizedVec);
    }

    return onbSet;
}
