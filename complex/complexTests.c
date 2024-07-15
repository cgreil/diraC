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
}
END_TEST


int main() {
    return 0;
}
