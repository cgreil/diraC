#include <check.h>
#include <stdio.h>
#include <time.h>

#include "complex/complex.h"
#include "ndarray/ndarray.h"

#include "utils/testutils.h"
#include "utils/dataArena.h"

// Global variable for arena
Arena *arena;

START_TEST(testNDArrayClone) {

    Complex values[] = {
            (Complex) { -4.0, 2.0 },
            (Complex) { 5.0, 9.0 },
            (Complex) { 3.0, 2.0 },
            (Complex) { 9.0, -2.0 }
    };

    NDArray expected = NDArray_create(arena, 2, 2, values);
    NDArray actual = NDArray_clone(arena, expected);

    ck_assert_ndarray_eq(expected, actual)
} END_TEST

START_TEST(testNDArrayCloneNoReference) {
    Complex values[] = {
        (Complex) { -4.0, 2.0 },
        (Complex) { 5.0, 9.0 },
        (Complex) { 3.0, 2.0 },
        (Complex) { 9.0, -2.0 }
    };

    NDArray original = NDArray_create(arena, 2, 2, values);
    NDArray copy = NDArray_clone(arena, original);
    // Alter original
    bool setSuccessful = NDArray_setElement(original, 0, 0, (Complex) {2.0, 2.0});
    ck_assert(setSuccessful);
    OptComplex changedNumber = NDArray_getElement(original, 0, 0);
    ck_assert(changedNumber.valid);
    ck_assert_complex_eq(changedNumber.value, ((Complex) {2.0, 2.0}));

    // Make sure complex_clone did not change
    NDArray expected = NDArray_create(arena, 2, 2, values);
    ck_assert_ndarray_eq(expected, copy);
} END_TEST


Suite* ndArrayCloneSuite(void) {
    Suite *suite;

    suite = suite_create("NDArray Clone");

    TCase *testCase1 = tcase_create("testNDArrayClone");
    TCase *testCase2 = tcase_create("testNDArrayCloneNoReference");

    tcase_add_test(testCase1, testNDArrayClone);
    tcase_add_test(testCase2, testNDArrayCloneNoReference);

    suite_add_tcase(suite, testCase1);
    suite_add_tcase(suite, testCase2);

    return suite;
}

/**
 * END OF CLONE TESTS
 */

/**
 * START OF RESIZE TESTS
 */

START_TEST(testNDArrayResize) {

    Complex values[] = {
        (Complex) { -4.0, 2.0 },
        (Complex) { 5.0, 9.0 },
        (Complex) { 3.0, 2.0 },
        (Complex) { 9.0, -2.0 }
    };

    NDArray original = NDArray_create(arena, 2, 2, values);
    NDArray_resize(arena, original, 1, 4);

    ck_assert_int_eq(original.numRows, 1);
    ck_assert_int_eq(original.numColumns, 4);
} END_TEST


Suite *ndArrayResizeSuite(void) {
   Suite *suite;

   suite = suite_create("ndArrayResizeSuite");

   TCase *testCase1 = tcase_create("testNDArrayResize");

   tcase_add_test(testCase1, testNDArrayResize);

   suite_add_tcase(suite, testCase1);

   return suite;
}

/**
 * END OF RESIZE TESTS
 */


int main(void) {

    // initialize arena variable
    arena = arena_init();

    // Array of suitePointers - filled by the respective functions
    Suite *testSuites[] = {
        ndArrayCloneSuite(),
        ndArrayResizeSuite()
    };

    size_t numSuites = sizeof(testSuites) / sizeof(testSuites[0]);

    SRunner *suiteRunner;
    for (size_t i = 0; i < numSuites; ++i) {

        fprintf(stdout, "Running suite (%zu/%zu) \n", i+1, numSuites);

        Suite *currentSuite = testSuites[i];
        suiteRunner = srunner_create(currentSuite);

        // Start timing
        time_t startTime = time(NULL);
        srunner_run_all(suiteRunner, CK_NORMAL);
        time_t endTime = time(NULL);

        int numFailed = srunner_ntests_failed(suiteRunner);
        fprintf(stdout, "Finished with %d failed tests in %f seconds \n", numFailed, difftime(startTime, endTime));
        srunner_free(suiteRunner);
    }

    // cleanup arena
    arena_destroy(arena);

    return 0;
}
