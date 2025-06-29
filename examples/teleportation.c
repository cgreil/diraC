#include <stdio.h>
#include <math.h>

#include "utils/dataArena.h"
#include "qureg/qureg.h"
#include "logging/Logger.h"

#define LOG_OUTPUT STDOUT 

// declare globally scoped structs
Arena* arena;
Logger* logger;

int main() {

    arena = arena_init();
    logger = logger_init(LOG_OUTPUT);

    // --- START OF PREPERATION ---

    size_t numQubits = 3;
    QuantumRegister qureg = qureg_new(numQubits); 
    // Set random seed
    srand(2000);

    // Create initial Bell basis by entangling qubits 1 and 2
    qureg = qureg_applyHadamard(qureg, 1);
    qureg = qureg_applyCNOT(qureg, 1, 2);

    // create random normalized quantum state in register 1
    // This is done by applying Rx, Ry and Rz gates respectively 
    // with random phases [0, 2*PI]
    /*double xPhase = ((double) rand() / (double) RAND_MAX) * 2 * M_PI;
    double yPhase = ((double) rand() / (double) RAND_MAX) * 2 * M_PI;
    double zPhase = ((double) rand() / (double) RAND_MAX) * 2 * M_PI;
    */
    double xPhase = 1.503123;
    double yPhase = 0.825345;
    double zPhase = 4.857364;    

    fprintf(stdout, "The phases are x: %f, y: %f, z: %f \n", xPhase, yPhase, zPhase);

    qureg = qureg_applyRX(qureg, 0, xPhase);
    qureg = qureg_applyRY(qureg, 0, yPhase);
    qureg = qureg_applyRZ(qureg, 0, zPhase);
    

    // This is a copy of the first qubit for comparison purposes. 
    // Not that this is not possible in  a physical implemetation of the teleporation protocol
    // due to no-cloning theorem

    Vector preparedState = qureg.stateVector;
    
    LOG_INFO(
        LOGOBJ("Initial prepared state:\n"),
        LOGOBJ(preparedState),
        LOGOBJ("\n")
    );
    
    // --- START OF PROTOCOL ---

    qureg = qureg_applyCNOT(qureg, 0, 1);
    qureg = qureg_applyHadamard(qureg, 0);

    //measure qubits 0 and 1
    MeasurementResult m0 = { 0 };
    qureg = qureg_applyZMeasurement(qureg, 0, &m0);

    MeasurementResult m1 = { 0 };
    qureg = qureg_applyZMeasurement(qureg, 1, &m1);


    // Correct qubit with index 2 depending on the classical measurement information
    if (m0.measuredValue - 1.0 < 0.00001) {
        qureg = qureg_applyPauliZ(qureg, 2);
    }

    if (m1.measuredValue - 1.0 < 0.00001) {
        qureg = qureg_applyPauliX(qureg, 2);
    }

    LOG_INFO(
        LOGOBJ("Recovered state after traversing protocol:\n"),
        LOGOBJ(qureg.stateVector),
        LOGOBJ("\n")
    );

    
    logger_destroy(logger);
    arena_destroy(arena);

    return 0; 
}