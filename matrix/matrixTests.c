#include <check.h>
#include <stdio.h>
#include <time.h>

#include "utils/testutils.h"

#include "vector/vector.h"
#include "ndarray/ndarray.h"
#include "matrix/matrix.h"


static Arena *arena;

START_TEST(matrixZeroTest) {

    size_t numRows = 4;
    size_t numCols = 5;

    Matrix zeroMat = matrix_zeros(arena, numRows, numCols);

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

    Matrix zeroMat = matrix_zeros(arena, numRows, numCols);

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

Suite *matrixCreationSuite(void) {

    Suite *suite;
    suite = suite_create("matrixCreationSuite");

    TCase *testcase1 = tcase_create("testMatrixZeroCreation");
    TCase *testcase2 = tcase_create("testMatrixOneCreation");

    tcase_add_test(testcase1, matrixZeroTest);
    tcase_add_test(testcase2, matrixOnesTest);

    suite_add_tcase(suite, testcase1);
    suite_add_tcase(suite, testcase2);

    return suite;
}

/**
 * END OF CREATION TESTS
 */

/**
 * START OF ARITHMETIC TESTS
 */

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

