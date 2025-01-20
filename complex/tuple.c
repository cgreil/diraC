//
// Created by christoph on 24.08.24.
//

#include <utils/dataArena.h>

#include "complex/tuple.h"

extern Arena *arena;

ComplexTuple complexTuple_new(size_t numElements) {

//     Complex* elements = arena_allocate_count(arena, Complex, numElements);

//     return (ComplexTuple) {
//         .data = elements,
//         .numElements = numElements,
// 
    return (ComplexTuple) { 0 };
}

