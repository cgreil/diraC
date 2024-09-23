//
// Created by christoph on 16.08.24.
//

#include <check.h>
#include <stdio.h>
#include <time.h>

#include "vector/vector.h"
#include "common/vectorSet.h"
#include "utils/testutils.h"
#include "utils/dataArena.h"
#include "ndarray/ndarray.h"

static Arena *arena;

START_TEST (createVectorSetTest) {

   Vector vectors[] = {
       vector_zeros(arena, 4),
       vector_zeros(arena, 5),
       vector_ones(arena, 6),
       vector_ones(arena, 7),
   };

   size_t numVectors = sizeof(vectors) / sizeof(Vector);
   VectorSet vectorSet = vectorSet_fromArray(vectors, numVectors);

    ck_assert(vectorSet.numVectors == numVectors);

    for (size_t i = 0; i < vectorSet.numVectors; i++) {
        ck_assert_vector_eq(vectors[i], vectorSet_getVectorAtIndex(vectorSet, i).data);
    }

    ck_assert(vectorSet_destroy(vectorSet));

} END_TEST


START_TEST (emptyVectorSetTest) {

   VectorSet vectorSet = vectorSet_createEmptySet();

   ck_assert_uint_eq(vectorSet.numVectors, 0);

   // Make sure functions behave as intended on empty vector set
   OptVector vectorResult = vectorSet_getVectorAtIndex(vectorSet, 0);
   ck_assert(!vectorResult.isValid);
   // by default, all elements of Vector should be 0
   ck_assert_ndarray_eq(vectorResult.data.dataArray, NDArray_create(0, 0, 0, NULL))

   bool removalResult = vectorSet_removeVectorAtIndex(vectorSet, 0);
   ck_assert(!removalResult);

} END_TEST


START_TEST(constructVectorSetTest) {

    VectorSet vectorSet = vectorSet_createEmptySet();
    size_t numValues = 4;
    size_t numRows = 1;

    Complex valuesVec1[] = {
            (Complex) {.re = 0.0, .im = 0.0},
            (Complex) {.re = 1.0, .im = 4.0},
            (Complex) {.re =8.0, .im = 2.0},
            (Complex) {.re = 2.333, .im = 3.0}
    };

    Vector vector1 = {
        .dataArray = (NDArray) {
            .numColumns = numValues,
            .numRows = numRows,
            .values = valuesVec1
        }
    };

    Vector vector2 = vector_clone(arena, vector1);
    vector_scaleINP(vector2, (Complex) { .re = 2.0, .im = 0.0 });

    Vector vector3 = vector_clone(arena, vector1);
    vector_scaleINP(vector3, (Complex) { .re = 0.0, .im = 1.0 });

    vectorSet_addVector(vectorSet, vector1);
    vectorSet_addVector(vectorSet, vector2);
    vectorSet_addVector(vectorSet, vector3);

    ck_assert(vectorSet_size(vectorSet) == 3);
} END_TEST

Suite *vectorSetSuite(void) {

    Suite *suite;
    suite = suite_create("vectorSetTestSuite");

    TCase *testcase1 = tcase_create("createVectorSetTest");
    TCase *testcase2 = tcase_create("emptyVectorSetTest");
    TCase *testcase3 = tcase_create("constructVectorSetTest");

    tcase_add_test(testcase1, createVectorSetTest);
    tcase_add_test(testcase2, emptyVectorSetTest);
    tcase_add_test(testcase3, constructVectorSetTest);

    suite_add_tcase(suite, testcase1);
    suite_add_tcase(suite, testcase2);
    suite_add_tcase(suite, testcase3);

    return suite;
}

int main(void) {

    arena = arena_init();

    Suite *testSuites[] = {
        vectorSetSuite()
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
}
