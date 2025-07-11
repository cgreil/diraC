//
// Created by christoph on 16.08.24.
//

#include <memory.h>
#include <assert.h>
#include <stdio.h>

#include "complex/complex.h"
#include "vector/vector.h"
#include "ndarray/ndarray.h"
#include "common/string.h"
#include "logging/Logger.h"

#include "matrix/matrix.h"

/**
 *  FORWARD DECLARATION
 *
 */
extern Arena* arena;


Vector vector_zeros(size_t vecSize) {
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
    // put matrix_zeros into VLA
    Complex value = {0.0, 0.0};
    for (size_t i = 0; i < vecSize; i++) {
        complexValues[i] = value;
    }
    NDArray ndArray = NDArray_create(numRows, numColumns, complexValues);

    vector = (Vector) {
        .dataArray = ndArray,
        .size = vecSize
    };

    return vector;
}

Vector vector_ones(size_t vecSize) {
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
    // put matrix_zeros into VLA
    Complex value = {1.0, 0.0};
    for (size_t i = 0; i < vecSize; i++) {
        complexValues[i] = value;
    }
    NDArray ndArray = NDArray_create(numRows, numColumns, complexValues);

    vector = (Vector) {
        .dataArray = ndArray,
        .size = vecSize
    };

    return vector;
}

Vector vector_clone(Vector vector) {

    return (Vector) {
        .dataArray = NDArray_clone(vector.dataArray),
        .size = vector.size
    };
}

Vector vector_fromArray(Complex *complexArray, size_t vecSize) {

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

    NDArray ndArray = NDArray_create(numRows, numColumns, complexArray);

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

    size_t rowIndex;
    size_t columnIndex;
    // Distinguish between row and column vectors:
    assert(vector.dataArray.numRows == 1 || vector.dataArray.numColumns == 1);
    if (vector.dataArray.numRows == 1) {
        rowIndex = 0;
        columnIndex = index;
    } else {
        columnIndex = 0;
        rowIndex = index;
    }

    return NDArray_getElement(vector.dataArray, rowIndex, columnIndex);
}

OptVector vector_addition(Vector vec1, Vector vec2) {

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

        if (!complex1.valid || !complex2.valid) {
            return (OptVector ) {
                .data = (Vector) { 0 },
                .isValid = false
            };
        }

        result[i] = complex_addition(complex1.value, complex2.value);
    }

    Vector vector = vector_fromArray(result, vecSize);

    return (OptVector) {
        .data = vector,
        .isValid = true
    };
}

OptVector vector_subtraction(const Vector vec1, const Vector vec2) {

    if (vec1.size != vec2.size) {
        return (OptVector) {
            .data = (Vector){ 0 },
            .isValid = false
        };
    }

    size_t vecSize = vec1.size;
    Complex result[vecSize];

    for (size_t i = 0; i < vecSize; i++) {
        OptComplex complex1 = vector_getElement(vec1, i);
        OptComplex complex2 = vector_getElement(vec2, i);

        if (!complex1.valid || !complex2.valid) {
            return (OptVector) {
                .data = (Vector) { 0 },
                .isValid = false
            };
        }

        result[i] = complex_subtraction(complex1.value, complex2.value);
    }

    Vector vector = vector_fromArray(result, vecSize);

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

        if (!complex1.valid || !complex2.valid) {
            return (OptComplex) {
                .value = (Complex) { 0 },
                .valid = false
            };
        }

        result = complex_addition(result, complex_multiplication(complex1.value, complex_conjugate(complex2.value)));
    }

    return (OptComplex) {
        .value = result,
        .valid = true
    };
}

Matrix vector_outerProduct(Vector columnVector, Vector rowVector){

    Matrix resultMatrix = matrix_zeros(columnVector.size, rowVector.size);

    for (size_t rowIndex = 0; rowIndex < columnVector.size; rowIndex++) {
        for (size_t colIndex = 0; colIndex < rowVector.size; colIndex++) {
            Complex columnElement = vector_getElement(rowVector, colIndex).value;
            Complex rowElement = vector_getElement(columnVector, rowIndex).value;

            Complex result = complex_multiplication(complex_conjugate(rowElement), columnElement);
            matrix_setElement(resultMatrix, rowIndex, colIndex, result);
        }
    }

    return resultMatrix;
}

Complex vector_norm(Vector vec) {

    /**
     * Returns a norm of the given vector
     * as induced by the inner product
     * ||v|| = sqrt(<v,v>)
     *
     */

    OptComplex innerProd = vector_innerProduct(vec, vec);
    assert(innerProd.valid);

    Complex norm = (Complex) {
        .re = sqrt(innerProd.value.re),
        .im = sqrt(innerProd.value.im)
    };

    return norm;
}

Vector vector_scaleINP(Vector vec, Complex factor) {

    for (size_t i = 0; i < vec.size; ++i) {
        OptComplex complex = vector_getElement(vec, i);

        Complex newElement = complex_multiplication(complex.value, factor);
        vector_setElement(vec, i, newElement);
    }

    return vec;
}

Vector vector_conjugateINP(Vector vec) {


    for (size_t i = 0; i < vec.size; i++) {
        OptComplex complex = vector_getElement(vec, i);

        Complex newElement = {
            .re = complex.value.re,
            .im = - complex.value.im
        };

        vector_setElement(vec, i, newElement);
    }

    return vec;
}

Vector vector_transposeINP(Vector vec) {
    /**
     * @param Vector
     *
     * @note In the case of a vector, the alignment of values is the same
     * for column-and rowvectors => only the dimensions of the
     * underlying ndArray has to be changed, the data may be left alone
     */

    size_t temp = vec.dataArray.numColumns;
    vec.dataArray.numColumns = vec.dataArray.numRows;
    vec.dataArray.numRows = temp;

    return vec;
}

Vector vector_adjointINP(Vector vec) {

    Vector transposedVec = vector_transposeINP(vec);
    Vector conjugatedVec = vector_conjugateINP(transposedVec);

    return conjugatedVec;
}

Vector vector_resize(Vector vec, size_t newLength) {

    /**
     *  @note Function to resize a vector to the given newLength
     *  If newLength > currentLength, pad new Values with zeros
     *  If newLength < currentLength, excessive values will be removed
     */

    Complex *vecData = vec.dataArray.values;

    Vector newVector = vector_fromArray(vecData, newLength);

    // Pad with 0 if neccessary
    if (vec.size < newLength) {
        // memset with 0 since after creation, vector will be filled with garbage data
        memset((newVector.dataArray.values + vec.size), 0, (newLength - vec.size) * sizeof(Complex));
    }

    return newVector;
}

Vector vector_normalize(Vector vec) {

    OptComplex normalizationFactor = complex_division(
        (Complex) {1.0, 0.0},
        vector_norm(vec)
    );
    assert(normalizationFactor.valid);

    return vector_scaleINP(vec, normalizationFactor.value);
}


Vector vector_matrixMultiplication(Vector vector, Matrix matrix) {

    if (vector.size != matrix.numColumns) {
        return (Vector) { 0 };
    }

    Complex values[matrix.numRows];

    for (size_t i = 0; i < matrix.numRows; i++) {
        
        // intermediate result for a single mulitplication
        Complex result = { 0 };

        if (vector.size != matrix.numColumns) {
            LOG_ERROR(LOGOBJ("[vector_matrixMultiplication] Called vector matrix multiplication with noncompliant dimensions \n"));
        }

        for (size_t j = 0; j < vector.size; j++) {
            Complex lhs = matrix_getElement(matrix, i, j);
            Complex rhs = vector_getElement(vector, j).value;

            result = complex_addition(result, complex_multiplication(lhs, rhs));
        }
        values[i] = result;
    }

    Vector vec = vector_fromArray(values, matrix.numRows);
    return vec;
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

bool vector_equal(Vector vec1, Vector vec2) {

    if (vec1.size != vec2.size || vector_isColumn(vec1) != vector_isColumn(vec2)) {
        return false;
    }

    for (size_t vecIndex = 0; vecIndex < vec1.size; vecIndex++) {
        Complex c1 = vector_getElement(vec1, vecIndex).value;
        Complex c2 = vector_getElement(vec2, vecIndex).value;
        if (!complex_nearlyEqual(c1, c2)) {
            return false;
        }
    }

    return true;
}

bool vector_isZeroVector(Vector vec) {

    Vector zeroVec = vector_zeros(vec.size);

    return vector_equal(vec, zeroVec);
}


bool vector_setElement(Vector vec, size_t index, Complex newElement) {

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

bool vector_isNormalized(Vector vec) {

    Complex innerProd = vector_innerProduct(vec, vec).value;

    if (fabs(complex_modulus(innerProd) - 1.0) < 0.00001) {
        return true;
    } 
    return false;
}

