#include <stdlib.h>
#include <assert.h>
#include <memory.h>

#include "matrix/matrix.h"
#include "complex/tuple.h"
#include "vector/vector.h"
#include "vector/vectorCollection.h"


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

Matrix matrix_fromColumnVectors(VectorCollection vectorCollection) {

    if (vectorCollection.vectors == NULL || vectorCollection_size(vectorCollection) == 0) {
        return (Matrix) { 0 };
    }

    // Since vectors are allowed to be different sizes within vectorSets
    // the vector max size has to be determined and smaller vectors have to
    // be padded with 0s
    size_t numRows = 0;
    for(size_t setIndex = 0; setIndex < vectorCollection_size(vectorCollection); setIndex++) {
        size_t vecSize = vectorCollection_getVectorAtIndex(vectorCollection, setIndex).data.size;
        if (vecSize > numRows) {
            numRows = vecSize;
        }
    }

    Matrix resultMatrix = matrix_zeros(numRows, vectorCollection_size(vectorCollection));

    for (size_t setIndex = 0; setIndex < vectorCollection_size(vectorCollection); setIndex++) {
        Vector vector = vectorCollection_getVectorAtIndex(vectorCollection, setIndex).data;

        if (vector.size < numRows) {
            // Resize vector
            vector = vector_resize(vector, vector.size);
        }

        matrix_setColumnAtIndex(resultMatrix, setIndex, vector);
    }

    return resultMatrix;
}

Matrix matrix_fromRowVectors(VectorCollection vectorCollection) {

    if (vectorCollection_size(vectorCollection) == 0) {
        return (Matrix) { 0 };
    }
    // unroll row collection into big array and utilize
    // matrix_fromRowArray
    size_t numRows = vectorCollection_size(vectorCollection);
    // retrive number of columns from the size of the first row vector
    Vector firstVector = vectorCollection_getVectorAtIndex(vectorCollection, 0).data;
    size_t numColumns = firstVector.size;

    size_t arraySize = numRows * numColumns;
    // create VLA for all complex values
    Complex array[arraySize];
    // add all values into VLA
    for (size_t vectorIdx = 0; vectorIdx < numRows; vectorIdx++) {
        Vector row = vectorCollection_getVectorAtIndex(vectorCollection, vectorIdx).data;
        memcpy(array+(vectorIdx * numColumns), row.dataArray.values, numColumns * sizeof(Complex));
    }
    Matrix matrix = matrix_fromRowArray(array, numRows, numColumns);
    return matrix;
}


Matrix matrix_permutation(size_t dimension, size_t index1, size_t index2) {

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
     * A only by having rows index1 and index2 swapped
     *
     * And for any matrix A, AP = A', where A' differs from A only by having
     * columns index1 and index2 swapped
     *
     */

    Matrix matrix = matrix_identity(dimension);

    // Directly set the elements in the ndarray layout
    // In the initial identity element, swapped elements

    NDArray_setElement(matrix.ndArray, index1, index1, (Complex) {0.0, 0.0 });
    NDArray_setElement(matrix.ndArray, index1, index2, (Complex) {1.0, 0.0 });

    NDArray_setElement(matrix.ndArray, index2, index2, (Complex) {0.0, 0.0 });
    NDArray_setElement(matrix.ndArray, index2, index1, (Complex) {1.0, 0.0 });

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
    // incement by 1 since the number of elements is one more than the actual difference of indices
    size_t indexDiff = valueEndIndex - valueStartIndex + 1;
    assert(vector.size == indexDiff);

    void *cpyRet = memcpy((matrix.ndArray.values) + valueStartIndex, vector.dataArray.values, indexDiff * sizeof(Complex));

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


Matrix matrix_transpose(Matrix matrix) {

    if (matrix.numRows == 0 || matrix.numColumns == 0) {
        return (Matrix) { 0 };
    }

    Matrix resultMatrix = matrix_zeros(matrix.numRows, matrix.numRows);

    for (size_t rowIndex = 0; rowIndex < matrix.numRows; rowIndex++) {
        for (size_t columnIndex = 0; columnIndex < matrix.numColumns; columnIndex++) {
            matrix_setElement(resultMatrix, rowIndex, columnIndex, matrix_getElement(matrix, columnIndex, rowIndex));
        }
    }

    return resultMatrix;
}

Matrix matrix_conjugate(Matrix matrix) {

    Matrix resultMatrix = matrix_zeros(matrix.numRows, matrix.numColumns);
    for (size_t rowIndex = 0; rowIndex < matrix.numRows; rowIndex++) {
        for (size_t colIndex = 0; colIndex < matrix.numColumns; colIndex++) {
            Complex conjugatedValue = complex_conjugate(matrix_getElement(matrix, rowIndex, colIndex));
            matrix_setElement(resultMatrix, rowIndex, colIndex, conjugatedValue);
        }
    }
    return resultMatrix;
}

Matrix matrix_adjoint(Matrix matrix) {
    return matrix_transpose(matrix_conjugate(matrix));
}

Matrix matrix_diagonalize(Matrix matrix) {
    return (Matrix) { 0 };
}

Complex matrix_determinant(Matrix matrix) {

    // Determinant is only defined for square matrices:
    if (matrix.numRows != matrix.numColumns) {
        return (Complex) {0.0, 0.0};
    }

    // in LU-decomposed form A = LU, the determinant can be
    // easily calculated det(A) = det(L) * det(U)
    // and both det(L) and det(U) are simply the product
    // of the diagonal entries

    LUResult luResult = matrix_LUDecomposition(matrix);

    // check that both L and U components have the same dimensions
    assert(luResult.matrix1.numRows == luResult.matrix2.numRows);
    assert(luResult.matrix1.numColumns == luResult.matrix2.numColumns);

    size_t numElements = luResult.matrix1.numRows;

    Complex prod = (Complex) { 1.0, 0.0 };
    for (size_t i = 0; i < numElements; i++) {
        Complex lFactor = matrix_getElement(luResult.matrix1, i, i);
        Complex uFactor = matrix_getElement(luResult.matrix2, i, i);
        Complex factor = complex_multiplication(lFactor, uFactor);
        prod = complex_multiplication(prod, factor);
    }

    return prod;
}

Matrix matrix_kron(Matrix matrix1, Matrix matrix2) {

    if (matrix1.numRows == 0 || matrix1.numColumns == 0 ||
        matrix2.numRows == 0 || matrix2.numColumns == 0) {
        return (Matrix) { 0 };
    }

    size_t rowDim = matrix1.numRows * matrix2.numRows;
    size_t colDim = matrix1.numColumns * matrix2.numColumns;

    // prepare VLA to store rows
    Complex values[rowDim * colDim];
    size_t valueIndex = 0;
    for (size_t m1RowIndex = 0; m1RowIndex < matrix1.numRows; m1RowIndex++) {
        for (size_t m2RowIndex = 0; m2RowIndex < matrix2.numRows; m2RowIndex++) {
            for (size_t m1ColIndex = 0; m1ColIndex < matrix1.numColumns; m1ColIndex++) {
                for (size_t m2ColIndex = 0; m2ColIndex < matrix2.numColumns; m2ColIndex++) {
                    // For a kronecker product P = kron(A,B),
                    // P[m1RowIndex*m2RowIndex][m1ColIndex*m2ColIndex] = A[m1RowIndex][m1ColIndex] * B[m2RowIndex][m2ColIndex]
                    values[valueIndex] = complex_multiplication(matrix_getElement(matrix1, m1RowIndex, m1ColIndex),
                                                                matrix_getElement(matrix2, m2RowIndex, m2ColIndex));
                    valueIndex++;
                }
            }
        }
    }

    Matrix kroneckerProduct = matrix_fromRowArray(values, rowDim, colDim);
    return kroneckerProduct;
}

Complex matrix_trace(Matrix matrix) {

    if (matrix.numRows != matrix.numColumns) {
        return (Complex) { 0 };
    }
    Complex sum = (Complex) {0.0, 0.0};
    for (size_t i = 0; i < matrix.numRows; i++) {
        sum = complex_addition(sum, matrix_getElement(matrix, i, i));
    }
    return sum;
}

Complex matrix_braket_product(Matrix matrix, Vector bra, Vector ket) {
    
    Vector vector = vector_matrixMultiplication(ket, matrix);
    
    Vector conjugatedBra = vector_conjugateINP(bra);

    return vector_innerProduct(conjugatedBra, vector).value;
}

bool matrix_isDiagonal(Matrix matrix) {

    if (matrix.numRows != matrix.numColumns) {
        return false;
    } else if (matrix.numRows == 1) {
        // Implicitly, this also means that numColumns == 1
        return true;
    }

    // Iteratively check all off-diagonal elements
    for (int rowIndex = 0; rowIndex < matrix.numRows; rowIndex++) {
        for (int colIndex = 0; colIndex < rowIndex; colIndex++) {

            if (rowIndex == colIndex) {
                continue;
            }

            Complex lowerTriangValue = matrix_getElement(matrix, rowIndex, colIndex);
            Complex upperTriangValue = matrix_getElement(matrix, colIndex, rowIndex);
            if (!complex_nearlyEqual(lowerTriangValue, (Complex) { 0.0, 0.0}) ||
            !complex_nearlyEqual(upperTriangValue, (Complex){0.0, 0.0})) {
                return false;
            }
        }
    }
    return true;
}

bool matrix_isSquare(Matrix matrix) {
    return matrix.numRows == matrix.numColumns;
}

bool matrix_isUpperTriangular(Matrix matrix) {

    if (matrix.numRows != matrix.numColumns) {
        return false;
    } else if (matrix.numRows == 1) {
        // Implicity, this also means that numColumns == 1
        return true;
    }

    for (int rowIndex = 0; rowIndex < matrix.numRows; rowIndex++) {
        for (int colIndex = 0; colIndex < rowIndex; colIndex++) {
            Complex lowerTriangValue = matrix_getElement(matrix, rowIndex, colIndex);
            if (!complex_nearlyEqual(lowerTriangValue, (Complex) { 0.0, 0.0})){
                return false;
            }
        }
    }
    return true;
}

bool matrix_isNormal(Matrix matrix) {

    Matrix adjointMatrix = matrix_adjoint(matrix);

    Matrix leftMultiplied = matrix_multiplication(adjointMatrix, matrix);
    Matrix rightMultiplied = matrix_multiplication(matrix, adjointMatrix);

    return matrix_isEqual(leftMultiplied, rightMultiplied);
}

bool matrix_isHermitian(Matrix matrix) {

    Matrix adjointMatrix = matrix_adjoint(matrix);

    return matrix_isEqual(matrix, adjointMatrix);
}

bool matrix_isUnitary(Matrix matrix) {

    Matrix adjointMatrix = matrix_adjoint(matrix);

    Matrix product = matrix_multiplication(matrix, adjointMatrix);
    Matrix identity = matrix_identity(matrix.numRows);

    return matrix_isEqual(identity, product);
}


bool matrix_isEqual(Matrix matrix1, Matrix matrix2) {

    if (matrix1.numRows != matrix2.numRows || matrix1.numColumns != matrix2.numColumns) {
        return false;
    }

    for (size_t rowIndex = 0; rowIndex < matrix1.numRows; rowIndex++) {
       for (size_t colIndex = 0; colIndex < matrix2.numColumns; colIndex++) {
           Complex mat1Value = matrix_getElement(matrix1, rowIndex, colIndex);
           Complex mat2Value = matrix_getElement(matrix2, rowIndex, colIndex);
           if (!complex_nearlyEqual(mat1Value, mat2Value)) {
               return false;
           }
       }
    }
    return true;
}

bool matrix_isZero(Matrix matrix) {
    Matrix zeroMatrix = matrix_zeros(matrix.numRows, matrix.numColumns);

    return matrix_isEqual(matrix, zeroMatrix);
}

bool matrix_isProjector(Matrix matrix) {

    Matrix squaredMatrix = matrix_multiplication(matrix, matrix);

    return matrix_isEqual(matrix, squaredMatrix);
}


// TODO: Eigenvectors, eigenvalues

ComplexTuple matrix_eigvalsQR(Matrix matrix) {

    Matrix iterationMatrix = matrix_clone(matrix);
    const size_t maximumIterations = 1000;

    size_t iterationIndex = 0;
    while(!matrix_isUpperTriangular(iterationMatrix) && iterationIndex < maximumIterations) {
        QRResult qrMatrices = matrix_QRDecomposition(iterationMatrix);
        // Calculate RQ to form next iteration
        iterationMatrix = matrix_multiplication(qrMatrices.matrix2, qrMatrices.matrix1);
        iterationIndex++;
    }

    ComplexTuple eigvals = complexTuple_new(matrix.numRows);
    for (size_t i = 0; i < matrix.numRows; i++) {
        eigvals.data[i] = matrix_getElement(iterationMatrix, i, i);
    }
    return eigvals;
}


void matrix_powerINP(Matrix matrix, unsigned int exponent) {

}

Matrix matrix_scaleINP(Matrix matrix, Complex factor) {
    
    size_t numElements = matrix.numRows * matrix.numColumns;
    for (size_t elementIndex = 0; elementIndex < numElements; elementIndex++) {
        Complex previousElement = matrix.ndArray.values[elementIndex];
        Complex newElement = complex_multiplication(previousElement, factor);
        matrix.ndArray.values[elementIndex] = newElement;
    }
    return matrix;
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

    Matrix qMatrix = matrix_fromColumnVectors(qMatrixColumns);

    return (QRResult) {
        .matrix1 = qMatrix,
        .matrix2 = rMatrix
    };
}

LUResult matrix_LUDecomposition(Matrix matrix) {

    if (matrix.numRows != matrix.numColumns) {
        return (LUResult) { 0 };
    }

    Matrix lMatrix = matrix_identity(matrix.numRows);
    Matrix uMatrix = matrix_zeros(matrix.numRows, matrix.numColumns);

    for (int i = 0; i < matrix.numRows; i++) {

        // Calculating U Matrix entries
        for (int j = i; j < matrix.numColumns; j++) {
            Complex sum = (Complex) {0.0, 0.0};
            for (int k = 0; k < i - 1; k++) {
                Complex lComponent = matrix_getElement(lMatrix, i, k);
                Complex uComponent = matrix_getElement(uMatrix, k, j);
                sum = complex_addition(sum, complex_multiplication(lComponent, uComponent));
            }
            Complex newElement = complex_subtraction(matrix_getElement(matrix, i, j), sum);
            matrix_setElement(uMatrix, i, j, newElement);
        }

        // Calculating L matrix entries
        for (int j = i + 1; j < matrix.numColumns; j++) {
            Complex sum = (Complex) {0.0, 0.0};
            for (int k = 0; k < i - 1; k++) {
                Complex lComponent = matrix_getElement(lMatrix, j, k);
                Complex uComponent = matrix_getElement(uMatrix, k, i);
                sum = complex_addition(sum, complex_multiplication(lComponent, uComponent));
            }
            Complex uQuotient = matrix_getElement(uMatrix, i, j);
            Complex newElement = complex_division(complex_subtraction(matrix_getElement(matrix, j, i), sum), uQuotient).value;
            matrix_setElement(lMatrix, j, i, newElement);
        }
    }

    return (LUResult) {
        .matrix1 = lMatrix,
        .matrix2 = uMatrix
    };

}
