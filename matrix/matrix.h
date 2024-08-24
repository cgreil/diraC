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

Matrix matrix_zeros(Arena *arena, size_t numRows, size_t numColumns);

Matrix matrix_ones(Arena *arena, size_t numRows, size_t numColumns);

Matrix matrix_identity(Arena *arena, size_t dimension);

Matrix permutation(Arena *arena, size_t dimension, size_t rowIndex1, size_t rowIndex2);

/**
 * Support
 */
Matrix matrixClone(Arena *arena, Matrix matrix);

/**
 * Linear Algebra
 */
Matrix matrixAdd(Arena *arena, Matrix matrix1, Matrix matrix2);

Matrix matrixMulitply(Arena *arena, Matrix matrix1, Matrix matrix2);

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
