cmake_minimum_required(VERSION 3.28)
project(DIRAC LANGUAGES C VERSION 0.0.1)

#set(CMAKE_C_STANDARD 10)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_C_FLAGS "-Wall -Werror -Wpedantic -Debug")
set(CMAKE_C_FLAGS_DEBUG "-O0 -ggdb3")

set(SRC_DIR ${CMAKE_SOURCE_DIR})
# Complex includes
set(
        COMPLEX_SOURCE_FILES
        utils/dataArena.c
        complex/complex.c
        complex/tuple.c
        common/string.c
        vector/vector.c
        vector/vectorCollection.c
        common/doubleLinkedList.c
        matrix/matrix.c
        ndarray/ndarray.c
        complex/complexTests.c
)

set(
        VECTOR_SOURCE_FILES
        complex/complex.c
        complex/tuple.c
        common/string.c
        vector/vector.c
        vector/vectorCollection.c
        common/doubleLinkedList.c
        matrix/matrix.c
        utils/dataArena.c
        vector/vectorTests.c
        ndarray/ndarray.c
        utils/testutils.h
)

set (
        NDARRAY_SOURCE_FILES
        common/string.c
        complex/complex.c
        complex/tuple.c
        ndarray/ndarray.c
        vector/vector.c
        vector/vectorCollection.c
        common/doubleLinkedList.c
        matrix/matrix.c
        ndarray/ndArrayTests.c
        utils/dataArena.c
)

set (
        VECTORCOLLECTION_SOURCE_FILES
        matrix/matrix.c
        common/doubleLinkedList.c
        complex/complex.c
        complex/tuple.c
        vector/vectorCollection.c
        utils/dataArena.c
        vector/vectorCollectionTests.c
        vector/vector.c
        ndarray/ndarray.c
        common/string.c
)

set (
        MATRIX_SOURCE_FILES
        matrix/matrix.c
        common/doubleLinkedList.c
        vector/vectorCollection.c
        common/string.c
        utils/dataArena.c
        complex/complex.c
        complex/tuple.c
        ndarray/ndarray.c
        vector/vector.c
        matrix/matrixTests.c
)

set (
        QUREG_SOURCE_FILES
        matrix/matrix.c
        common/doubleLinkedList.c
        vector/vectorCollection.c
        common/string.c
        utils/dataArena.c
        complex/complex.c
        complex/tuple.c
        ndarray/ndarray.c
        vector/vector.c
        qureg/qureg.c
        qureg/qureg_gates.c
        qureg/quregTests.c
)

add_executable(DIRAC main.c
        complex/complex.c
        float/float.c
        ndarray/ndarray.c
        vector/vector.c
        common/doubleLinkedList.c
        vector/vectorCollection.c
        utils/dataArena.c
        common/string.c
        matrix/matrix.c
        complex/tuple.c
        logging/Logger.h
        logging/Logger.c
        qureg/qureg.c
        qureg/qureg_gates.c
)


add_executable(TELEPORTATION 
        utils/dataArena.c
        examples/teleportation.c
        complex/complex.c
        float/float.c
        ndarray/ndarray.c
        vector/vector.c
        common/doubleLinkedList.c
        vector/vectorCollection.c
        common/string.c
        matrix/matrix.c
        complex/tuple.c
        logging/Logger.h
        logging/Logger.c
        qureg/qureg.c
        qureg/qureg_gates.c
)



# Complex tests
set(COMPLEX_TESTS "complexTests")
add_executable(${COMPLEX_TESTS} ${COMPLEX_SOURCE_FILES})

set (NDARRAY_TESTS "ndarrayTests")
add_executable(${NDARRAY_TESTS} ${NDARRAY_SOURCE_FILES})

set (VECTOR_TESTS "vectorTests")
add_executable(${VECTOR_TESTS} ${VECTOR_SOURCE_FILES})

set(VECTORCOLLECTION_TESTS "vectorCollectionTests")
add_executable(${VECTORCOLLECTION_TESTS} ${VECTORCOLLECTION_SOURCE_FILES})

set(MATRIX_TESTS "matrixTests")
add_executable(${MATRIX_TESTS} ${MATRIX_SOURCE_FILES})

set (QUREG_TESTS "quregTests")
add_executable(${QUREG_TESTS} ${QUREG_SOURCE_FILES})

#include Source files
include_directories(SYSTEM ${SRC_DIR})
#include directory for check header file
include_directories("../../libs/check/src")
#include check and math library
target_link_libraries(${COMPLEX_TESTS} m check)
target_link_libraries(${NDARRAY_TESTS} m check)
target_link_libraries(${MATRIX_TESTS} m check)
target_link_libraries(${VECTOR_TESTS} m check)
target_link_libraries(${VECTORCOLLECTION_TESTS} m check)
target_link_libraries(${QUREG_TESTS} m check)


# Include math file for main
target_link_libraries(${PROJECT_NAME} m)

target_link_libraries(TELEPORTATION m)

#add_compile_options(-DDEBUG)
add_definitions(-DDEBUG_BUILD)
