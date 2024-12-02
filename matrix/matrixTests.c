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

    tcase_add_test(testcase1, matrixZeroTest);
    tcase_add_test(testcase2, matrixOnesTest);
    tcase_add_test(testcase3, matrixIdentityTest);

    suite_add_tcase(suite, testcase1);
    suite_add_tcase(suite, testcase2);
    suite_add_tcase(suite, testcase3);

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



Suite *matrixArithmeticSuite(void) {

    Suite *suite = suite_create("matrixArithmeticSuite");

    TCase *testcase1 = tcase_create("matrixAdditionTest");
    TCase *testcase2 = tcase_create("matrixSubtractionTest");
    TCase *testcase3 = tcase_create("matrixMultiplicationTest");
    TCase *testcase4 = tcase_create("matrixFromColumnSetTest");
    TCase *testcase5 = tcase_create("matrixFromColumnSetEmptyTest");
    TCase *testcase6 = tcase_create("matrixTransposeTest");
    TCase *testcase7 = tcase_create("matrixRowPermutationTest");

    tcase_add_test(testcase1, matrixAdditionTest);
    tcase_add_test(testcase2, matrixSubtractionTest);
    tcase_add_test(testcase3, matrixMultiplicationTest);
    tcase_add_test(testcase4, matrixFromColumnSetTest);
    tcase_add_test(testcase5, matrixFromColumnSetEmptyTest);
    tcase_add_test(testcase6, matrixTransposeTest);
    tcase_add_test(testcase7, matrixRowPermutationTest);

    suite_add_tcase(suite, testcase1);
    suite_add_tcase(suite, testcase2);
    suite_add_tcase(suite, testcase3);
    suite_add_tcase(suite, testcase4);
    suite_add_tcase(suite, testcase5);
    suite_add_tcase(suite, testcase6);
    suite_add_tcase(suite, testcase7);

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

