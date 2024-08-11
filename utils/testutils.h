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

#define ck_assert_ndarray_eq(expected, actual)                                  \
    for (int i = 0; i < expected.numRows; i++) {                                \
        for (int j = 0; j < expected.numColumns; j++) {                         \
                Complex expVal = NDArray_getElement(expected, i, j);            \
                Complex actualVal = NDArray_getElement(actual, i, j);           \
                ck_assert_complex_eq(expVal, actualVal)                         \
                                                                                \
            }                                                                   \
    }




#endif