#ifndef QUREG_H_
#define QUREG_H_


#include "vector/vector.h"
#include "matrix/matrix.h"

typedef struct {
    Vector stateVector;
    size_t numQubits;
} QuantumRegister;


// a struct to encapsulate the result of the measurement
// after the collapse of the wavefunction
typedef struct {
    double measuredValue;
    size_t measuredQubitIndex;
} MeasurementResult;


/**
 * Creates a QuantumRegister, i.e. a statvector representing n qubits using 2^n
 * complex amplitudes. As the statevector representation is used, only pure states
 * are possible.
 *
 *
 */
QuantumRegister qureg_new(size_t numQubits);

QuantumRegister qureg_applyPauliX(QuantumRegister qureg, size_t target);

QuantumRegister qureg_applyPauliY(QuantumRegister qureg, size_t target);

QuantumRegister qureg_applyPauliZ(QuantumRegister qureg, size_t target);

QuantumRegister qureg_applyHadamard(QuantumRegister qureg, size_t target);

QuantumRegister qureg_applyT(QuantumRegister qureg, size_t target);

QuantumRegister qureg_applyS(QuantumRegister qureg, size_t target);

QuantumRegister qureg_applySWAP(QuantumRegister qureg, size_t target1, size_t target2);

QuantumRegister qureg_applyCNOT(QuantumRegister qureg, size_t control, size_t target);

QuantumRegister qureg_applyCZ(QuantumRegister qureg, size_t control, size_t target);

QuantumRegister qureg_applyRX(QuantumRegister qureg, size_t target, double phi);

QuantumRegister qureg_applyRY(QuantumRegister qureg, size_t target, double phi);

QuantumRegister qureg_applyRZ(QuantumRegister qureg, size_t target, double phi);

QuantumRegister qureg_apply1QubitUnitary(QuantumRegister qureg, size_t target, Matrix gateDefinition);

QuantumRegister qureg_apply2QubitUnitary(QuantumRegister qureg, size_t control, size_t target, Matrix gateDefinition);

QuantumRegister qureg_applyZMeasurement(QuantumRegister qureg, size_t target, MeasurementResult* measurementResult);







#endif
