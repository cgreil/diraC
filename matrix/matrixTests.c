#include <check.h>
#include <stdio.h>
#include <time.h>

#include "utils/testutils.h"
#include "utils/dataArena.h"

#include "vector/vector.h"
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

            OptComplex actual = NDArray_getElement(zeroMat.dataArray, i, j);
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

            OptComplex actual = NDArray_getElement(zeroMat.dataArray, i, j);
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
                ck_assert_complex_eq(oneComp, NDArray_getElement(identity.dataArray, i, j).value);
            } else {
                ck_assert_complex_eq(zeroComp, NDArray_getElement(identity.dataArray, i, j).value);
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

    Matrix matrix1 = matrix_fromArray(matrix1Values, numRows, numColumns);
    Matrix matrix2 = matrix_fromArray(matrix2Values, numRows, numColumns);

    Matrix expectedMatrix = matrix_fromArray(resultValues, numRows, numColumns);
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

    Matrix matrix1 = matrix_fromArray(matrix1Values, numRows, numColumns);
    Matrix matrix2 = matrix_fromArray(matrix2Values, numRows, numColumns);

    Matrix expectedMatrix = matrix_fromArray(resultValues, numRows, numColumns);
    Matrix resultMatrix = matrix_subtraction(matrix1, matrix2);

    ck_assert_matrix_eq(expectedMatrix, resultMatrix);

} END_TEST
/*
START_TEST(matrixMultiplicationTest) {

} END_TEST
*/

Suite *matrixArithmeticSuite(void) {

    Suite *suite = suite_create("matrixArithmeticSuite");

    TCase *testcase1 = tcase_create("matrixAdditionTest");
    TCase *testcase2 = tcase_create("matrixSubtractionTest");

    tcase_add_test(testcase1, matrixAdditionTest);
    tcase_add_test(testcase2, matrixSubtractionTest);

    suite_add_tcase(suite, testcase1);
    suite_add_tcase(suite, testcase2);

    return suite;
}


int main(void) {

    arena = arena_init();

    Suite *testSuites[] = {
        matrixCreationSuite()
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

