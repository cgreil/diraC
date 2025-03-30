#include <stdio.h>
#include <math.h>

#include "utils/dataArena.h"
#include "qureg/qureg.h"


Arena* arena;

int main() {

#ifdef DEBUG_BUILD
    fprintf(stdout, "Starting debug build \n");
#endif

    arena = arena_init();

    // --- START OF PREPERATION ---

    size_t numQubits = 3;
    QuantumRegister qureg = qureg_new(numQubits); 

    // Create initial Bell basis by entangling qubits 0 and 1
    qureg = qureg_applyHadamard(qureg, 1);
    qureg = qureg_applyCNOT(qureg, 1, 2);

    // create random normalized quantum state in register 1
    // This is done by applying Rx, Ry and Rz gates respectively 
    // with random phases [0, 2*PI]
    double xPhase = ((double) rand() / (double) RAND_MAX) * 2 * M_PI;
    double yPhase = ((double) rand() / (double) RAND_MAX) * 2 * M_PI;
    double zPhase = ((double) rand() / (double) RAND_MAX) * 2 * M_PI;

    qureg = qureg_applyRX(qureg, 0, xPhase);
    qureg = qureg_applyRY(qureg, 0, yPhase);
    qureg = qureg_applyRZ(qureg, 0, zPhase);

    // This is a copy of the first qubit for comparison purposes. 
    // Not that this is not possible in the actual teleporation protocol
    // due to no-cloning theorem
    Vector preparedState = qureg.stateVector;


    // --- START OF PROTOCOL ---

    qureg = qureg_applyCNOT(qureg, 0, 1);
    qureg = qureg_applyHadamard(qureg, 0);

    //measure qubits 0 and 1
    MeasurementResult m1 = { 0 };
    qureg = qureg_applyZMeasurement(qureg, 0, &m1);

    MeasurementResult m2 = { 0 };
    qureg = qureg_applyZMeasurement(qureg, 1, &m2);


    // Correct qubit with index 2 depending on the classical measurement information
    if (m1.measuredValue - 1.0 < 0.00001) {
        qureg = qureg_applyPauliX(qureg, 2);
    }

    if (m2.measuredValue - 1.0 < 0.00001) {
        qureg = qureg_applyPauliZ(qureg, 2);
    }

    // check that the final state is equal to the prepared state 
    fprintf(stdout, "The amplitudes of the randomly prepared qubit were: "
        "(%f + %fi) |0> + (%f + %fi) |1> \n",
        preparedState.dataArray.values[0].re, preparedState.dataArray.values[0].im,
        preparedState.dataArray.values[1].re, preparedState.dataArray.values[1].im
    );

    fprintf(stdout, "The amplitudes of the recovered qubit are: "
        "(%f + %fi)|0> + (%f + %fi) |1> \n",
        qureg.stateVector.dataArray.values[6].re, qureg.stateVector.dataArray.values[6].im,
        qureg.stateVector.dataArray.values[7].re, qureg.stateVector.dataArray.values[7].im
    );

    arena_destroy(arena);

    return 0; 
}