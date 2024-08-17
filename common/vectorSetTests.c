//
// Created by christoph on 16.08.24.
//

#include <check.h>

#include "vector/vector.h"
#include "common/vectorSet.h"

START_TEST (createVectorSetTest) {



   Vector vectors[] = {
       vector_Zeros(4),
       vector_Ones(5),
       vector_Zeros(6),
       vector_Ones(7),
   };

   size_t numVectors = sizeof(vectors) / sizeof(Vector);
   VectorSet vectorSet = vectorSet_create(vectors, sizeof(numVectors));

} END_TEST