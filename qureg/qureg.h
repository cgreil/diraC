#ifndef QUREG_H_
#define QUREG_H_


#include "vector/vector.h"
#include "matrix/matrix.h"

typedef struct {
    Vector stateVector;
    size_t numQubits;
}QuantumRegister;

typedef struct {
    Matrix gateMatrix;
}Gate;

/**
 * Creates a QuantumRegister, i.e. a statvector representing n qubits using 2^n
 * complex amplitudes. As the statevector representation is used, only pure states
 * are possible.
 *
 *
 */
QuantumRegister qureq_new(size_t numQubits);

bool qureg_applyPauliX(QuantumRegister qureg, size_t target);

bool qureg_applyPauliY(QuantumRegister qureg, size_t target);

bool qureg_applyPauliZ(QuantumRegister qureg, size_t target);

bool qureg_applyHadamard(QuantumRegister qureg, size_t target);

bool qureg_applyT(QuantumRegister qureg, size_t target);

bool qureg_applyS(QuantumRegister qureg, size_t target);

bool qureg_applySWAP(QuantumRegister qureg, size_t target1, size_t target2);

bool qureg_applyCNOT(QuantumRegister qureg, size_t control, size_t target);

bool qureg_applyCZ(QuantumRegister qureg, size_t control, size_t target);

bool qureg_applyRX(QuantumRegister qureg, size_t target, double phi);

bool qureg_applyRY(QuantumRegister qureg, size_t target, double phi);

bool qureg_applyRZ(QuantumRegister qureg, size_t target, double phi);

bool qureg_apply1QubitUnitary(QuantumRegister qureg, size_t target, Matrix gateDefinition);

bool qureg_apply2QubitUnitary(QuantumRegister qureg, size_t control, size_t target, Matrix gateDefinition);







#endif
