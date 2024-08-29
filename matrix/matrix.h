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

Matrix matrix_fromArray(Arena *arena, Complex *values, size_t numRows, size_t numColumns);

Matrix permutation(Arena *arena, size_t dimension, size_t rowIndex1, size_t rowIndex2);

/**
 * Support
 */
Matrix matrixClone(Arena *arena, Matrix matrix);

/**
 * Linear Algebra
 */
Matrix matrix_addition(Arena *arena, Matrix matrix1, Matrix matrix2);

Matrix matrix_subtraction(Arena *arena, Matrix matrix1, Matrix matrix2);

Matrix matrix_multiplication(Arena *arena, Matrix matrix1, Matrix matrix2);

// TODO: Eigenvectors, eigenvalues

void matrix_powerINP(Matrix matrix, unsigned int exponent);

void matrix_scaleINP(Matrix matrix, Complex factor);

void matrix_sqrt(Matrix matrix);

void matrix_exp(Matrix matrix);

void matrix_sin(Matrix matrix);

void matrix_cos(Matrix matrix);

void matrix_transposeINP(Matrix matrix);

void matrix_inverseINP(Matrix matrix);






#endif
