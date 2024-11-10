#ifndef TESTUTILS_H_
#define TESTUTILS_H_

#include <stdbool.h>

#include "complex.h"
#include "ndarray/ndarray.h"

/*
 * Test Utils
 */

#define ck_assert_complex_eq(expected, actual)                                                                          \
    ck_assert_float_eq((expected).re, (actual).re);                                                                     \
    ck_assert_float_eq((expected).im, (actual).im);

#define ck_assert_ndarray_eq(expectedNDArray, actualNDArray)                                                            \
    for (int _vecRowIndex = 0; _vecRowIndex < (expectedNDArray).numRows; _vecRowIndex++) {                              \
        for (int _vecColIndex = 0; _vecColIndex < (expectedNDArray).numColumns; _vecColIndex++) {                       \
                OptComplex expVal = NDArray_getElement((expectedNDArray), _vecRowIndex, _vecColIndex);                  \
                OptComplex actualVal = NDArray_getElement((actualNDArray), _vecRowIndex, _vecColIndex);                 \
                ck_assert(expVal.valid);                                                                                \
                ck_assert(actualVal.valid);                                                                             \
                ck_assert_complex_eq((expVal.value), (actualVal.value));                                                \
            }                                                                                                           \
    }

#define ck_assert_vectorValues_eq(expectedVector, actualVector)                                                         \
    ck_assert((expectedVector).size == (actualVector).size);                                                            \
    for (size_t _vecIndex = 0; _vecIndex < (expectedVector).size; _vecIndex++) {                                        \
        ck_assert_complex_eq(vector_getElement((expectedVector), _vecIndex).value,                                      \
        vector_getElement((actualVector), _vecIndex).value)                                                             \
    }

#define ck_assert_matrix_eq(expectedMatrix, actualMatrix)                                                               \
    NDArray expectedData = (expectedMatrix).ndArray;                                                                  \
    NDArray actualData = (actualMatrix).ndArray;                                                                      \
    ck_assert_int_eq((expectedMatrix).numRows, (actualMatrix).numRows);                                                 \
    ck_assert_int_eq((expectedMatrix).numColumns, (actualMatrix).numColumns);                                           \
    ck_assert_ndarray_eq(expectedData, actualData)                                                                      \


#define ck_assert_vectorSet_eq(expectedSet, actualSet)                                                                  \
    ck_assert_uint_eq((expectedSet).numVectors, (actualSet).numVectors);                                                \
    for (size_t _vecSetIndex = 0; _vecSetIndex < (expectedSet).numVectors; _vecSetIndex++) {                            \
        expVector = vectorSet_getVectorAtIndex((expectedSet), _vecSetIndex);                                            \
        actVector = vectorSet_getVectorAtIndex((actualSet), _vecSetIndex);                                              \
        ck_assert_vectorValues_eq(expVector, actVector)                                                                 \
    }

#endif