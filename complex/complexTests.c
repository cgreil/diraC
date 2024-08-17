#include <stdio.h>
#include <check.h>
#include <time.h>

#include "complex.h"
#include "utils/testutils.h"

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
    Complex actual = complex_addition(z1, z2);

    ck_assert_complex_eq(expected, actual)

}END_TEST

START_TEST(testComplexAddOneNegative) {
    
    Complex z1 = { 2.0, 4.0 };
    Complex z2 = { -4.0, -2.0 };

    Complex expected = { -2.0, 2.0 };
    Complex actual = complex_addition(z1, z2);

    ck_assert_complex_eq(expected, actual)
} END_TEST

START_TEST(testComplexAddTwoNegatives) {

    Complex z1 = { -1.2, -8.0 };
    Complex z2 = { -9.6, -4.4 };

    Complex expected = { -10.8, -12.4 };
    Complex actual = complex_addition(z1, z2);

    ck_assert_complex_eq(expected, actual)
} END_TEST

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

    ck_assert_complex_eq(expected, actual)
} END_TEST

START_TEST (testComplexSubtractionOneNegative) {

    Complex z1 = { 8.0, 6.0 };
    Complex z2 = { -3.0, -4.0 };

    Complex expected = { 11.0, 10.0 };
    Complex actual = subtract(z1, z2);

    ck_assert_complex_eq(expected, actual)
} END_TEST

START_TEST (testComplexSubtractionTwoNegatives) {

    Complex z1 = { -5.3, -2.4 };
    Complex z2 = { -7.2, -9.1 };

    Complex expected = { 1.9, 6.7 };
    Complex actual = subtract(z1, z2);

    ck_assert_complex_eq(expected, actual)
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
 * END OF SUBTRACTION TESTS
 *
 */

 /*
  * START OF MULTIPLICATION TESTS
  */

START_TEST (complexMultiplicationTest) {

    Complex z1 = { 4.0, 2.0 };
    Complex z2 = { 2.0, -5.0};

    Complex expected = { 18.0, -16.0 };
    Complex actual = complex_multiplication(z1, z2);

    ck_assert_complex_eq(expected, actual)
} END_TEST

START_TEST (complexMultiplicationImaginaryTest) {

    Complex z1 = { 0.0, 1.0 };
    Complex z2 = { 0.0, 1.0 };
    
    Complex expected = { -1.0, 0.0 };
    Complex actual = complex_multiplication(z1, z2);

    ck_assert_complex_eq(expected, actual)
} END_TEST

Suite *complexMultiplicationSuite(void) {

    Suite *suite = suite_create("Complex multiplication");

    TCase *testCase1 = tcase_create("multiplicationTestOne");
    TCase *testCase2 = tcase_create("mulitplicationTestTwo");

    tcase_add_test(testCase1, complexMultiplicationTest);
    tcase_add_test(testCase2, complexMultiplicationImaginaryTest);

    suite_add_tcase(suite, testCase1);
    suite_add_tcase(suite, testCase2);

    return suite;
}
/*
 * END OF COMPLEX MULTIPLICATION TESTS
 */

 /*
  * START OF COMPLEX DIVISION TESTS
  */

START_TEST (complexDivisionTest) {

    Complex z1 = { 16.0, 4.0 };
    Complex z2 = { 2.0, 3.0 };

    Complex expected = { 44.0/13.0, -(40.0/13.0)};
    OptComplex actual = divide(z1, z2);

    ck_assert(actual.valid);
    ck_assert_complex_eq(expected, actual.value)
} END_TEST

START_TEST (complexDivisonZeroTest) {

    Complex z1 = { 15.0, 2.0 };
    Complex z2 = { 0.0, 0.0};

    OptComplex actual = divide(z1, z2);
    Complex expected = { 0.0, 0.0 };

    ck_assert(!actual.valid);
    ck_assert_complex_eq(expected, actual.value)
} END_TEST

Suite *complexDivisionSuite(void) {

    Suite *suite = suite_create("Complex division");

    TCase *testCase1 = tcase_create("divisionTest");
    TCase *testCase2 = tcase_create("divisionZeroTest");

    tcase_add_test(testCase1, complexDivisionTest);
    tcase_add_test(testCase2, complexDivisonZeroTest);

    suite_add_tcase(suite, testCase1);
    suite_add_tcase(suite, testCase2);
    
    return suite;
}

/*
 * END OF COMPLEX ROOT TEST
 */

/*
 * START OF COMPLEX ROOT TEST
 */

// TODO: implement root
/*
START_TEST(complexRootTest) {
    
    Complex z = { 3.0, 4.0 };

    Complex expected = { 2.0, 1.0 };
    Complex actual = root(z);

    ck_assert_complex_eq(expected, actual)
} END_TEST

START_TEST(complexRootZeroTest) {

    Complex z = { 0.0, 0.0 };

    Complex expected = { 0.0, 0.0};
    Complex actual = root(z);

    ck_assert_complex_eq(expected, actual)
} END_TEST

START_TEST(complexRootImaginaryTest) {

    Complex z = { 0.0, 1.0 };

    Complex expected = { 0.0, 1.0};
    Complex actual = root(z);

    ck_assert_complex_eq(expected, actual)
} END_TEST

Suite *complexRootSuite(void) {

    Suite *suite = suite_create("Complex root");

    TCase *testCase1 = tcase_create("complexRootTest");
    TCase *testCase2 = tcase_create("complexRootZeroTest");
    TCase *testCase3 = tcase_create("complexRootImaginaryTest");

    tcase_add_test(testCase1, complexRootTest);
    tcase_add_test(testCase2, complexRootZeroTest);
    tcase_add_test(testCase3, complexRootImaginaryTest);

    suite_add_tcase(suite, testCase1);
    suite_add_tcase(suite, testCase2);
    suite_add_tcase(suite, testCase3);

    return suite;
}
*/
/*
 * END OF COMPLEX ROOT TESTS
 */

/*
 * START OF COMPLEX POWER TESTS
 */

START_TEST(complexPowerTest) {

    Complex z = { 4.0, 2.0 };
    int pow = 5;

    Complex expected = { -1216.0, 1312.0 };
    OptComplex actual = power(z, pow);

    ck_assert(actual.valid);
    ck_assert_complex_eq(expected, actual.value)
} END_TEST

START_TEST(complexPowerZeroTest) {

    Complex z = { 4.0, 2.0 };
    int pow = 0;

    Complex expected = { 1.0, 0.0 };
    OptComplex actual = power(z, pow);

    ck_assert(actual.valid);
    ck_assert_complex_eq(expected, actual.value)
} END_TEST

START_TEST(complexPowerNegativeTest) {

    Complex z = { 2.0, -3.0 };
    int pow = -3;

    Complex expected = { -(46.0/2197.0), (9.0/2197.0) };
    OptComplex actual = power(z, pow);

    ck_assert(actual.valid);
    ck_assert_complex_eq(actual.value, expected)
} END_TEST

Suite *complexPowerSuite(void) {

    Suite *suite = suite_create("Complex power");

    TCase *testCase1 = tcase_create("complexPowerTest");
    TCase *testCase2 = tcase_create("complexPowerZeroTest");
    TCase *testCase3 = tcase_create("complexPowerNegativeTest");

    tcase_add_test(testCase1, complexPowerTest);
    tcase_add_test(testCase2, complexPowerZeroTest);
    tcase_add_test(testCase3, complexPowerNegativeTest);

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
        complexSubtractionSuite(),
        complexMultiplicationSuite(),
        complexDivisionSuite(),
        complexPowerSuite()
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
