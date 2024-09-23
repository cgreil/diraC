cmake_minimum_required(VERSION 3.28)
project(LEIBNITZ LANGUAGES C VERSION 0.0.1)

#set(CMAKE_C_STANDARD 10)
set(CMAKE_C_FLAGS "-Wall -Werror -Wpedantic")

set(SRC_DIR ${CMAKE_SOURCE_DIR})
# Complex includes
set(
        COMPLEX_SOURCE_FILES
        complex/complex.c
        common/string.c
        complex/complexTests.c
        vector/vector.c
        ndarray/ndarray.c
        utils/dataArena.c
)

set(
        VECTOR_SOURCE_FILES
        complex/complex.c
        common/string.c
        vector/vector.c
        utils/dataArena.c
        vector/vectorTests.c
        ndarray/ndarray.c
        utils/testutils.h
)

set (
        NDARRAY_SOURCE_FILES
        common/string.c
        complex/complex.c
        ndarray/ndarray.c
        ndarray/ndarray.c
        ndarray/ndArrayTests.c
        utils/dataArena.c
)

set (
        VECTORSET_SOURCE_FILES
        common/doubleLinkedList.c
        complex/complex.c
        common/vectorSet.c
        utils/dataArena.c
        common/vectorSetTests.c
        vector/vector.c
        common/vectorSet.c
        ndarray/ndarray.c
        common/string.c
)

set (
        MATRIX_SOURCE_FILES
        common/doubleLinkedList.c
        common/vectorSet.c
        common/string.c
        utils/dataArena.c
        complex/complex.c
        ndarray/ndarray.c
        vector/vector.c
        matrix/matrix.c
        matrix/matrixTests.c
)

add_executable(LEIBNITZ main.c
        complex/complex.c
        float/float.c
        ndarray/ndarray.c
        vector/vector.c
        common/doubleLinkedList.c
        utils/dataArena.c
        common/string.c
        matrix/matrix.c
        common/tuple.c
)



# Complex tests
set(COMPLEX_TESTS "complexTests")
add_executable(${COMPLEX_TESTS} ${COMPLEX_SOURCE_FILES})

set (NDARRAY_TESTS "ndarrayTests")
add_executable(${NDARRAY_TESTS} ${NDARRAY_SOURCE_FILES})
target_link_libraries(${NDARRAY_TESTS} m check)

set (VECTOR_TESTS "vectorTests")
add_executable(${VECTOR_TESTS} ${VECTOR_SOURCE_FILES})

set(VECTORSET_TESTS "vectorSetTests")
add_executable(${VECTORSET_TESTS} ${VECTORSET_SOURCE_FILES})

set(MATRIX_TESTS "matrixTests")
add_executable(${MATRIX_TESTS} ${MATRIX_SOURCE_FILES})

#include Source files
include_directories(SYSTEM ${SRC_DIR})
#include directory for check header file
include_directories("../../libs/check/src")
#include check and math library
target_link_libraries(${COMPLEX_TESTS} m check)

target_link_libraries(${MATRIX_TESTS} m check)

target_link_libraries(${VECTOR_TESTS} m check)
target_link_libraries(${VECTORSET_TESTS} m check)