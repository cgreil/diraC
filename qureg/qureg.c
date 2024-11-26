#include "qureg.h"


QuantumRegister qureg_new(size_t numQubits) {

    if (numQubits == 0) {
        return (QuantumRegister) { 0 };
    }
    size_t numCoefficients = (2 << numQubits);

    Vector coeffVector = vector_zeros(numCoefficients);
    // To achieve |000...0> vector, set very last coefficient
    // to 1:
    vector_setElement(coeffVector, numCoefficients, (Complex) { 1.0, 0.0 });

    return (QuantumRegister) {
        .numQubits = numQubits,
        .stateVector = coeffVector
    };
}
