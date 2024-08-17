//
// Created by christoph on 16.08.24.
//

#include <check.h>

#include "vector/vector.h"
#include "common/vectorSet.h"
#include "utils/testutils.h"

START_TEST (createVectorSetTest) {

   Vector vectors[] = {
       vector_Zeros(4),
       vector_Ones(5),
       vector_Zeros(6),
       vector_Ones(7),
   };

   size_t numVectors = sizeof(vectors) / sizeof(Vector);
   VectorSet vectorSet = vectorSet_fromArray(vectors, numVectors);

    ck_assert(vectorSet.numVectors == numVectors);

    for (size_t i = 0; i < vectorSet.numVectors; i++) {
        ck_assert_vector_eq(vectors[i], vectorSet_getVectorAtIndex(vectorSet, i));
    }

    ck_assert(vectorSet_destroy(vectorSet));

} END_TEST


START_TEST (emptyVectorSetTest) {

   VectorSet vectorSet = vectorSet_createEmptySet();

   ck_assert_uint_eq(vectorSet.numVectors, 0);

   // Make sure functions behave as intended on empty vector set
   OptVector vectorResult = vectorSet_getVectorAtIndex(vectorSet, 0);
   ck_assert(!vectorResult.isValid)
   // by default, all elements of Vector should be 0
   ck_assert(vectorResult.data.size = 0)
   ck_assert_ndarray_eq(vectorResult.data.dataArray, NDArray_create(0, {}, 0, NULL))

   bool removalResult = vectorSet_removeVectorAtIndex(vectorSet, 0);
   ck_assert(!removalResult)

} END_TEST


START_TEST(constructVectorSetTest) {

    VectorSet vectorSet = vectorSet_createEmptySet();

    vectorSet_addVector(vectorSet, (Vector) {
            .dataArray = (NDArray) {
            .numColumns = 4,
            .numRows = 1,
            .values = {
                (Complex) {.re = 0.0, .im = 0.0 }
            }
        }
    }



}