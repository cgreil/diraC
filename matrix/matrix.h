#ifndef MATRIX_H_
#define MATRIX_H_

#include <stdlib.h>
#include <vector/vector.h>
#include <vector/vectorCollection.h>

#include "complex/complex.h"
#include "ndarray/ndarray.h"

typedef struct {
    size_t numRows;
    size_t numColumns;
    NDArray ndArray;
} Matrix;

typedef struct {
    Matrix matrix;
    bool isValid;
} OptMatrix;

// Helper Object to store two matrices
// as needed in QR decomposition
typedef struct {
    Matrix matrix1;
    Matrix matrix2;
} MatrixTuple;

typedef MatrixTuple QRResult;


Matrix matrix_zeros(size_t numRows, size_t numColumns);

Matrix matrix_ones(size_t numRows, size_t numColumns);

Matrix matrix_fromRowArray(Complex *values, size_t numRows, size_t numColumns);

Matrix matrix_fromColumnVectorSet(VectorCollection vectorSet);

Matrix matrix_fromRowVectorSet(VectorCollection vectorSet);

Matrix matrix_identity(size_t dimension);

Matrix matrix_permutation(size_t dimension, size_t rowIndex1, size_t rowIndex2);

/**
 * Support
 */
Matrix matrix_clone(Matrix matrix);

Complex matrix_getElement(Matrix matrix, size_t rowIndex, size_t columnIndex);

bool matrix_setElement(Matrix matrix, size_t rowIndex, size_t columnIndex, Complex newElement);

OptVector matrix_getColumnAtIndex(Matrix matrix, size_t columnIndex);

bool matrix_setColumnAtIndex(Matrix matrix, size_t columnIndex, Vector vector);

OptVector matrix_getRowAtIndex(Matrix matrix, size_t rowIndex);

 /* Linear Algebra
 */
Matrix matrix_addition(Matrix matrix1, Matrix matrix2);

Matrix matrix_subtraction(Matrix matrix1, Matrix matrix2);

Matrix matrix_multiplication(Matrix matrix1, Matrix matrix2);

bool matrix_isUpperTriangular(Matrix matrix);

bool matrix_isDiagonal(Matrix matrix);

bool matrix_isSquare(Matrix matrix);

bool matrix_isNormal(Matrix matrix);

bool matrix_isHermitian(Matrix matrix);

bool matrix_isUnitary(Matrix matrix);

// TODO: Eigenvectors, eigenvalues

void matrix_powerINP(Matrix matrix, unsigned int exponent);

void matrix_scaleINP(Matrix matrix, Complex factor);

void matrix_sqrt(Matrix matrix);

void matrix_exp(Matrix matrix);

void matrix_sin(Matrix matrix);

void matrix_cos(Matrix matrix);

void matrix_transposeINP(Matrix matrix);

void matrix_inverseINP(Matrix matrix);

QRResult matrix_QRDecomposition(Matrix matrix);



#endif