//
// Created by christoph on 16.08.24.
//

#include "vector/vector.h"
Vector vector_Zeros(size_t vecSize) {
    return (Vector) {};
}

Vector vector_Ones(size_t vecSize) {
    return (Vector) {};
}

Vector vector_Copy(Vector vector) {
    return (Vector) {};
}

Vector vector_fromArray(size_t vecSize, Complex *complexArray) {
    return (Vector) {};
}

Complex vector_getElement(Vector vector, size_t index){
    return (Complex) {};
}

OptVector vector_addition(Vector vec1, Vector vec2) {
    return (OptVector) {};
}

Complex vector_innerProduct(Vector vec1, Vector vec2) {
    return (Complex) {};
}

void vector_scaleINP(Vector vec1, Complex factor) {

}

void vector_conjugateINP(Vector vec) {

}

void vector_transposeINP(Vector vec) {

}

void vector_adjointINP(Vector vec) {

}

void vector_resize(Vector vec, size_t newLength) {

}

bool vector_isColumn(Vector vec) {
    return false;
}


