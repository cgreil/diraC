#ifndef TESTUTILS_H_
#define TESTUTILS_H_

#include <stdbool.h>

#include "complex.h"
#include "ndarray/ndarray.h"

/*
 * Test Utils
 */

#define ck_assert_complex_eq(expected, actual) \
    ck_assert_float_eq((expected).re, (actual).re); \
    ck_assert_float_eq((expected).im, (actual).im);

#define ck_assert_ndarray_eq(expectedNDArray, actualNDArray)                                  \
    for (int i = 0; i < (expectedNDArray).numRows; i++) {                                \
        for (int j = 0; j < (expectedNDArray).numColumns; j++) {                         \
                OptComplex expVal = NDArray_getElement((expectedNDArray), i, j);            \
                OptComplex actualVal = NDArray_getElement((actualNDArray), i, j);               \
                ck_assert(expVal.valid);                                                      \
                ck_assert(actualVal.valid);                                                                              \
                ck_assert_complex_eq((expVal.value), (actualVal.value));                         \
            }                                                                   \
    }

#define ck_assert_vector_eq(expectedVector, actualVector) \
    ck_assert((expectedVector).size == (actualVector).size);                                                      \
    for (size_t i = 0; i < (expectedVector).size; i++) {  \
        ck_assert_complex_eq(vector_getElement((expectedVector), i).value, vector_getElement((actualVector), i).value)  \
    }

#define ck_assert_matrix_eq(expectedMatrix, actualMatrix)   \
    NDArray expectedData = (expectedMatrix).dataArray;      \
    NDArray actualData = (actualMatrix).dataArray;          \
    ck_assert_int_eq((expectedMatrix).numRows, (actualMatrix).numRows); \
    ck_assert_int_eq((expectedMatrix).numColumns, (actualMatrix).numColumns); \
    ck_assert_ndarray_eq(expectedData, actualData);                      \


#define ck_assert_vectorSet_eq(expectedSet, actualSet)                                                                 \
    ck_assert_uint_eq((expectedSet).numVectors, (actualSet).numVectors);                                                   \
    for (size_t i = 0; i < (expectedSet).numVectors; i++) {                                                              \
        expVector = vectorSet_getVectorAtIndex((expectedSet), i);                                                        \
        actVector = vectorSet_getVectorAtIndex((actualSet), i);                                                          \
        ck_assert_vector_eq(expVector, actVector)                                                                      \
    }

#endif