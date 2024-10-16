#include <stdlib.h>
#include <assert.h>

#include "matrix/matrix.h"

extern Arena *arena;

Matrix matrix_zeros(size_t numRows, size_t numColumns) {

    Matrix matrix;
    size_t matrixSize = numRows * numColumns;

    // Temporarily store data in a VLA
    Complex matrixData[matrixSize];
    Complex value = {0.0, 0.0};

    for (size_t i = 0; i < matrixSize; i++) {
        matrixData[i] = value;
    }

    NDArray ndarray = NDArray_create(arena, numRows, numColumns, matrixData);

    matrix = (Matrix) {
        .numRows = numRows,
        .numColumns = numColumns,
        .dataArray = ndarray
    };

    return matrix;
}

Matrix matrix_ones(size_t numRows, size_t numColumns) {

    Matrix matrix;
    size_t matrixSize = numRows * numColumns;

    Complex matrixData[matrixSize];
    Complex value = {1.0, 0.0};

    for (size_t i = 0; i < matrixSize; i++) {
        matrixData[i] = value;
    }

    NDArray ndarray = NDArray_create(arena, numRows, numColumns, matrixData);

    matrix = (Matrix) {
        .numRows = numRows,
        .numColumns = numColumns,
        .dataArray = ndarray
    };

    return matrix;
}

Matrix matrix_identity(size_t dimension) {

    Matrix matrix;
    size_t matrixSize = dimension * dimension;

    Complex matrixData[matrixSize];
    Complex zero = { 0.0, 0.0 };
    Complex one = { 1.0, 0.0 };

    size_t index = 0;
    for (size_t i = 0; i < dimension; i++) {
        for (size_t j = 0; j < dimension; j++) {
            if (i == j) {
                matrixData[index] = one;
            } else {
                matrixData[index] = zero;
            }
            index++;
        }
    }

    NDArray ndarray = NDArray_create(arena, dimension, dimension, matrixData);

    matrix = (Matrix) {
        .numRows = dimension,
        .numColumns = dimension,
        .dataArray = ndarray
    };

    return matrix;
}

Matrix matrix_fromArray(Complex *values, size_t numRows, size_t numColumns) {

    // TODO: Check whether num Rows * numColumns is actually a plausible matrix size
    Matrix matrix;

    // NDArray creation clones values, so ownership is not moved into the matrix
    NDArray matrixData = NDArray_create(arena, numRows, numColumns, values);

    matrix = (Matrix) {
        .numRows = numRows,
        .numColumns = numColumns,
        .dataArray = matrixData
    };

    return matrix;
}


Matrix matrix_permutation(size_t dimension, size_t rowIndex1, size_t rowIndex2) {

    /**
     * @param Arena
     * @param dimension
     * @param rowIndex1
     * @param rowIndex2
     *
     * @return Matrix
     *
     * @note Creates and returns a dimension x dimension permutation matrix P
     * where for any matrix A, PA == A*, where A* differs from
     * A only by having rows rowIndex1 and rowIndex2 swapped
     */

    Matrix matrix = matrix_identity(dimension);

    // Directly set the elements in the ndarray layout
    // In the initial identity element, swapped elements

    NDArray_setElement(matrix.dataArray, rowIndex1, rowIndex1, (Complex) { 0.0, 0.0 });
    NDArray_setElement(matrix.dataArray, rowIndex1, rowIndex2, (Complex) { 1.0, 0.0 });

    NDArray_setElement(matrix.dataArray, rowIndex2, rowIndex2, (Complex) { 0.0, 0.0 });
    NDArray_setElement(matrix.dataArray, rowIndex2, rowIndex1, (Complex) { 1.0, 0.0 });

    return matrix;
}

/**
 * Support
 */
Matrix matrix_clone(Matrix matrix) {

    return (Matrix) {
        .numRows = matrix.numRows,
        .numColumns = matrix.numColumns,
        .dataArray = NDArray_clone(arena, matrix.dataArray)
    };
}

/**
 * Linear Algebra
 */
Matrix matrix_addition(Matrix matrix1, Matrix matrix2) {

    assert(matrix1.numRows == matrix2.numRows);
    assert(matrix1.numColumns == matrix2.numColumns);

    Matrix matrix = matrix_zeros(matrix1.numRows, matrix1.numColumns);

    for (size_t i = 0; i < matrix1.numRows; i++) {
        for (size_t j = 0; j < matrix1.numColumns; j++) {

            OptComplex num1 = NDArray_getElement(matrix1.dataArray, i, j);
            OptComplex num2 = NDArray_getElement(matrix2.dataArray, i, j);

            assert(num1.valid);
            assert(num2.valid);

            Complex result = complex_addition(num1.value, num2.value);
            NDArray_setElement(matrix.dataArray, i, j, result);
        }
    }

    return matrix;
}

Matrix matrix_subtraction(Matrix matrix1, Matrix matrix2) {

    assert(matrix1.numRows == matrix2.numRows);
    assert(matrix1.numColumns == matrix2.numColumns);

    Matrix matrix = matrix_zeros(matrix1.numRows, matrix1.numColumns);

    for (size_t i = 0; i < matrix1.numRows; i++) {
        for (size_t j = 0; j < matrix1.numColumns; j++) {

            OptComplex num1 = NDArray_getElement(matrix1.dataArray, i, j);
            OptComplex num2 = NDArray_getElement(matrix2.dataArray, i, j);

            assert(num1.valid);
            assert(num2.valid);

            Complex result = complex_subtraction(num1.value, num2.value);
            NDArray_setElement(matrix.dataArray, i, j, result);
        }
    }

    return matrix;

}

Matrix matrix_multiplication(Matrix matrix1, Matrix matrix2) {

    /**
     *
     * Naive matrix multiplication algorithm
     */

    assert(matrix1.numColumns == matrix2.numRows);

    Matrix resultMatrix = matrix_zeros(matrix1.numRows, matrix2.numColumns);

    // iterate over rows of result resultMatrix
    for (size_t rowIndex = 0; rowIndex < resultMatrix.numRows; rowIndex++) {
        // iterate over columns of result resultMatrix
        for (size_t columnIndex = 0; columnIndex < resultMatrix.numColumns; columnIndex++) {

            Complex sum = { 0.0, 0.0 };
            // inner iteration for summation
            for (size_t sumIndex = 0; sumIndex < matrix1.numColumns; sumIndex++) {
                OptComplex factor1 = NDArray_getElement(matrix1.dataArray, rowIndex, sumIndex);
                OptComplex factor2 = NDArray_getElement(matrix2.dataArray, sumIndex, columnIndex);

                assert(factor1.valid);
                assert(factor2.valid);

                Complex num = complex_multiplication(factor1.value, factor2.value);
                complex_addition(sum, num);
            }

            NDArray_setElement(resultMatrix.dataArray, rowIndex, columnIndex, sum);
        }
    }

    return resultMatrix;
}

// TODO: Eigenvectors, eigenvalues

void matrix_powerINP(Matrix matrix, unsigned int exponent) {

}

void matrix_scaleINP(Matrix matrix, Complex factor) {

}

void matrix_sqrt(Matrix matrix) {

}

void matrix_exp(Matrix matrix) {

}

void matrix_sin(Matrix matrix) {

}

void matrix_cos(Matrix matrix) {

}

void matrix_transposeINP(Matrix matrix) {

}

void matrix_inverseINP(Matrix matrix) {

}
