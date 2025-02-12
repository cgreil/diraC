#include <stdlib.h>
#include <matrix/matrix.h>
#include <common/string.h>
#include <assert.h>

#include "utils/dataArena.h"
#include "qureg/qureg.h"

Arena *arena;





int main(void) {

    // Initialize arena
    arena = arena_init();

    size_t numQubits = 4;
    QuantumRegister quantumRegister = qureg_new(numQubits);

    qureg_applyPauliX(quantumRegister, 0);
    qureg_applyPauliY(quantumRegister, 1);
    qureg_applyPauliZ(quantumRegister, 2);
    qureg_applyHadamard(quantumRegister, 3);
    qureg_applyCNOT(quantumRegister, 1, 2);

    StringBuilder *stringBuilder = stringBuilder_create();

    stringBuilder_appendCharArray(stringBuilder, "Statevector: \n", 20);
    stringBuilder_appendVector(stringBuilder, quantumRegister.stateVector);


    String string = stringBuilder_build(stringBuilder);
    // Use fwrite over fprintf to print beyond zero (null terminator) bytes
    fwrite(string.data, sizeof(char), string.length, stdout);


    // Testing stuff
    /* // identity matrix
    Matrix sigma_0 = matrix_fromRowArray(
        (Complex[]) {
            (Complex) {1.0, 0.0},
            (Complex) {0.0, 0.0},
            (Complex) {0.0, 0.0},
            (Complex) {1.0, 0.0}},
        2,
        2
    );

    // pauli x gate
    Matrix sigma_1 = matrix_fromRowArray(
        (Complex[]) {
            (Complex) {0.0, 0.0},
            (Complex) {1.0, 0.0},
            (Complex) {1.0, 0.0},
            (Complex) {0.0, 0.0}},
        2,
        2
    );

    // pauli y gate
    Matrix sigma_2 = matrix_fromRowArray(
        (Complex[]) {
            (Complex) {0.0, 0.0},
            (Complex) {0.0, -1.0},
            (Complex) {0.0, 1.0},
            (Complex) {0.0, 0.0}},
        2,
        2
    );

    // pauli z gate
    Matrix sigma_3 = matrix_fromRowArray(
        (Complex[]) {
            (Complex) {1.0, 0.0},
            (Complex) {0.0, 0.0},
            (Complex) {0.0, 0.0},
            (Complex) {0.0, 0.0}},
        2,
        2
    );

    assert(sigma_0.numRows != 0);
    assert(sigma_1.numRows != 0);
    assert(sigma_2.numRows != 0);
    assert(sigma_3.numRows != 0);


    StringBuilder *stringBuilder = stringBuilder_create();

    stringBuilder_appendMatrix(stringBuilder, sigma_0);
    stringBuilder_appendCharArray(stringBuilder, "\n", 1);

    stringBuilder_appendMatrix(stringBuilder, sigma_1);
    stringBuilder_appendCharArray(stringBuilder, "\n", 1);

    stringBuilder_appendMatrix(stringBuilder, sigma_2);
    stringBuilder_appendCharArray(stringBuilder, "\n", 1);

    stringBuilder_appendMatrix(stringBuilder, sigma_3);
    stringBuilder_appendCharArray(stringBuilder, "\n", 1);

    String string = stringBuilder_build(stringBuilder);
    // Use fwrite over fprintf to print beyond zero (null terminator) bytes
    fwrite(string.data, sizeof(char), string.length, stdout);


    // clean up
    stringBuilder_destroy(stringBuilder); */



    arena_destroy(arena);

    return EXIT_SUCCESS;
}
