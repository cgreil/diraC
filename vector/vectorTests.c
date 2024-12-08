#include <stdlib.h>
#include <check.h>
#include <stdio.h>
#include <time.h>

#include "vector/vector.h"
#include "ndarray/ndarray.h"
#include "utils/testutils.h"

Arena *arena;


START_TEST(testVectorZeros) {

    size_t vecSize = 8;
    Vector zeroVector = vector_zeros(vecSize);
    for (size_t i = 0; i < vecSize; i++) {
        ck_assert_complex_eq(vector_getElement(zeroVector, i).value, ((Complex) {0.0, 0.0}))
    }

} END_TEST

START_TEST(testVectorOnes) {

    size_t vecSize = 8;
    Vector oneVector = vector_ones(vecSize);
    for (size_t i = 0; i< vecSize; i++) {
        ck_assert_complex_eq(vector_getElement(oneVector, i).value, ((Complex) {1.0, 0.0}))
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

    Vector vector = vector_fromArray(values, vecSize);

    for(size_t i = 0; i < vecSize; i++) {
        ck_assert_complex_eq(vector_getElement(vector, i).value, values[i])
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

    Vector vector1 = vector_fromArray(values1, vecSize);
    Vector vector2 = vector_fromArray(values2, vecSize);

    OptVector result = vector_addition(vector1, vector2);
    ck_assert(result.isValid);
    Vector resultVector = result.data;

    for (size_t i = 0; i < vecSize; i++) {
        Complex expected = complex_addition(values1[i], values2[i]);

        ck_assert_complex_eq(expected, vector_getElement(resultVector, i).value)
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

    Vector vector1 = vector_fromArray(data1, sizeVec1);
    Vector vector2 = vector_fromArray(data2, sizeVec2);

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

    Vector vector = vector_fromArray(originalValues, vecSize);

    Complex scaledValues[] = {
        (Complex) {6.0, -9.0},
        (Complex) {12.0, 27.0},
        (Complex) {-3.0, 12.0}
    };
    Vector expected = vector_fromArray(scaledValues, vecSize);

    Vector resultVector = vector_scaleINP(vector, scaleFactor);
    ck_assert_vectorValues_eq(expected, resultVector);

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

    Vector vector1 = vector_fromArray(values1, vecSize);
    Vector vector2 = vector_fromArray(values2, vecSize);

    Complex expectedValue = {39.3, -25.4};
    Complex actualValue = vector_innerProduct(vector1, vector2).value;

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

    Vector vector = vector_fromArray(values, vecSize);
    Vector expectedVector = vector_fromArray(expectedValues, vecSize);

    Vector resultVector = vector_conjugateINP(vector);

    ck_assert_vectorValues_eq(expectedVector, resultVector)
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

    Vector vector = vector_fromArray(values, vecSize);

    ck_assert_int_eq(vector.dataArray.numRows, vecSize);
    ck_assert_int_eq(vector.dataArray.numColumns, 1);

    Vector resultVector = vector_transposeINP(vector);

    ck_assert_int_eq(resultVector.dataArray.numRows, 1);
    ck_assert_int_eq(resultVector.dataArray.numColumns, vecSize);
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

    Vector vector = vector_fromArray(values, vecSize);
    ck_assert_int_eq(vector.dataArray.numRows, vecSize);
    ck_assert_int_eq(vector.dataArray.numColumns, 1);

    Vector resultVector = vector_adjointINP(vector);
    ck_assert_int_eq(resultVector.dataArray.numRows, 1);
    ck_assert_int_eq(resultVector.dataArray.numColumns, vecSize);

    Vector expectedVector = vector_fromArray(expectedValues, vecSize);

    ck_assert_vectorValues_eq(expectedVector, resultVector);
}END_TEST

START_TEST(vectorNormTest) {

    size_t numValues = 4;
    Complex values[] = {
        (Complex) {1.0, 2.0},
        (Complex) {3.0, 4.0},
        (Complex) {0.5, -2.0},
        (Complex) {-5.0, 8.0}
    };

    Vector vector = vector_fromArray(values, numValues);

    Vector normalizedVector = vector_normalize(vector);
    Complex norm = vector_norm(normalizedVector);

    ck_assert_float_eq(norm.re, 1.0);
    ck_assert_float_eq(norm.im, 0.0);

}END_TEST

START_TEST(vectorResizeEnlargeTest) {

    size_t initialNumValues = 4;
    size_t newNumValues = 8;

    Vector initialVector = vector_ones(initialNumValues);

    Vector resizedVector = vector_resize(initialVector, newNumValues);

    for (size_t i = 0; i < initialNumValues; i++) {
        ck_assert_complex_eq(((Complex) {1.0, 0.0}), vector_getElement(resizedVector, i).value);
    }

    for (size_t j = initialNumValues; j < newNumValues; j++) {
        ck_assert_complex_eq(((Complex) {0.0, 0.0}), vector_getElement(resizedVector, j).value);
    }
} END_TEST

START_TEST(vectorResizeShrinkTest) {

    size_t initialNumValues = 8;
    size_t newNumValues = 4;

    Vector initialVector = vector_ones(initialNumValues);
    Vector resizedVector = vector_resize(initialVector, newNumValues);

    for (size_t i = 0; i < newNumValues; i++) {
        ck_assert_complex_eq(((Complex) {1.0, 0.0}), vector_getElement(resizedVector, i).value);
    }

    ck_assert_int_eq(resizedVector.size, newNumValues);

} END_TEST

START_TEST(vectorResizeInvariantTest) {

    size_t initialNumValues = 5;
    size_t newNumValues = 5;

    Vector initialVector = vector_ones(initialNumValues);
    Vector resizedVector = vector_resize(initialVector, newNumValues);

    ck_assert_vectorValues_eq(initialVector, resizedVector);
    ck_assert_int_eq(initialVector.size, resizedVector.size);
} END_TEST

START_TEST(vectorEqualPositiveTest) {

    Complex arrayValues[] = {
        (Complex) { 1.0, 1.0 },
        (Complex) { 1.0, 1.0 },
        (Complex) { 0.0, -1.0}
    };

    Complex nearValues[] = {
        (Complex) { 0.99999999, 0.99999999},
        (Complex) { 0.99999999, 0.99999999},
        (Complex) { 0.00000001, -0.99999999}
    };

    size_t arraySize = sizeof(arrayValues) / sizeof(arrayValues[0]);
    Vector vector = vector_fromArray(arrayValues, arraySize);

    size_t nearVectorSize = sizeof(nearValues) / sizeof(nearValues[0]);
    Vector nearVector = vector_fromArray(nearValues, nearVectorSize);

    bool test = vector_equal(vector, nearVector);
    ck_assert(test);

} END_TEST

START_TEST(vectorEqualNegativeTest) {

    Complex arrayValues[] = {
        (Complex) { 2.0, 8.0 },
        (Complex) { -1.0, 1.44}
    };

    Complex nearValue[] = {
        (Complex) { 2.0001, 8.0 },
        (Complex) { -1.0, 1.44 }
    };

    size_t arraySize = sizeof(arrayValues) / sizeof(arrayValues[0]);

    Vector vector = vector_fromArray(arrayValues, arraySize);
    Vector nearVector = vector_fromArray(nearValue, arraySize);

    ck_assert(!(vector_equal(vector, nearVector)));

} END_TEST

START_TEST(vectorEqualSizeDiffTest) {

    Complex arrayValues[] = {
        (Complex) { 4.0, 2.0 },
        (Complex) { -2.44, 1.39}
    };

    Complex nearValues[] = {
        (Complex) {4.0, 2.0},
        (Complex) {-2.44, 1.39},
        (Complex) {1.0, 2.9}
    };

    size_t arraySize = sizeof(arrayValues) / sizeof(arrayValues[0]);
    size_t nearSize = sizeof(nearValues) / sizeof(nearValues[0]);

    Vector vector = vector_fromArray(arrayValues, arraySize);
    Vector nearVector = vector_fromArray(nearValues, nearSize);

    ck_assert(!(vector_equal(vector, nearVector)));
} END_TEST

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
    TCase *testcase8 = tcase_create("vectorNormTest");
    TCase *testcase9 = tcase_create("vectorResizeEnlargeTest");
    TCase *testcase10 = tcase_create("vectorResizeShrinkTest");
    TCase *testcase11 = tcase_create("vectorResizeInvariantTest");
    TCase *testcase12 = tcase_create("vectorEqualPositiveTest");
    TCase *testcase13 = tcase_create("vectorEqualNegativeTest");
    TCase *testcase14 = tcase_create("vectorEqualSizeTest");

    tcase_add_test(testcase1, vectorAdditionTest);
    tcase_add_test(testcase2, vectorAdditionInvalidDimsTest);
    tcase_add_test(testcase3, vectorScalingTest);
    tcase_add_test(testcase4, vectorInnerProductTest);
    tcase_add_test(testcase5, vectorConjugateTest);
    tcase_add_test(testcase6, vectorTransposeTest);
    tcase_add_test(testcase7, vectorAdjointTest);
    tcase_add_test(testcase8, vectorNormTest);
    tcase_add_test(testcase9, vectorResizeEnlargeTest);
    tcase_add_test(testcase10, vectorResizeShrinkTest);
    tcase_add_test(testcase11, vectorResizeInvariantTest);
    tcase_add_test(testcase12, vectorEqualPositiveTest);
    tcase_add_test(testcase13, vectorEqualNegativeTest);
    tcase_add_test(testcase14, vectorEqualSizeDiffTest);

    suite_add_tcase(suite, testcase1);
    suite_add_tcase(suite, testcase2);
    suite_add_tcase(suite, testcase3);
    suite_add_tcase(suite, testcase4);
    suite_add_tcase(suite, testcase5);
    suite_add_tcase(suite, testcase6);
    suite_add_tcase(suite, testcase7);
    suite_add_tcase(suite, testcase8);
    suite_add_tcase(suite, testcase8);
    suite_add_tcase(suite, testcase9);
    suite_add_tcase(suite, testcase10);
    suite_add_tcase(suite, testcase11);
    suite_add_tcase(suite, testcase12);
    suite_add_tcase(suite, testcase13);
    suite_add_tcase(suite, testcase14);

    return suite;
}

/**
 * END OF ARITHMETIC TESTS
 */

/**
 * TESTSUITE REGISTRATION
 */

int main(void) {

    arena = arena_init();

    Suite *testSuites[] = {
        vectorCreationSuite(),
        vectorArithmeticSuite()
    };

    size_t numSuites = sizeof(testSuites) / sizeof(testSuites[0]);

    SRunner *suiteRunner;
    for(size_t i = 0; i < numSuites; i++) {

        fprintf(stdout, "Running suite (%zu/%zu) \n", i+1, numSuites);

        Suite *currentSuite = testSuites[i];
        suiteRunner = srunner_create(currentSuite);

        //Start timing
        time_t startTime = time(NULL);
        srunner_run_all(suiteRunner, CK_NORMAL);
        time_t endTime = time(NULL);

        int numFailed = srunner_ntests_failed(suiteRunner);
        fprintf(stdout, "Finished with %d failed tests in %f seconds \n", numFailed, difftime(endTime, startTime));
        srunner_free(suiteRunner);

    }

    arena_destroy(arena);

    return 0;
}





