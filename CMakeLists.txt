cmake_minimum_required(VERSION 3.28)
project(LEIBNITZ LANGUAGES C VERSION 0.0.1)

#set(CMAKE_C_STANDARD 10)
set(CMAKE_C_FLAGS "-Wall -Werror -Wpedantic")

set(SRC_DIR ${CMAKE_SOURCE_DIR})
# Complex includes
set(
        COMPLEX_SOURCE_FILES
        complex/complex.h
        complex/complex.c
        common/string.h
        common/string.c
        utils/testutils.h
        complex/complexTests.c
        common/vectorSetTests.c
)

set(
        VECTOR_SOURCE_FILES
        complex/complex.h
        complex/complex.c
        common/string.h
        common/string.c
        ndarray/ndarray.h
        ndarray/ndarray.c
        vector/vector.h
        vector/vector.c
        utils/dataArena.h
        utils/dataArena.c
        vector/vectorTests.c
)

set (
        NDARRAY_SOURCE_FILES
        complex/complex.h
        complex/complex.c
        utils/testutils.h
        ndarray/ndarray.c
        ndarray/ndarray.c
        ndarray/ndArrayTests.c
        utils/dataArena.c
        utils/dataArena.h
)

set (
        VECTORSET_SOURCE_FILES
        common/doubleLinkedList.h
        common/doubleLinkedList.c
        common/vectorSet.h
        common/vectorSet.c
        common/vectorSetTests.c
        common/vectorSetTests.c
        vector/vector.h
        vector/vector.c
        common/vectorSet.c
)


add_executable(LEIBNITZ main.c
        common/types.h
        complex/complex.c
        float/float.c
        ndarray/ndarray.c
        utils/testutils.h
        vector/vector.h
        vector/vector.c
        common/vectorSet.h
        common/doubleLinkedList.h
        common/doubleLinkedList.c
        utils/dataArena.c
        utils/dataArena.h
        common/string.c
        common/string.h
        matrix/matrix.h
        matrix/matrix.c
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

#include Source files
include_directories(SYSTEM ${SRC_DIR})
#include directory for check header file
include_directories("../../libs/check/src")
#include check and math library
target_link_libraries(${COMPLEX_TESTS} m check)

target_link_libraries(${VECTOR_TESTS} m check)
target_link_libraries(${VECTORSET_TESTS} m check)