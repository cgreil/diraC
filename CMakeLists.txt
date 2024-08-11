cmake_minimum_required(VERSION 3.13)

project(LEIBNITZ)

set(SRC_DIR ${CMAKE_SOURCE_DIR})
set(CHECK_DIR ../../libs/check/src)


add_executable(${PROJECT_NAME} ${SRC_DIR}/main.c)

# Complex includes
set(
        COMPLEX_SOURCE_FILES
        complex/complex.h
        complex/complex.c
        utils/testutils.h
        complex/complexTests.c
)

set (
        NDARRAY_SOURCE_FILES
        complex/complex.h
        complex/complex.c
        utils/testutils.h
        ndarray/ndarray.c
        ndarray/ndarray.c
        ndarray/ndArrayTests.c
)


# Complex tests
set(COMPLEX_TESTS "complexTests")
add_executable(${COMPLEX_TESTS} ${COMPLEX_SOURCE_FILES})

set (NDARRAY_TESTS "ndarrayTests")
add_executable(${NDARRAY_TESTS} ${NDARRAY_SOURCE_FILES})


#include Source files
include_directories(${SRC_DIR})
#include directory for check header file
include_directories("../../libs/check/src")
#include check and math library
target_link_libraries(${COMPLEX_TESTS} m check)
target_link_libraries(${NDARRAY_TESTS} m check)
