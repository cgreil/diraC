#include <stdio.h>
#include <check.h>
#include <time.h>

#include "complex.h"

/*
 * Test Utils
 */

#define ck_assert_complex_eq(expected, actual) \
    ck_assert_float_eq((expected).re, (actual).re); \
    ck_assert_float_eq((expected).im, (actual).im); 

/**
 * Testfile containing tests for complex number operations using
 * the check library
 *
 *
 *
 */

/*
 * START OF ADDITION TESTS
 */

START_TEST(testComplexAdd) {
    
    Complex z1 = {0.0, 1.0};
    Complex z2 = {1.0, 0.0};

    Complex expected = {1.0, 1.0};
    Complex actual = add(z1, z2);

    ck_assert_complex_eq(expected, actual);

}END_TEST

START_TEST(testComplexAddOneNegative) {
    
    Complex z1 = { 2.0, 4.0 };
    Complex z2 = { -4.0, -2.0 };

    Complex expected = { -2.0, 2.0 };
    Complex actual = add(z1, z2);

    ck_assert_complex_eq(expected, actual);
} END_TEST

START_TEST(testComplexAddTwoNegatives) {

    Complex z1 = { -1.2, -8.0 };
    Complex z2 = { -9.6, -4.4 };

    Complex expected = { -10.8, -12.4 };
    Complex actual = add(z1, z2);

    ck_assert_complex_eq(expected, actual);    
}

Suite * complexAddSuite(void) {
    Suite *suite;

    suite = suite_create("Complex Addition");
    
    TCase *testCase1 = tcase_create("additionTwoPositives");
    TCase *testCase2 = tcase_create("additionOneNegative");
    TCase *testCase3 = tcase_create("additionTwoNegatives");

    tcase_add_test(testCase1, testComplexAdd);
    tcase_add_test(testCase2, testComplexAddOneNegative);
    tcase_add_test(testCase3, testComplexAddTwoNegatives);

    suite_add_tcase(suite, testCase1);
    suite_add_tcase(suite, testCase2);
    suite_add_tcase(suite, testCase3);

    return suite;
}

/*
 * END OF ADDITION TESTS
 */

/*
 * START OF SUBTRACTION TESTS
 */ 

START_TEST(testComplexSubtraction) {

    Complex z1 = { 5.0, 2.0 };
    Complex z2 = { 4.0, 1.0 };

    Complex expected = { 1.0, 1.0 };
    Complex actual = subtract(z1, z2);

    ck_assert_complex_eq(expected, actual);
} END_TEST

START_TEST (testComplexSubtractionOneNegative) {

    Complex z1 = { 8.0, 6.0 };
    Complex z2 = { -3.0, -4.0 };

    Complex expected = { 11.0, 10.0 };
    Complex actual = subtract(z1, z2);

    ck_assert_complex_eq(expected, actual);
} END_TEST

START_TEST (testComplexSubtractionTwoNegatives) {

    Complex z1 = { -5.3, -2.4 };
    Complex z2 = { -7.2, -9.1 };

    Complex expected = { 1.9, 6.7 };
    Complex actual = subtract(z1, z2);

    ck_assert_complex_eq(expected, actual);
} END_TEST

Suite *complexSubtractionSuite(void) {

    Suite *suite = suite_create("Complex subtraction");

    TCase *testCase1 = tcase_create("subtractionTwoPositives");
    TCase *testCase2 = tcase_create("subtractionOneNegative");
    TCase *testCase3 = tcase_create("subtractionTwoNegatives");

    tcase_add_test(testCase1, testComplexSubtraction);
    tcase_add_test(testCase2, testComplexSubtractionOneNegative);
    tcase_add_test(testCase3, testComplexSubtractionTwoNegatives);

    suite_add_tcase(suite, testCase1);
    suite_add_tcase(suite, testCase2);
    suite_add_tcase(suite, testCase3);

    return suite;
}


/*
 * TESTSUITE REGISTRATION
 *
 */



int main() {

    // Array of suitePointers - filled by the respective functions
    Suite *testSuites[] = {
        complexAddSuite(),
        complexSubtractionSuite()
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

    return 0;
}
