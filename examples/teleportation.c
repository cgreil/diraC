#include <stdio.h>
#include <math.h>

#include "utils/dataArena.h"
#include "qureg/qureg.h"
#include "logging/Logger.h"

#define LOG_OUTPUT STDOUT 

#define UNUSED(x) (void) (x)

// declare globally scoped structs
Arena* arena;
Logger* logger;

int main() {


    arena = arena_init();
    logger = logger_init(LOG_OUTPUT);

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
    
    // silence compiler warning
    UNUSED(preparedState);

    LOG_INFO(LOGOBJ("Initial prepared state: \n"));
    LOG_INFO(LOGOBJ(preparedState));
    
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

    
    logger_destroy(logger);
    arena_destroy(arena);

    return 0; 
}