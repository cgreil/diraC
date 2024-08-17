//
// Created by christoph on 16.08.24.
//

#include <memory.h>
#include <assert.h>
#include "vector/vector.h"
#include "ndarray/ndarray.h"

/**
 *  FORWARD DECLARATION
 *
 */

static bool vector_setElement(Vector vec, size_t index, Complex newElement);


Vector vector_zeros(Arena *arena, size_t vecSize) {
    /**
     * @param arena
     * @param vecSize
     *
     * @return Vector
     *
     * @note Creates a column vector of size vecSize,
     * containing 0 in all elements
     */

    Vector vector;
    size_t numRows = vecSize;
    size_t numColumns = 1;

    // Temporarily store data in a VLA
    Complex complexValues[vecSize];
    // put zeros into VLA
    Complex value = {0.0, 0.0};
    for (size_t i = 0; i < vecSize; i++) {
        complexValues[i] = value;
    }
    NDArray ndArray = NDArray_create(arena, numRows, numColumns, complexValues);

    vector = (Vector) {
        .dataArray = ndArray,
        .size = vecSize
    };

    return vector;
}

Vector vector_ones(Arena *arena, size_t vecSize) {
    /**
    * @param arena
    * @param vecSize
    *
    * @return Vector
    *
    * @note Creates a column vector of size vecSize,
    * containing 0 in all elements
    */

    Vector vector;
    size_t numRows = vecSize;
    size_t numColumns = 1;

    // Temporarily store data in a VLA
    Complex complexValues[vecSize];
    // put zeros into VLA
    Complex value = {1.0, 0.0};
    for (size_t i = 0; i < vecSize; i++) {
        complexValues[i] = value;
    }
    NDArray ndArray = NDArray_create(arena, numRows, numColumns, complexValues);

    vector = (Vector) {
        .dataArray = ndArray,
        .size = vecSize
    };

    return vector;
}

Vector vector_clone(Arena *arena, Vector vector) {

    return (Vector) {
        .dataArray = NDArray_clone(arena, vector.dataArray),
        .size = vector.size
    };
}

Vector vector_fromArray(Arena *arena, Complex *complexArray, size_t vecSize) {

    /**
     * @param arena
     * @param complexArray
     * @param vecSize
     *
     * @note Creates a column vector of size vecSize containing (cloned)
     * values of complexArray,
     */

    size_t numRows = vecSize;
    size_t numColumns = 1;

    NDArray ndArray = NDArray_create(arena, numRows, numColumns, complexArray);

    return (Vector) {
        .dataArray = ndArray,
        .size = vecSize
    };
}

OptComplex vector_getElement(Vector vector, size_t index){

    /**
     * @param vector
     * @param index
     *
     * @return Complex
     *
     * @Note returns a copy of the element at the (zero-based)
     * index within the supplied vector
     */

    size_t rowIndex = index;
    size_t columnIndex = 0;
    return NDArray_getElement(vector.dataArray, rowIndex, columnIndex);
}

OptVector vector_addition(Arena *arena, Vector vec1, Vector vec2) {

    if (vec1.size != vec2.size) {
        return (OptVector) {
            .data = (Vector) { 0 },
            .isValid = false
        };
    }

    size_t vecSize = vec1.size;
    Complex result[vecSize];


    for(size_t i = 0; i < vecSize; i++) {
        OptComplex complex1 = vector_getElement(vec1, i);
        OptComplex complex2 = vector_getElement(vec2, i);

        if (!complex1.valid || complex2.valid) {
            return (OptVector ) {
                .data = (Vector) { 0 },
                .isValid = false
            };
        }

        result[i] = complex_addition(complex1.value, complex2.value);
    }

    Vector vector = vector_fromArray(arena, result, vecSize);

    return (OptVector) {
        .data = vector,
        .isValid = true
    };
}

OptComplex vector_innerProduct(Vector vec1, Vector vec2) {

    if (vec1.size != vec2.size) {
        return (OptComplex) {
            .value = (Complex)  { 0 },
            .valid = false
        };
    }

    size_t vecSize = vec1.size;
    Complex result = { 0 };

    for (size_t i = 0; i < vecSize; i++) {
        OptComplex complex1 = vector_getElement(vec1, i);
        OptComplex complex2 = vector_getElement(vec2, i);

        if (!complex1.valid || complex2.valid) {
            return (OptComplex) {
                .value = (Complex) { 0 },
                .valid = false
            };
        }

        // sum up all terms
        complex_addition(result, complex_multiplication(complex1.value, complex2.value));
    }

    return (OptComplex) {
        .value = result,
        .valid = true
    };
}

void vector_scaleINP(Vector vec, Complex factor) {

    for (size_t i = 0; i < vec.size; ++i) {
        OptComplex complex = vector_getElement(vec, i);

        Complex newElement = complex_multiplication(complex.value, factor);
        vector_setElement(vec, i, newElement);
    }
}

void vector_conjugateINP(Vector vec) {

    for (size_t i = 0; i < vec.size; i++) {
        OptComplex complex = vector_getElement(vec, i);

        Complex newElement = {
            .re = complex.value.re,
            .im = - complex.value.im
        };

        vector_setElement(vec, i, newElement);
    }
}

void vector_transposeINP(Vector vec) {
    //TODO
    return;
}

void vector_adjointINP(Vector vec) {
    vector_transposeINP(vec);
    vector_conjugateINP(vec);
}

void vector_resize(Arena *arena, Vector vec, size_t newLength) {

}

bool vector_isColumn(Vector vec) {
    //assert that the given vector is atleast either column or row vec
    assert((vec.dataArray.numColumns == 1) || (vec.dataArray.numRows == 1));

    if (vec.dataArray.numColumns == 1) {
        return true;
    }
    else {
        return false;
    }
}

static bool vector_setElement(Vector vec, size_t index, Complex newElement) {

    if (index >= vec.size) {
        return false;
    }

    if (vector_isColumn(vec)) {
        NDArray_setElement(vec.dataArray, index, 0, newElement);
    }
    else {
        NDArray_setElement(vec.dataArray, 0, index, newElement);
    }

    return true;
}
