#ifndef MATRIX_H_
#define MATRIX_H_

#include <stdlib.h>

#include "complex/tuple.h"
#include "complex/complex.h"
#include "ndarray/ndarray.h"


typedef struct Vector Vector;
typedef struct OptVector OptVector;
typedef struct VectorCollection VectorCollection;


typedef struct Matrix {
    size_t numRows;
    size_t numColumns;
    NDArray ndArray;
} Matrix;

typedef struct OptMatrix {
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
typedef MatrixTuple LUResult;


Matrix matrix_zeros(size_t numRows, size_t numColumns);

Matrix matrix_ones(size_t numRows, size_t numColumns);

Matrix matrix_fromRowArray(Complex *values, size_t numRows, size_t numColumns);

Matrix matrix_fromColumnVectors(VectorCollection vectorCollection);

Matrix matrix_fromRowVectors(VectorCollection vectorCollection);

Matrix matrix_identity(size_t dimension);

Matrix matrix_permutation(size_t dimension, size_t index1, size_t index2);

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

Matrix matrix_transpose(Matrix matrix);

Matrix matrix_conjugate(Matrix matrix);

Matrix matrix_adjoint(Matrix matrix);

Matrix matrix_diagonalize(Matrix matrix);

Matrix matrix_kron(Matrix matrix1, Matrix matrix2);

Complex matrix_determinant(Matrix matrix);

Complex matrix_trace(Matrix matrix);

bool matrix_isUpperTriangular(Matrix matrix);

bool matrix_isDiagonalizeable(Matrix matrix);

bool matrix_isDiagonal(Matrix matrix);

bool matrix_isSquare(Matrix matrix);

bool matrix_isNormal(Matrix matrix);

bool matrix_isHermitian(Matrix matrix);

bool matrix_isUnitary(Matrix matrix);

bool matrix_isEqual(Matrix matrix1, Matrix matrix2);

bool matrix_isZero(Matrix matrix);

// TODO: Eigenvectors, eigenvalues

ComplexTuple matrix_eigvalsQR(Matrix matrix);

void matrix_powerINP(Matrix matrix, unsigned int exponent);

void matrix_scaleINP(Matrix matrix, Complex factor);

void matrix_sqrt(Matrix matrix);

void matrix_exp(Matrix matrix);

void matrix_sin(Matrix matrix);

void matrix_cos(Matrix matrix);

void matrix_transposeINP(Matrix matrix);

void matrix_inverseINP(Matrix matrix);

QRResult matrix_QRDecomposition(Matrix matrix);

LUResult matrix_LUDecomposition(Matrix matrix);



#endif