#include <stdlib.h>
#include <assert.h>
#include <vector/vectorCollection.h>
#include <memory.h>

#include "matrix/matrix.h"

extern Arena* arena;

Matrix matrix_zeros(size_t numRows, size_t numColumns) {

    Matrix matrix;
    size_t matrixSize = numRows * numColumns;

    // Temporarily store data in a VLA
    Complex matrixData[matrixSize];
    Complex value = {0.0, 0.0};

    for (size_t i = 0; i < matrixSize; i++) {
        matrixData[i] = value;
    }

    NDArray ndarray = NDArray_create(numRows, numColumns, matrixData);

    matrix = (Matrix) {
        .numRows = numRows,
        .numColumns = numColumns,
        .ndArray = ndarray
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

    NDArray ndarray = NDArray_create(numRows, numColumns, matrixData);

    matrix = (Matrix) {
        .numRows = numRows,
        .numColumns = numColumns,
        .ndArray = ndarray
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

    NDArray ndarray = NDArray_create(dimension, dimension, matrixData);

    matrix = (Matrix) {
        .numRows = dimension,
        .numColumns = dimension,
        .ndArray = ndarray
    };

    return matrix;
}

Matrix matrix_fromRowArray(Complex *values, size_t numRows, size_t numColumns) {

    // TODO: Check whether num Rows * numColumns is actually a plausible matrix size
    Matrix matrix;

    // NDArray creation clones values, so ownership is not moved into the matrix
    NDArray matrixData = NDArray_create(numRows, numColumns, values);

    matrix = (Matrix) {
        .numRows = numRows,
        .numColumns = numColumns,
        .ndArray = matrixData
    };

    return matrix;
}

Matrix matrix_fromColumnVectorSet(VectorCollection vectorSet) {

    // TODO: has to become VectorList
    if (vectorSet.vectors == NULL || vectorCollection_size(vectorSet) == 0) {
        return (Matrix) { 0 };
    }

    // Since vectors are allowed to be different sizes within vectorSets
    // the vector max size has to be determined and smaller vectors have to
    // be padded with 0s
    size_t numRows = 0;
    for(size_t setIndex = 0; setIndex < vectorCollection_size(vectorSet); setIndex++) {
        size_t vecSize = vectorCollection_getVectorAtIndex(vectorSet, setIndex).data.size;
        if (vecSize > numRows) {
            numRows = vecSize;
        }
    }

    Matrix resultMatrix = matrix_zeros(numRows, vectorCollection_size(vectorSet));

    for (size_t setIndex = 0; setIndex < vectorCollection_size(vectorSet); setIndex++) {
        Vector vector = vectorCollection_getVectorAtIndex(vectorSet, setIndex).data;

        if (vector.size < numRows) {
            // Resize vector
            vector = vector_resize(vector, vector.size);
        }

        matrix_setColumnAtIndex(resultMatrix, setIndex, vector);
    }

    return resultMatrix;
}

Matrix matrix_fromRowVectorSet(VectorCollection vectorSet) {

    // TODO: Has to become VectorList
    // TODO: Implement

    return (Matrix) { 0 };
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

    NDArray_setElement(matrix.ndArray, rowIndex1, rowIndex1, (Complex) {0.0, 0.0 });
    NDArray_setElement(matrix.ndArray, rowIndex1, rowIndex2, (Complex) {1.0, 0.0 });

    NDArray_setElement(matrix.ndArray, rowIndex2, rowIndex2, (Complex) {0.0, 0.0 });
    NDArray_setElement(matrix.ndArray, rowIndex2, rowIndex1, (Complex) {1.0, 0.0 });

    return matrix;
}

/**
 * Support
 */
Matrix matrix_clone(Matrix matrix) {

    return (Matrix) {
        .numRows = matrix.numRows,
        .numColumns = matrix.numColumns,
        .ndArray = NDArray_clone(matrix.ndArray)
    };
}

Complex matrix_getElement(Matrix matrix, size_t rowIndex, size_t columnIndex) {
    size_t valueIndex = columnIndex * matrix.numRows + rowIndex;
    return matrix.ndArray.values[valueIndex];
}

bool matrix_setElement(Matrix matrix, size_t rowIndex, size_t columnIndex, Complex newElement) {

    return NDArray_setElement(matrix.ndArray, rowIndex, columnIndex, newElement);
}

OptVector matrix_getColumnAtIndex(const Matrix matrix, const size_t columnIndex) {

    if (columnIndex >= matrix.numColumns) {
        return (OptVector) {
            .data = { 0 },
            .isValid = false
        };
    }

    // prepare indices arrays of <numRows> size which corresponds to
    // the number of elements within a single matrix column
    size_t rowIndices[matrix.numRows];
    size_t columnIndices[matrix.numRows];

    for (size_t i = 0; i < matrix.numRows; i++) {
        rowIndices[i] = i;
        columnIndices[i] = columnIndex;
    }

    Slice columnSlice = NDArray_getSlice(matrix.ndArray, matrix.numRows, rowIndices, columnIndices);

    return (OptVector) {
        .data = vector_fromArray(columnSlice.sliceValues, columnSlice.sliceLength),
        .isValid = true
    };
}

bool matrix_setColumnAtIndex(const Matrix matrix, const size_t columnIndex, const Vector vector) {

    size_t valueStartIndex = NDArray_index(matrix.ndArray.numRows, 0, columnIndex);
    // (numRows-1) corresponds to the max index an element of the column can ave
    size_t valueEndIndex = NDArray_index(matrix.ndArray.numRows, (matrix.ndArray.numRows - 1), columnIndex);

    assert(valueStartIndex <= valueEndIndex);
    size_t indexDiff = valueEndIndex - valueStartIndex;

    assert(vector.size == indexDiff);

    void *cpyRet = memcpy((matrix.ndArray.values) + valueStartIndex, vector.dataArray.values, indexDiff);

    return (cpyRet == NULL);

}

OptVector matrix_getRowAtIndex(const Matrix matrix, const size_t rowIndex) {

    if (rowIndex >= matrix.numRows) {
        return (OptVector) {
            .data = { 0 },
            .isValid = false
        };
    }

    // prepare indices arrays of <numRows> size which corresponds to
    // the number of elements within a single matrix column
    size_t numElements = matrix.numRows;
    size_t rowIndices[numElements];
    size_t columnIndices[numElements];

    for (size_t i = 0; i < numElements; i++) {
        rowIndices[i] = i;
        columnIndices[i] = rowIndex;
    }

    Slice columnSlice = NDArray_getSlice(matrix.ndArray, numElements, rowIndices, columnIndices);

    return (OptVector) {
        .data = vector_fromArray(columnSlice.sliceValues, columnSlice.sliceLength),
        .isValid = true
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

            OptComplex num1 = NDArray_getElement(matrix1.ndArray, i, j);
            OptComplex num2 = NDArray_getElement(matrix2.ndArray, i, j);

            assert(num1.valid);
            assert(num2.valid);

            Complex result = complex_addition(num1.value, num2.value);
            NDArray_setElement(matrix.ndArray, i, j, result);
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

            OptComplex num1 = NDArray_getElement(matrix1.ndArray, i, j);
            OptComplex num2 = NDArray_getElement(matrix2.ndArray, i, j);

            assert(num1.valid);
            assert(num2.valid);

            Complex result = complex_subtraction(num1.value, num2.value);
            NDArray_setElement(matrix.ndArray, i, j, result);
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
                OptComplex factor1 = NDArray_getElement(matrix1.ndArray, rowIndex, sumIndex);
                OptComplex factor2 = NDArray_getElement(matrix2.ndArray, sumIndex, columnIndex);

                assert(factor1.valid);
                assert(factor2.valid);

                Complex num = complex_multiplication(factor1.value, factor2.value);
                sum = complex_addition(sum, num);
            }

            NDArray_setElement(resultMatrix.ndArray, rowIndex, columnIndex, sum);
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

QRResult matrix_QRDecomposition(Matrix matrix) {


    if (matrix.numRows == 0 || matrix.numColumns == 0) {
        return (MatrixTuple) { 0 };
    }

    // QR Decomposition of a given matrix A, where
    // A = Q * R, where Q is unitary and R is an upper triangular matrix

    // Implementing QR-Factorization using the Gram-Schmidt process:
    // https://www.math.ucla.edu/~yanovsky/Teaching/Math151B/handouts/GramSchmidt.pdf

    VectorCollection columnList = vectorCollection_createEmpty(List);


    for (size_t columnIndex = 0; columnIndex < matrix.numColumns; columnIndex++) {
        Vector column = matrix_getColumnAtIndex(matrix, columnIndex).data;
        // TODO: This has to become vectors, as the same values may occur in multiple columns
        vectorCollection_addVector(columnList, column);
    }

    // Applying gram-schmidt procedure gives the columns which constitue Q matrix
    VectorCollection qMatrixColumns = vectorCollection_gramSchmidt(columnList);

    //Initialize result R-matrix
    // Since R will be upper traingular form, it will always be a square matrix
    Matrix rMatrix = matrix_zeros(matrix.numColumns, matrix.numColumns);


    // iterate over columns
    for (size_t columnIndex = 0; columnIndex < matrix.numColumns; columnIndex++) {

        // retrieve column a_i from A
        Vector columnVector = matrix_getColumnAtIndex(matrix, columnIndex).data;

        // iterate over rows up to column index
        for (size_t gramVectorIndex = 0; gramVectorIndex <= columnIndex; gramVectorIndex++) {
           Vector gramVector = vectorCollection_getVectorAtIndex(qMatrixColumns, gramVectorIndex).data;
           Complex rMatrixElement = vector_innerProduct(columnVector, gramVector).value;

            matrix_setElement(rMatrix, gramVectorIndex, columnIndex, rMatrixElement);
        }
    }

    Matrix qMatrix = matrix_fromColumnVectorSet(qMatrixColumns);

    return (QRResult) {
        .matrix1 = qMatrix,
        .matrix2 = rMatrix
    };
}
