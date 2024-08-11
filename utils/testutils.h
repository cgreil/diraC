#ifndef TESTUTILS_H_
#define TESTUTILS_H_

#include <stdbool.h>
#include <complex.h>
#include <ndarray/ndarray.h>

/*
 * Test Utils
 */

#define ck_assert_complex_eq(expected, actual) \
    ck_assert_float_eq((expected).re, (actual).re); \
    ck_assert_float_eq((expected).im, (actual).im);

#define ck_assert_ndarray_eq(expectedNDArray, actualNDArray)                                  \
    for (int i = 0; i < expectedNDArray.numRows; i++) {                                \
        for (int j = 0; j < expectedNDArray.numColumns; j++) {                         \
                Complex expVal = NDArray_getElement(expectedNDArray, i, j);            \
                Complex actualVal = NDArray_getElement(actualNDArray, i, j);           \
                ck_assert_complex_eq(expVal, actualVal)                         \
                                                                                \
            }                                                                   \
    }

#define ck_assert_vector_eq(expectedVector, actualVector) \
    NDArray expectedData = expectedVector.dataArray;\
    NDArray actualData = actualVector.dataArray;    \
    ck_assert_ndarray_eq(expectedData, actualData) \


#endif