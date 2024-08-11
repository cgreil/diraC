#ifndef MATRIX_H_
#define MATRIX_H_

#include <stdlib.h>

#include "complex/complex.h"
#include "ndarray/ndarray.h"

typedef struct {
    size_t numRows;
    size_t numColumns;
    NDArray dataArray;  
} Matrix;

typedef struct {
    Matrix matrix;
    bool isValid;
} OptMatrix;

Matrix zeros(size_t numRows, size_t numColumns);

Matrix ones(size_t numRows, size_t numColumns);

Matrix identity(size_t dimension);

Matrix permutation(size_t dimension, size_t rowIndex1, size_t rowIndex2);

/**
 * Support
 */
Matrix matrixClone(Matrix matrix);

/**
 * Linear Algebra
 */
Matrix matrixAdd(Matrix matrix1, Matrix matrix2);

Matrix matrixMulitply(Matrix matrix1, Matrix matrix2);

// TODO: Eigenvectors, eigenvalues

void matrixPower_INP(Matrix matrix, unsigned int exponent);

void matrixScale_INP(Matrix matrix, Complex factor);

void matrixSqrt(Matrix matrix);

void matrixExp(Matrix matrix);

void matrixSin(Matrix matrix);

void matrixCos(Matrix matrix);

void matrixTranspose_INP(Matrix matrix);

void MatrixInverse_INP(Matrix matrix);






#endif