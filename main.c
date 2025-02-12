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


    arena_destroy(arena);

    return EXIT_SUCCESS;
}
