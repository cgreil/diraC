#include <stdlib.h>
#include <utils/testutils.h>

#include "vector.h"
#include "check.h"

START_TEST(testVectorZeros) {

    size_t vecSize = 8;
    Vector zeroVector = vector_Zeros(vecSize);
    for (size_t i = 0; i < vecSize; i++) {
        ck_assert_complex_eq(vector_getElement(zeroVector, i), ((Complex) {0.0, 0.0}))
    }

} END_TEST

START_TEST(testVectorOnes) {

    size_t vecSize = 8;
    Vector oneVector = vector_Ones(vecSize);
    for (size_t i = 0; i< vecSize; i++) {
        ck_assert_complex_eq(vector_getElement(oneVector, i), ((Complex) {1.0, 0.0}))
    }
} END_TEST

START_TEST(testVectorFromArray) {

    size_t vecSize = 4;
    Complex values[] = {
        (Complex) {2.0, 4.0},
        (Complex) {8.0, 9.0},
        (Complex) {-3.0, 2.0},
        (Complex) {0.0, -9.0}
    };

    Vector vector = vector_fromArray(vecSize, values);

    for(size_t i = 0; i < vecSize; i++) {
        ck_assert_complex_eq(vector_getElement(vector, i), values[i])
    }

} END_TEST

Suite *vectorCreationSuite(void) {

    Suite *suite;
    suite = suite_create("vectorCreationSuite");

    TCase *testcase1 = tcase_create("testVectorZeroCreation");
    TCase *testcase2 = tcase_create("testVectorOneCreation");
    TCase *testcase3 = tcase_create("testVectorFromArray");

    tcase_add_test(testcase1, testVectorZeros);
    tcase_add_test(testcase2, testVectorOnes);
    tcase_add_test(testcase3, testVectorFromArray);

    suite_add_tcase(suite, testcase1);
    suite_add_tcase(suite, testcase2);
    suite_add_tcase(suite, testcase3);

    return suite;
}

/**
* END OF VECTOR CREATION TESTS
*/

/**
* START OF VECTOR ARITHMATIC TESTS
*/

START_TEST(vectorAdditionTest) {

    size_t vecSize = 4;
    Complex values1[] = {
        (Complex) {2.0, 4.0},
        (Complex) {8.0, 9.0},
        (Complex) {-3.0, 2.0},
        (Complex) {0.0, -9.0}
    };

    Complex values2[] = {
        (Complex) {4.0, 8.0},
        (Complex) {0.0, 19.0},
        (Complex) {-322.0, 2.0},
        (Complex) {7.0, -2.0}
    };

    Vector vector1 = vector_fromArray(vecSize, values1);
    Vector vector2 = vector_fromArray(vecSize, values2);

    OptVector result = vector_addition(vector1, vector2);
    ck_assert(result.isValid);
    Vector resultVector = result.data;

    for (size_t i = 0; i < vecSize; i++) {
        Complex expected = add(values1[i], values2[i]);

        ck_assert_complex_eq(expected, vector_getElement(resultVector, i))
    }
} END_TEST

START_TEST(vectorAdditionInvalidDimsTest) {

    size_t sizeVec1 = 3;
    size_t sizeVec2 = 2;

    Complex data1[] = {
        (Complex) {2.0, 3.0},
        (Complex) {6.0, 1.0},
        (Complex) {9.0, 1.0}
    };

    Complex data2[] = {
        (Complex) {4.0, 2.0},
        (Complex) {9.0, 9.0}
    };

    Vector vector1 = vector_fromArray(sizeVec1, data1);
    Vector vector2 = vector_fromArray(sizeVec2, data2);

    OptVector result = vector_addition(vector1, vector2);
    ck_assert(!result.isValid);

} END_TEST

START_TEST(vectorScalingTest) {

    size_t vecSize = 3;
    Complex scaleFactor= {3.0, 0.0};

    Complex originalValues[] = {
        (Complex) {2.0, -3.0 },
        (Complex) {4.0, 9.0},
        (Complex) {-1.0, 4.0}
    };

    Vector vector = vector_fromArray(vecSize, originalValues);

    Complex scaledValues[] = {
        (Complex) {6.0, -18.0},
        (Complex) {12.0, 27.0},
        (Complex) {-3.0, 12.0}
    };
    Vector expected = vector_fromArray(vecSize, scaledValues);

    vector_scaleINP(vector, scaleFactor);
    ck_assert_vector_eq(expected, vector);

} END_TEST

START_TEST(vectorInnerProductTest) {

    size_t vecSize = 3;

    Complex values1[] = {
        (Complex) {2.0, 3.0},
        (Complex) {3.0, -1.0},
        (Complex) {9.0, -2.0}
    };

    Complex values2[] = {
        (Complex) {19.0, 1.0},
        (Complex) {2.0, 2.0},
        (Complex) {1.1, 7.8}
    };

    Vector vector1 = vector_fromArray(vecSize, values1);
    Vector vector2 = vector_fromArray(vecSize, values2);

    Complex expectedValue = {68.5, 131};
    Complex actualValue = vector_innerProduct(vector1, vector2);

    ck_assert_complex_eq(expectedValue, actualValue)
} END_TEST

START_TEST(vectorConjugateTest) {

    size_t vecSize = 3;

    Complex values[] = {
        (Complex) {2.0, 3.2},
        (Complex) {3.0, -1.1},
        (Complex) {9.0, -2.9}
    };

    Complex expectedValues[] = {
        (Complex) {2.0, -3.2},
        (Complex) {3.0, 1.1},
        (Complex) {9.0, 2.9}
    };

    Vector vector = vector_fromArray(vecSize, values);
    Vector expectedVector = vector_fromArray(vecSize, expectedValues);

    vector_conjugateINP(vector);

    ck_assert_vector_eq(expectedVector, vector)
}END_TEST

START_TEST(vectorTransposeTest) {

    // Upon first creation, vector of size n will be a column vector
    // i.e. will have dimensions n x 1

    size_t vecSize = 3;
    Complex values[] = {
        (Complex) {19.0, 1.0},
        (Complex) {2.0, 2.0},
        (Complex) {1.1, 7.8}
    };

    Vector vector = vector_fromArray(vecSize, values);

    ck_assert_int_eq(vector.dataArray.numRows, vecSize);
    ck_assert_int_eq(vector.dataArray.numColumns, 1);

    vector_transposeINP(vector);

    ck_assert_int_eq(vector.dataArray.numRows, 1);
    ck_assert_int_eq(vector.dataArray.numColumns, vecSize);
} END_TEST

START_TEST(vectorAdjointTest) {

    size_t vecSize = 3;
    Complex values[] = {
        (Complex) {19.0, 1.0},
        (Complex) {2.0, 2.0},
        (Complex) {1.1, 7.8}
    };

    Complex expectedValues[] = {
        (Complex) {19.0, -1.0},
        (Complex) {2.0, -2.0},
        (Complex) {1.1, -7.8}
    };

    Vector vector = vector_fromArray(vecSize, values);
    ck_assert_int_eq(vector.dataArray.numRows, vecSize);
    ck_assert_int_eq(vector.dataArray.numColumns, 1);

    vector_adjointINP(vector);
    ck_assert_int_eq(vector.dataArray.numRows, 1);
    ck_assert_int_eq(vector.dataArray.numColumns, vecSize);

    Vector expectedVector = vector(vecSize, expectedValues);

    ck_assert_vector_eq(expectedVector, vector);
}END_TEST


Suite *vectorArithmeticSuite(void) {

    Suite *suite;
    suite = suite_create("vectorArithmeticSuite");

    TCase *testcase1 = tcase_create("vectorAdditionTest");
    TCase *testcase2 = tcase_create("vectorAdditionInvalidDimsTest");
    TCase *testcase3 = tcase_create("vectorScalingTest");
    TCase *testcase4 = tcase_create("vectorInnerProductTest");
    TCase *testcase5 = tcase_create("vectorConjugateTest");
    TCase *testcase6 = tcase_create("vectorTransposeTest");
    TCase *testcase7 = tcase_create("vectorAdjointTest");

    tcase_add_test(testcase1, vectorAdditionTest);
    tcase_add_test(testcase2, vectorAdditionInvalidDimsTest);
    tcase_add_test(testcase3, vectorScalingTest);
    tcase_add_test(testcase4, vectorInnerProductTest);
    tcase_add_test(testcas5, vectorConjugateTest);
    tcase_add_test(testcase6, vectorTransposeTest);
    tcase_add_test(testcase7, vectorAdjointTest);

    suite_add_tcase(suite, testcase1);
    suite_add_tcase(suite, testcase2);
    suite_add_tcase(suite, testcase3);
    suite_add_tcase(suite, testcase4);
    suite_add_tcase(suite, testcase5);
    suite_add_tcase(suite, testcase6);
    suite_add_tcase(suite, testcase7);

    return suite;
}

/**
 * END OF ARITHMETIC TESTS
 */



