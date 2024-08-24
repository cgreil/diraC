#include <stdlib.h>
#include <assert.h>

#include "matrix/matrix.h"

Matrix matrix_zeros(Arena *arena, size_t numRows, size_t numColumns) {

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

Matrix matrix_ones(Arena *arena, size_t numRows, size_t numColumns) {

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

Matrix matrix_identity(Arena *arena, size_t dimension) {

    Matrix matrix;
    size_t matrixSize = dimension * dimension;

    Complex matrixData[matrixSize];
    Complex zero = { 0.0, 0.0 };
    Complex one = { 1.0, 0.0 };

    for (size_t i = 0; i < matrixSize; i++) {

        // diagonal elements have the property that i is a
        // multiple of dimension
        if (i % dimension == 0) {
            matrixData[i] = one;
        }
        else {
            matrixData[i] = zero;
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

Matrix permutation(Arena *arena, size_t dimension, size_t rowIndex1, size_t rowIndex2) {

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

    Matrix matrix = matrix_identity(arena, dimension);

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
Matrix matrixClone(Arena *arena, Matrix matrix) {

    return (Matrix) {
        .numRows = matrix.numRows,
        .numColumns = matrix.numColumns,
        .dataArray = NDArray_clone(arena, matrix.dataArray)
    };
}

/**
 * Linear Algebra
 */
Matrix matrixAdd(Arena *arena, Matrix matrix1, Matrix matrix2) {

    assert(matrix1.numRows == matrix2.numRows);
    assert(matrix1.numColumns == matrix2.numColumns);

    Matrix matrix;
    matrix_zeros(arena, matrix1.numRows, matrix1.numColumns);

    size_t matrixSize = matrix1.numRows * matrix1.numColumns;

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

Matrix matrixMulitply(Arena *arena, Matrix matrix1, Matrix matrix2) {

    assert(matrix1.numColumns == matrix2.numRows);

    Matrix matrix = matrix_zeros(arena, matrix1.numRows, matrix2.numColumns);




    return (Matrix) { 0 };
}

// TODO: Eigenvectors, eigenvalues

void matrixPower_INP(Matrix matrix, unsigned int exponent) {

}

void matrixScale_INP(Matrix matrix, Complex factor) {

}

void matrixSqrt(Matrix matrix) {

}

void matrixExp(Matrix matrix) {

}

void matrixSin(Matrix matrix) {

}

void matrixCos(Matrix matrix) {

}

void matrixTranspose_INP(Matrix matrix) {

}

void MatrixInverse_INP(Matrix matrix) {

}
