#include <check.h>
#include <stdio.h>
#include <time.h>

#include "utils/testutils.h"
#include "utils/dataArena.h"

#include "vector/vector.h"
#include "vector/vectorCollection.h"
#include "ndarray/ndarray.h"
#include "matrix/matrix.h"


Arena *arena;

START_TEST(matrixZeroTest) {

    size_t numRows = 4;
    size_t numCols = 5;

    Matrix zeroMat = matrix_zeros(numRows, numCols);

    // manually check values, since matrix creaton witll subesequently be tested using
    // these creation utilities

    Complex zeroComp = (Complex) { 0.0, 0.0 };

    for (size_t i = 0; i < numRows; i++) {
        for (size_t j = 0; j < numCols; j++) {

            OptComplex actual = NDArray_getElement(zeroMat.ndArray, i, j);
            // Compare to zero
            ck_assert_complex_eq(zeroComp, actual.value);
        }
    }

} END_TEST


START_TEST(matrixOnesTest) {

    size_t numRows = 3;
    size_t numCols = 3;

    Matrix zeroMat = matrix_ones(numRows, numCols);

    // manually check values, since matrix creaton witll subesequently be tested using
    // these creation utilities

    Complex oneComp = (Complex) { 1.0, 0.0 };

    for (size_t i = 0; i < numRows; i++) {
        for (size_t j = 0; j < numCols; j++) {

            OptComplex actual = NDArray_getElement(zeroMat.ndArray, i, j);
            // Compare to zero
            ck_assert_complex_eq(oneComp, actual.value);
        }
    }

} END_TEST


START_TEST(matrixFromRowArrayTest) {

    size_t numRows = 3;
    size_t numColumns = 2;

    Complex values[] = {
        (Complex) { 9.0, 2.0}, (Complex) { 2.0, 2.0},
        (Complex) { 0.0, 0.0}, (Complex) {1.0, 1.0},
        (Complex) {-2.0, -4.333}, (Complex) {3.0, 3.0}
    };

    Matrix matrix = matrix_fromRowArray(values, numRows, numColumns);

    for (size_t rowIndex = 0; rowIndex < numRows; rowIndex++) {
         for (size_t colIndex = 0; colIndex < numColumns; colIndex++) {
             Complex actualElement = matrix_getElement(matrix, rowIndex, colIndex);
             Complex expectedElement = values[colIndex + rowIndex * numColumns];
             ck_assert_complex_eq(expectedElement, actualElement);
         }
    }

} END_TEST


START_TEST(matrixFromColumnVectorsTest) {

    // length of vectors is equivalent to numRows
    size_t numRows = 4;
    size_t numColumns = 3;

    Vector col1 = vector_fromArray(
        (Complex[]) {(Complex) { 1.0, 1.0}, (Complex) {4.0, 5.0}, (Complex) {-2.3, 1.1111}, (Complex) {1.1, 2.2}},
        numRows);

    Vector col2 = vector_fromArray(
        (Complex[]) {(Complex) {9.0, 4.0}, (Complex) {-3.0, -8.4}, (Complex) {-2.3, 1.1111}, (Complex) {9.2, 12.4}},
        numRows);

    Vector col3 = vector_fromArray(
        (Complex[]) {(Complex) {2.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {8.99, 2.31}, (Complex) {9.2, 12.4}},
        numRows);

    Vector vecArray[] = {col1, col2, col3};

    VectorCollection vecCollection = vectorCollection_fromArray(vecArray, sizeof(vecArray) / sizeof(vecArray[0]), List);
    Matrix matrix = matrix_fromColumnVectors(vecCollection);

    ck_assert_int_eq(matrix.numRows, numRows);
    ck_assert_int_eq(matrix.numColumns, numColumns);

    for (size_t rowIndex = 0; rowIndex < matrix.numRows; rowIndex++) {
        for (size_t columnIndex = 0; columnIndex < matrix.numColumns; columnIndex++) {
            Complex actualElement = matrix_getElement(matrix, rowIndex, columnIndex);
            Complex expectedElement = vector_getElement(vecArray[columnIndex], rowIndex).value;
            ck_assert_complex_eq(expectedElement, actualElement);
        }
    }

} END_TEST



START_TEST(matrixIdentityTest) {

    size_t dimension = 4;

    Matrix identity = matrix_identity(dimension);

    Complex oneComp = (Complex) { 1.0, 0.0 };
    Complex zeroComp = (Complex) { 0.0, 0.0 };

    for (size_t i = 0; i < identity.numRows; i++) {
        for (size_t j = 0; j < identity.numColumns; j++) {

            if (i == j) {
                ck_assert_complex_eq(oneComp, NDArray_getElement(identity.ndArray, i, j).value);
            } else {
                ck_assert_complex_eq(zeroComp, NDArray_getElement(identity.ndArray, i, j).value);
            }
        }
    }

} END_TEST


Suite *matrixCreationSuite(void) {

    Suite *suite;
    suite = suite_create("matrixCreationSuite");

    TCase *testcase1 = tcase_create("testMatrixZeroCreation");
    TCase *testcase2 = tcase_create("testMatrixOneCreation");
    TCase *testcase3 = tcase_create("testMatrixIdCreation");
    TCase *testcase4 = tcase_create("testMatrixFromRowArrayCreation");
    TCase *testcase5 = tcase_create("testMatrixFromColumnVectors");

    tcase_add_test(testcase1, matrixZeroTest);
    tcase_add_test(testcase2, matrixOnesTest);
    tcase_add_test(testcase3, matrixIdentityTest);
    tcase_add_test(testcase4, matrixFromRowArrayTest);
    tcase_add_test(testcase5, matrixFromColumnVectorsTest);

    suite_add_tcase(suite, testcase1);
    suite_add_tcase(suite, testcase2);
    suite_add_tcase(suite, testcase3);
    suite_add_tcase(suite, testcase4);
    suite_add_tcase(suite, testcase5);

    return suite;
}

/**
 * END OF CREATION TESTS
 */

/**
 * START OF ARITHMETIC TESTS
 */


START_TEST(matrixAdditionTest) {

    size_t numRows = 2;
    size_t numColumns = 2;

    Complex matrix1Values[] = {
        (Complex) { 3.44, 9.88 },
        (Complex) { 7.21, 0.0 },
        (Complex) { 9.32, 9.99999 },
        (Complex) { 5.0, 2.25 }
    };

    Complex matrix2Values[] = {
        (Complex) { 2.98, 4.6 },
        (Complex) { 7.0, 2 },
        (Complex) { 4, 2 },
        (Complex) { 0.0001, 0.2 }
    };

    size_t matLen = numRows * numColumns;

    Complex resultValues[matLen];
    for (int i = 0; i < matLen; i++) {
        resultValues[i] = complex_addition(matrix1Values[i], matrix2Values[i]);
    }

    Matrix matrix1 = matrix_fromRowArray(matrix1Values, numRows, numColumns);
    Matrix matrix2 = matrix_fromRowArray(matrix2Values, numRows, numColumns);

    Matrix expectedMatrix = matrix_fromRowArray(resultValues, numRows, numColumns);
    Matrix resultMatrix = matrix_addition(matrix1, matrix2);

    ck_assert_matrix_eq(expectedMatrix, resultMatrix);

} END_TEST

START_TEST(matrixSubtractionTest) {

    size_t numRows = 2;
    size_t numColumns = 2;

    Complex matrix1Values[] = {
        (Complex) { 3.44, 9.88 },
        (Complex) { 7.21, 0.0 },
        (Complex) { 9.32, 9.99999 },
        (Complex) { 5.0, 2.25 }
    };

    Complex matrix2Values[] = {
        (Complex) { 2.98, 4.6 },
        (Complex) { 7.0, 2 },
        (Complex) { 4, 2 },
        (Complex) { 0.0001, 0.2 }
    };

    size_t matLen = numRows * numColumns;

    Complex resultValues[matLen];
    for (int i = 0; i < matLen; i++) {
        resultValues[i] = complex_subtraction(matrix1Values[i], matrix2Values[i]);
    }

    Matrix matrix1 = matrix_fromRowArray(matrix1Values, numRows, numColumns);
    Matrix matrix2 = matrix_fromRowArray(matrix2Values, numRows, numColumns);

    Matrix expectedMatrix = matrix_fromRowArray(resultValues, numRows, numColumns);
    Matrix resultMatrix = matrix_subtraction(matrix1, matrix2);

    ck_assert_matrix_eq(expectedMatrix, resultMatrix);

} END_TEST

START_TEST(matrixMultiplicationTest) {
    size_t numRows = 2;
    size_t numColumns = 2;

    Complex matrix1Values[] = {
        (Complex) { 4.0, 2.0 },
        (Complex) { 7.0, 0.5 },
        (Complex) { 9.75, 9.0 },
        (Complex) { 5.0, 2.25 }
    };

    Complex matrix2Values[] = {
        (Complex) { 2.75, 4.5 },
        (Complex) { 7.0, 2.0 },
        (Complex) { 4.0, 2.0 },
        (Complex) { 0.0, 0.5 }
    };

    Matrix matrix1 = matrix_fromRowArray(matrix1Values, numRows, numColumns);
    Matrix matrix2 = matrix_fromRowArray(matrix2Values, numRows, numColumns);

    Matrix multiplicationResult = matrix_multiplication(matrix1, matrix2);

    Complex matrixData[] = {
        (Complex) {29.0, 39.5},
        (Complex) {23.75, 25.5},
        (Complex) {1.8125, 87.625},
        (Complex) {49.125, 85.0}
    };

    Matrix expectedMatrix = matrix_fromRowArray(matrixData, numRows, numColumns);
    ck_assert_matrix_eq(expectedMatrix, multiplicationResult)

} END_TEST

START_TEST(matrixFromColumnSetTest) {

    Vector vectorArray[] = {
        vector_fromArray((Complex[]) {
            (Complex) {1.0, 2.0},
            (Complex) {4.0, 4.0},
            (Complex) {2.0, 3.0}
            },
            3
        ),
        vector_fromArray((Complex[]) {
            (Complex) {4.0, 2.0},
            (Complex) {7.0, 3.0},
            (Complex) {9.9, 9.9}
            },
            3
        )
    };

    VectorCollection vectorList = vectorCollection_fromArray(vectorArray, sizeof(vectorArray) / sizeof(vectorArray[0]), List);

    Matrix matrix = matrix_fromColumnVectors(vectorList);

    for (size_t rowIndex = 0; rowIndex < matrix.numRows; rowIndex++) {
        for (size_t colIndex = 0; colIndex < matrix.numColumns; colIndex++) {
            Complex matrixValue = matrix_getElement(matrix, rowIndex, colIndex);
            Complex comparisonValue = vector_getElement(vectorArray[colIndex], rowIndex).value;
            ck_assert_complex_eq(matrixValue, comparisonValue);
        }
    }
} END_TEST

START_TEST(matrixFromColumnSetEmptyTest) {

    VectorCollection vectorList = vectorCollection_createEmpty(List);
    Matrix matrix = matrix_fromColumnVectors(vectorList);

    Matrix nullMatrix = (Matrix) { 0 };

    ck_assert_matrix_eq(nullMatrix, matrix);
} END_TEST

START_TEST(matrixTransposeTest) {

    Complex rowArray[] = {
        (Complex) {0.0, 0.0},
        (Complex) {1.0, 1.0},
        (Complex) {-1.0, -1.0},
        (Complex) {0.0, 0.0}
    };

    Matrix matrix = matrix_fromRowArray(
        rowArray,
        2,
        2
    );

    ck_assert_complex_eq(matrix_getElement(matrix, 0, 0), rowArray[0]);
    ck_assert_complex_eq(matrix_getElement(matrix, 0, 1), rowArray[2]);
    ck_assert_complex_eq(matrix_getElement(matrix, 1, 0), rowArray[1]);
    ck_assert_complex_eq(matrix_getElement(matrix, 1, 1), rowArray[3]);

} END_TEST

START_TEST(matrixRowPermutationTest) {

    Matrix originalMatrix = matrix_fromRowArray(
        (Complex[]) {
            (Complex) {0.0, 0.0},
            (Complex) {1.0, 1.0},
            (Complex) {-1.0, -1.0},
            (Complex) {0.0, 0.0}
        },
        2,
        2
    );

    Matrix permutationMatrix = matrix_permutation(2, 0, 1);

    Matrix permutedMatrix = matrix_multiplication(permutationMatrix, originalMatrix);

    ck_assert_matrix_eq(matrix_transpose(originalMatrix), permutedMatrix);

} END_TEST

START_TEST(matrixDiagonalizeTest) {

    /**
     * Use a diagonalizeable matrix defined by:
     * | 1 1 0 0 |
     * | 0 1 0 0 |
     * | 0 0 2 1 |
     * | 0 0 0 2 |
     *
     */


    size_t dimension = 4;
    Matrix matrix = matrix_identity(dimension);

    matrix_setElement(matrix, 0, 1, (Complex) {1.0});
    matrix_setElement(matrix, 2, 2, (Complex) {2.0});
    matrix_setElement(matrix, 2, 3, (Complex) {1.0});
    matrix_setElement(matrix, 3, 3, (Complex) {2.0});

    ck_assert(!matrix_isDiagonal(matrix));

    Matrix diagonalMatrix = matrix_diagonalize(matrix);

    ck_assert(matrix_isDiagonal(diagonalMatrix));
} END_TEST


START_TEST(matrixAlreadyDiagonalTest) {

    size_t dimension = 4;
    Matrix matrix = matrix_identity(dimension);

    ck_assert(matrix_isDiagonal(matrix));

    Matrix diagonalMatrix = matrix_diagonalize(matrix);

    ck_assert(matrix_isDiagonal(diagonalMatrix));
    ck_assert_matrix_eq(matrix, diagonalMatrix);
} END_TEST

START_TEST(matrixNotDiagonalizeableTest) {

    /**
     * Use nondiagonalizable 2x2 matrix
     * | 0 1 |
     * | 0 0 |
     * as example:
     */

    size_t dimension = 2;
    Matrix matrix = matrix_identity(dimension);

    matrix_setElement(matrix, 2, 2, (Complex) {0.0, 0.0});

    ck_assert(!matrix_isDiagonal(matrix));

    //Matrix diagMatrix = matrix_diagonalize(matrix);

    // TODO: How to actually verify this ... use OptMatrix?

    ck_assert(true);

} END_TEST

START_TEST(matrixKronTest) {

    /**
     * Calculate the kronecker Product of the
     * pauli gates Y and Z, i.e. kron(Y,Z)
     *
     */

    Complex pauliYArr[] = {
        (Complex) {0.0, 0.0}, (Complex) {0.0, -1.0},
        (Complex) {0.0, 1.0}, (Complex) {0.0, 0.0}
    };
    Matrix pauliY = matrix_fromRowArray(pauliYArr, 2, 2);

    Complex pauliZArr[] = {
        (Complex) {1.0, 0.0}, (Complex) {0.0, 0.0},
        (Complex) {0.0, 0.0}, (Complex) {-1.0, 0.0}
    };
    Matrix pauliZ = matrix_fromRowArray(pauliZArr, 2, 2);

    Complex expectedValues[] = {
        (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {0.0, -1.0}, (Complex) {0.0, 0.0},
        (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {0.0, 1.0},
        (Complex) {0.0, 1.0}, (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0},
        (Complex) {0.0, 0.0}, (Complex) {0.0, -1.0}, (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0},
    };
    Matrix expectedMatrix = matrix_fromRowArray(expectedValues, 4, 4);

    Matrix actualMatrix = matrix_kron(pauliY, pauliZ);

    ck_assert_int_eq(actualMatrix.numRows, 4);
    ck_assert_int_eq(actualMatrix.numColumns, 4);

    ck_assert_matrix_eq(expectedMatrix, actualMatrix);

} END_TEST

// TODO: test kronecker product of non-square matrices

START_TEST(matrixDeterminantTest) {

    Complex pauliZArr[] = {
        (Complex) {1.0, 0.0}, (Complex) {0.0, 0.0},
        (Complex) {0.0, 0.0}, (Complex) {-1.0, 0.0}
    };
    Matrix pauliZ = matrix_fromRowArray(pauliZArr, 2, 2);

    Complex determinant = matrix_determinant(pauliZ);
    ck_assert_complex_eq(((Complex) {-1.0, 0.0}), determinant);

} END_TEST

//TODO: Test return value of determinant for non-square matrices

START_TEST(matrixTraceTest) {

    Complex values[] = {
        (Complex) {1.0, 1.0}, (Complex) {2.0, 8.0}, (Complex) {4.0, -8.0},
        (Complex) {-3.0, 0.0}, (Complex) {-3.3, -2.0}, (Complex) {0.0, 0.0},
        (Complex) {4.0, 4.0}, (Complex) {-1.0, 0.0}, (Complex) {8.0, 2.0}
    };
    Matrix matrix = matrix_fromRowArray(values, 3, 3);

    Complex expectedValue = (Complex) {7.7, -1.0};
    Complex actualValue = matrix_trace(matrix);

    ck_assert_complex_eq(expectedValue, actualValue);
} END_TEST

// TODO: Test for non-square matrix


START_TEST(matrixIsUpperTriangularPositiveTest) {

    Complex values[] = {
        (Complex) {1.0, 1.0}, (Complex) {2.0, 8.0}, (Complex) {4.0, -8.0},
        (Complex) {0.0, 0.0}, (Complex) {-3.3, -2.0}, (Complex) {0.0, 0.0},
        (Complex) {0.0, 0.0}, (Complex) {-0.0, 0.0}, (Complex) {0.0, 2.0}
    };
    Matrix upperMatrix = matrix_fromRowArray(values, 3, 3);

    ck_assert(matrix_isUpperTriangular(upperMatrix));

} END_TEST

START_TEST(matrixIsUpperTriangularNegativeTest) {

    Complex values[] = {
        (Complex) {1.0, 0.0}, (Complex) {0.0, 0.0},
        (Complex) {0.0, -2.0}, (Complex) {8.0, 9.999}
    };
    Matrix matrix = matrix_fromRowArray(values, 2, 2);

    ck_assert(!matrix_isUpperTriangular(matrix));
} END_TEST

// TODO: ISDiagonalizeable Test

START_TEST(matrixIsDiagonalPositiveTest) {

    Complex values[] = {
        (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0},
        (Complex) {0.0, 0.0}, (Complex) {4.0, 2.0}
    };
    Matrix matrix = matrix_fromRowArray(values, 2, 2);

    ck_assert(matrix_isDiagonal(matrix));
} END_TEST

START_TEST(matrixIsDiagonalNegativeTest) {

    Complex values[] = {
        (Complex) {1.0, 3.0}, (Complex) {9.0, 0.0001},
        (Complex) {0.0, 0.0}, (Complex) {4.0, 2.0}
    };
    Matrix matrix = matrix_fromRowArray(values, 2, 2);

    ck_assert(!matrix_isDiagonal(matrix));
} END_TEST

START_TEST(matrixIsSquarePositiveTest) {

    Complex values[] = {
        (Complex) {1.0, 3.0}, (Complex) {9.0, 0.0001}, (Complex) {0.0, 8.0},
        (Complex) {0.0, 0.0}, (Complex) {4.0, 2.0}, (Complex) {2.0, 3.0},
        (Complex) {0.0, 0.0}, (Complex) {4.0, 2.0}, (Complex) {2.0, 3.0},
    };
    Matrix matrix = matrix_fromRowArray(values, 3, 3);

    ck_assert(matrix_isSquare(matrix));
} END_TEST

START_TEST(matrixIsSquareNegativeTest) {

    Complex values[] = {
        (Complex) {1.0, 3.0}, (Complex) {9.0, 0.0001}, (Complex) {0.0, 8.0},
        (Complex) {0.0, 0.0}, (Complex) {4.0, 2.0}, (Complex) {2.0, 3.0},
        (Complex) {0.0, 0.0}, (Complex) {4.0, 2.0}, (Complex) {2.0, 3.0},
        (Complex) {0.0, 0.0}, (Complex) {4.0, 2.0}, (Complex) {2.0, 3.0},
    };
    Matrix matrix = matrix_fromRowArray(values, 4, 3);

    ck_assert(!matrix_isSquare(matrix));
} END_TEST

START_TEST(matrixIsNormalPositiveTest) {

    Matrix matrix = matrix_identity(2);
    matrix_setElement(matrix, 1, 1, (Complex) {1.0, 1.0});
    matrix_setElement(matrix, 1, 1, (Complex) {1.0, 1.0});

    ck_assert(matrix_isNormal(matrix));
} END_TEST


START_TEST(matrixIsNormalNegativeTest) {

    /**
     * Use non-normal matrix
     * | 0 1 |
     * | 0 0 |
     * as example
     */

    Matrix matrix = matrix_identity(2);
    matrix_setElement(matrix, 1, 1, (Complex) {0, 0});

    ck_assert(!matrix_isNormal(matrix));
} END_TEST

START_TEST(matrixIsHermitianPositiveTest) {

    Matrix matrix = matrix_identity(2);
    matrix_setElement(matrix, 0, 0, (Complex) {2.0, 2.0});
    matrix_setElement(matrix, 1, 1, (Complex) {3.0, 3.0});

    ck_assert(matrix_isHermitian(matrix));
} END_TEST

START_TEST(matrixIsHermitianNegativeTest) {

    Matrix matrix = matrix_identity(3);
    matrix_setElement(matrix, 2, 1, (Complex) {2.0, -5.0});

    ck_assert(!matrix_isHermitian(matrix));
} END_TEST

START_TEST(matrixIsUnitaryPositiveTest) {

    Matrix matrix = matrix_identity(3);

    ck_assert(matrix_isUnitary(matrix));
} END_TEST

START_TEST(matrixIsUnitaryNegativeTest) {

    Matrix matrix = matrix_identity(3);
    matrix_setElement(matrix, 1, 2, (Complex) {0.0, 2.0});

    ck_assert(!matrix_isUnitary(matrix));
} END_TEST

START_TEST(matrixIsEqualPositiveTest) {

    Complex values[] = {
        (Complex) {1.0, 3.0}, (Complex) {9.0, 0.0001},
        (Complex) {0.0, 0.0}, (Complex) {4.0, 2.0}
    };
    Matrix matrix1 = matrix_fromRowArray(values, 2, 2);

    Vector rowVec1 = vector_fromArray(&values[0], 2);
    Vector rowVec2 = vector_fromArray(&values[1], 2);
    VectorCollection vectorCollection = vectorCollection_fromArray((Vector[]){rowVec1, rowVec2}, 2, List);
    Matrix matrix2 = matrix_fromRowVectors(vectorCollection);

    ck_assert(matrix_isEqual(matrix1, matrix2));
} END_TEST

START_TEST(matrixIsEqualNegativeTest) {

    Complex values[] = {
        (Complex) {1.0, 3.0}, (Complex) {9.0, 0.0001},
        (Complex) {0.0, 0.0}, (Complex) {4.0, 2.0}
    };
    Matrix matrix1 = matrix_fromRowArray(values, 2, 2);
    Matrix matrix2 = matrix_identity(2);

    ck_assert(!matrix_isEqual(matrix1, matrix2));
} END_TEST

START_TEST(matrixIsZeroPositiveTest) {

    size_t numRows = 5;
    size_t numCols = 6;
    Matrix zeroMatrix = matrix_zeros(numRows, numCols);

    ck_assert(matrix_isZero(zeroMatrix));

} END_TEST

START_TEST(matrixIsZeroNegativeTest) {

    size_t numRows = 4;
    size_t numCols = 8;

    Matrix matrix = matrix_zeros(numRows, numCols);
    matrix_setElement(matrix, 3, 7, (Complex) {0.0, 0.01});

    ck_assert(!matrix_isZero(matrix));

} END_TEST




Suite *matrixArithmeticSuite(void) {

    Suite *suite = suite_create("matrixArithmeticSuite");

    TCase *testcase1 = tcase_create("matrixAdditionTest");
    TCase *testcase2 = tcase_create("matrixSubtractionTest");
    TCase *testcase3 = tcase_create("matrixMultiplicationTest");
    TCase *testcase4 = tcase_create("matrixFromColumnSetTest");
    TCase *testcase5 = tcase_create("matrixFromColumnSetEmptyTest");
    TCase *testcase6 = tcase_create("matrixTransposeTest");
    TCase *testcase7 = tcase_create("matrixRowPermutationTest");
    TCase *testcase8 = tcase_create("matrixDiagonalizationTest");
    TCase *testcase9 = tcase_create("matrixAlreadyDiagonalTest");
    TCase *testcase10 = tcase_create("matrixNotDiagonalizableTest");
    TCase *testcase11 = tcase_create("matrixKroneckerTest");
    TCase *testcase12 = tcase_create("matrixDeterminantTest");
    TCase *testcase13 = tcase_create("matrixTraceTest");
    TCase *testcase14 = tcase_create("matrixIsUpperTriangularPositiveTest");
    TCase *testcase15 = tcase_create("matrixIsUpperTriangularNegativeTest");
    TCase *testcase16 = tcase_create("matrixIsDiagonalPositiveTest");
    TCase *testcase17 = tcase_create("matrixIsDiagonalNegativeTest");
    TCase *testcase18 = tcase_create("matrixIsSquarePositiveTest");
    TCase *testcase19 = tcase_create("matrixIsSquareNegativeTest");
    TCase *testcase20 = tcase_create("matrixIsNormalPositiveTest");
    TCase *testcase21 = tcase_create("matrixIsNormalNegativeTest");
    TCase *testcase22 = tcase_create("matrixIsHermitianPositiveTest");
    TCase *testcase23 = tcase_create("matrixIsHermitianNegativeTest");
    TCase *testcase24 = tcase_create("matrixIsUnitaryPositiveTest");
    TCase *testcase25 = tcase_create("matrixIsUnitaryNegativeTest");
    TCase *testcase26 = tcase_create("matrixIsEqualPositiveTest");
    TCase *testcase27 = tcase_create("matrixIsEqualNegativeTest");
    TCase *testcase28 = tcase_create("matrixIsZeroPositiveTest");
    TCase *testcase29 = tcase_create("matrixIsZeroNegativeTest");

    tcase_add_test(testcase1, matrixAdditionTest);
    tcase_add_test(testcase2, matrixSubtractionTest);
    tcase_add_test(testcase3, matrixMultiplicationTest);
    tcase_add_test(testcase4, matrixFromColumnSetTest);
    tcase_add_test(testcase5, matrixFromColumnSetEmptyTest);
    tcase_add_test(testcase6, matrixTransposeTest);
    tcase_add_test(testcase7, matrixRowPermutationTest);
    tcase_add_test(testcase8, matrixDiagonalizeTest);
    tcase_add_test(testcase9, matrixAlreadyDiagonalTest);
    tcase_add_test(testcase10, matrixNotDiagonalizeableTest);
    tcase_add_test(testcase11, matrixKronTest);
    tcase_add_test(testcase12, matrixDeterminantTest);
    tcase_add_test(testcase13, matrixTraceTest);
    tcase_add_test(testcase14, matrixIsUpperTriangularPositiveTest);
    tcase_add_test(testcase15, matrixIsUpperTriangularNegativeTest);
    tcase_add_test(testcase16, matrixIsDiagonalPositiveTest);
    tcase_add_test(testcase17, matrixIsDiagonalNegativeTest);
    tcase_add_test(testcase18, matrixIsSquarePositiveTest);
    tcase_add_test(testcase19, matrixIsSquareNegativeTest);
    tcase_add_test(testcase20, matrixIsNormalPositiveTest);
    tcase_add_test(testcase21, matrixIsNormalNegativeTest);
    tcase_add_test(testcase22, matrixIsHermitianPositiveTest);
    tcase_add_test(testcase23, matrixIsHermitianNegativeTest);
    tcase_add_test(testcase24, matrixIsUnitaryPositiveTest);
    tcase_add_test(testcase25, matrixIsUnitaryNegativeTest);
    tcase_add_test(testcase26, matrixIsEqualPositiveTest);
    tcase_add_test(testcase27, matrixIsEqualNegativeTest);
    tcase_add_test(testcase28, matrixIsZeroPositiveTest);
    tcase_add_test(testcase29, matrixIsZeroNegativeTest);

    suite_add_tcase(suite, testcase1);
    suite_add_tcase(suite, testcase2);
    suite_add_tcase(suite, testcase3);
    suite_add_tcase(suite, testcase4);
    suite_add_tcase(suite, testcase5);
    suite_add_tcase(suite, testcase6);
    suite_add_tcase(suite, testcase7);
    suite_add_tcase(suite, testcase8);
    suite_add_tcase(suite, testcase9);
    suite_add_tcase(suite, testcase10);
    suite_add_tcase(suite, testcase11);
    suite_add_tcase(suite, testcase12);
    suite_add_tcase(suite, testcase13);
    suite_add_tcase(suite, testcase14);
    suite_add_tcase(suite, testcase15);
    suite_add_tcase(suite, testcase16);
    suite_add_tcase(suite, testcase17);
    suite_add_tcase(suite, testcase18);
    suite_add_tcase(suite, testcase19);
    suite_add_tcase(suite, testcase20);
    suite_add_tcase(suite, testcase21);
    suite_add_tcase(suite, testcase22);
    suite_add_tcase(suite, testcase23);
    suite_add_tcase(suite, testcase24);
    suite_add_tcase(suite, testcase25);
    suite_add_tcase(suite, testcase26);
    suite_add_tcase(suite, testcase27);
    suite_add_tcase(suite, testcase28);
    suite_add_tcase(suite, testcase29);


    return suite;
}


int main(void) {

    arena = arena_init();

    Suite *testSuites[] = {
        matrixCreationSuite(),
        matrixArithmeticSuite()
    };

    size_t numSuites = sizeof(testSuites) / sizeof(testSuites[0]);

    SRunner *suiteRunner;

    for(size_t i = 0; i < numSuites; i++) {

        fprintf(stdout, "Running suite (%zu/%zu) \n", i+1, numSuites);

        Suite *currentSuite = testSuites[i];
        suiteRunner = srunner_create(currentSuite);

        time_t startTime = time(NULL);
        srunner_run_all(suiteRunner, CK_NORMAL);
        time_t endTime = time(NULL);

        int numFailed = srunner_ntests_failed(suiteRunner);
        fprintf(stdout, "Finished with %d failed tests in %f seconds", numFailed, difftime(endTime, startTime));
    }

    arena_destroy(arena);

    return 0;
}

