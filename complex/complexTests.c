#include <stdio.h>
#include <stdlib.h>
#include <check.h>

#include "complex.h"


/**
 * Testfile containing tests for complex number operations using
 * the check library
 *
 *
 *
 */

START_TEST(testComplexAdd) {
    
    Complex z1 = {0.0, 1.0};
    Complex z2 = {1.0, 0.0};

    Complex expected = {1.0, 1.0};
    Complex actual = add(z1, z2);

    ck_assert_float_eq(expected.re, actual.re);
    ck_assert_float_eq(expected.im, actual.im);
} END_TEST



Suite * complexAddSuite(void) {
    Suite *suite;
    TCase *testCase;

    suite = suite_create("Complex Addition");
    
    testCase = tcase_create("addition_simple_case");
    tcase_add_test(testCase, testComplexAdd);


    suite_add_tcase(suite, testCase);

    return suite;

}



int main() {

    int numFailedTests;
    Suite *suite;
    SRunner *suiteRunner;
    
    suite = complexAddSuite();
    suiteRunner = srunner_create(suite);

    srunner_run_all(suiteRunner, CK_NORMAL);
    numFailedTests = srunner_ntests_failed(suiteRunner);
    srunner_free(suiteRunner);

    return (numFailedTests == 0) ? EXIT_SUCCESS : EXIT_FAILURE;



    return 0;
}
