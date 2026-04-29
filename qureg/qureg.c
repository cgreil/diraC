#include "qureg.h"
#include "logging/Logger.h"
#include "common/string.h"
#include "complex/complex.h"
#include "vector/vector.h"

QuantumRegister qureg_new(size_t numQubits) {

    if (numQubits == 0) {
        return (QuantumRegister) { 0 };
    }
    size_t numCoefficients = (1 << numQubits);

    Vector coeffVector = vector_zeros(numCoefficients);
    // To achieve |000...0> vector, set very first coefficient
    // to 1:
    vector_setElement(coeffVector, 0, (Complex) { 1.0, 0.0 });

    return (QuantumRegister) {
        .numQubits = numQubits,
        .stateVector = coeffVector
    };
}


void qureg_printFullMeasurement(QuantumRegister qureg) {

    // Use the Born rule to simulate measurement in the 
    // computational basis where the measurement probability
    // of getting |n> when measuring |psi> is |<psi|n>|^2
    
    // for n qubits, N = 2^n basis states
    size_t N = (1 << qureg.numQubits);

    for(size_t i = 0; i < N; i++) {
    
        Complex prob_amplitude = vector_getElement(qureg.stateVector, i).value;
        double probability = pow(complex_modulus(prob_amplitude), 2);
        
        String basisVec = string_binaryFromUInt(i);

        LOG_INFO(LOGOBJ("|"),
                 LOGOBJ(basisVec),
                 LOGOBJ("> : "));
        printf("%f\n", probability);
    }
}


double qureg_calculateFidelity(QuantumRegister qureg1, QuantumRegister qureg2) {
    
    Complex prob_amplitude = vector_innerProduct(qureg1.stateVector, qureg2.stateVector).value;
    return pow(complex_modulus(prob_amplitude), 2);
}
