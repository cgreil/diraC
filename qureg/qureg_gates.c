#include "qureg.h"


bool qureg_applyPauliX(QuantumRegister qureg, size_t target) {

    static Complex pauliXArr[] = {
        (Complex) {0.0, 0.0}, (Complex) {1.0, 0.0},
        (Complex) {1.0, 0.0}, (Complex) {0.0, 0.0}
    };
    Matrix pauliX = matrix_fromRowArray(pauliXArr, 2, 2);

    size_t numAmplitudes = 2 << qureg.numQubits;

    Matrix transformationMatrix = matrix_zeros(2, 2);
    Matrix idMatrix = matrix_identity(2);

    for (size_t qubitCount = 0; qubitCount < target - 1; qubitCount++) {
        transformationMatrix = matrix_kron(transformationMatrix, idMatrix);
    }
    transformationMatrix = matrix_kron(transformationMatrix, pauliX);
    for (size_t qubitCount = target + 1; qubitCount < qureg.numQubits; qubitCount++) {
        transformationMatrix = matrix_kron(transformationMatrix, idMatrix);
    }

    qureg.stateVector = vector_matrixMultiplication(qureg.stateVector, transformationMatrix);
    return true;
}

bool qureg_applyPauliY(QuantumRegister qureg, size_t target);

bool qureg_applyPauliZ(QuantumRegister qureg, size_t target);

bool qureg_applyHadamard(QuantumRegister qureg, size_t target);

bool qureg_applyT(QuantumRegister qureg, size_t target);

bool qureg_applyS(QuantumRegister qureg, size_t target);

bool qureg_applyCNOT(QuantumRegister qureg, size_t control, size_t target);

bool qureg_applyRX(QuantumRegister qureg, size_t target, double phi);

bool qureg_applyRY(QuantumRegister qureg, size_t target, double phi);

bool qureg_applyRZ(QuantumRegister qureg, size_t target, double phi);

bool qureg_apply1QubitUnitary(QuantumRegister qureg, size_t target, Matrix gateDefinition);

bool qureg_apply2QubitUnitary(QuantumRegister qureg, size_t control, size_t target, Matrix gateDefinition);





