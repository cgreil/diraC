#include <stdio.h>
#include "qureg.h"


bool qureg_applyPauliX(QuantumRegister qureg, size_t target) {

    static Complex pauliXArr[] = {
        (Complex) {0.0, 0.0}, (Complex) {1.0, 0.0},
        (Complex) {1.0, 0.0}, (Complex) {0.0, 0.0}
    };
    Matrix pauliX = matrix_fromRowArray(pauliXArr, 2, 2);

    return qureg_apply1QubitUnitary(qureg, target, pauliX);
}

bool qureg_applyPauliY(QuantumRegister qureg, size_t target) {

    static Complex pauliYArr[] = {
        (Complex) {0.0, 0.0}, (Complex) {0.0, -1.0},
        (Complex) {0.0, 1.0}, (Complex) {0.0, 0.0}
    };
    Matrix pauliY = matrix_fromRowArray(pauliYArr, 2, 2);

    return qureg_apply1QubitUnitary(qureg, target, pauliY);
}

bool qureg_applyPauliZ(QuantumRegister qureg, size_t target) {

    static Complex pauliZArr[] = {
        (Complex) {1.0, 0.0}, (Complex) {0.0, 0.0},
        (Complex) {0.0, 0.0}, (Complex) {-1.0, 0.0}
    };
    Matrix pauliZ = matrix_fromRowArray(pauliZArr, 2, 2);

    return qureg_apply1QubitUnitary(qureg, target, pauliZ);
}

bool qureg_applyHadamard(QuantumRegister qureg, size_t target) {

    Complex hadamardArray[] = {
        (Complex) {(1.0)/(sqrt(2.0)), 0.0}, (Complex) {(1.0)/sqrt(2.0)},
        (Complex) {(1.0)/(sqrt(2.0)), 0.0}, (Complex) {(-1.0)/sqrt(2.0)}
    };
    Matrix hadamard = matrix_fromRowArray(hadamardArray, 2, 2);

    return qureg_apply1QubitUnitary(qureg, target, hadamard);
}

bool qureg_applyT(QuantumRegister qureg, size_t target) {

    double imagComponent = 1 / sin(M_PI_4);
    double realComponent = 1 / cos(M_PI_4);

    Complex tGateArray[] = {
        (Complex) {1.0, 0.0}, (Complex) {0.0, 0.0},
        (Complex) {0.0, 0.0}, (Complex) {realComponent, imagComponent}
    };
    Matrix tGate = matrix_fromRowArray(tGateArray, 2, 2);

    return qureg_apply1QubitUnitary(qureg, target, tGate);
}

bool qureg_applyS(QuantumRegister qureg, size_t target) {

    static Complex sGateArray[] = {
        (Complex) {1.0, 0.0}, (Complex) {0.0, 0.0},
        (Complex) {0.0, 0.0}, (Complex) {0.0, 1.0}
    };
    Matrix sGate = matrix_fromRowArray(sGateArray, 2, 2);

    return qureg_apply1QubitUnitary(qureg, target, sGate);
}

bool qureg_applySWAP(QuantumRegister qureg, size_t target1, size_t target2) {

    Complex cnotGateArray[] = {
        (Complex) {1.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0},
        (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {1.0, 0.0}, (Complex) {0.0, 0.0},
        (Complex) {0.0, 0.0}, (Complex) {1.0, 0.0}, (Complex) {1.0, 0.0}, (Complex) {0.0, 0.0},
        (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {1.0, 0.0},
    };
    Matrix cnotGate = matrix_fromRowArray(cnotGateArray, 4, 4);

    return qureg_apply2QubitUnitary(qureg, target1, target2, cnotGate);
}

bool qureg_applyCNOT(QuantumRegister qureg, size_t control, size_t target) {

    Complex cnotGateArray[] = {
        (Complex) {1.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0},
        (Complex) {0.0, 0.0}, (Complex) {1.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0},
        (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {1.0, 0.0},
        (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {1.0, 0.0}, (Complex) {0.0, 0.0},
    };
    Matrix cnotGate = matrix_fromRowArray(cnotGateArray, 4, 4);

    return qureg_apply2QubitUnitary(qureg, control, target, cnotGate);
}

bool qureg_applyCZ(QuantumRegister qureg, size_t control, size_t target) {

    Complex czGateArray[] = {
        (Complex) {1.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0},
        (Complex) {0.0, 0.0}, (Complex) {1.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0},
        (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {1.0, 0.0}, (Complex) {0.0, 0.0},
        (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {-1.0, 0.0},
    };
    Matrix czGate = matrix_fromRowArray(czGateArray, 4, 4);

    return qureg_apply2QubitUnitary(qureg, control, target, czGate);
}

bool qureg_applyRX(QuantumRegister qureg, size_t target, double phi) {

    const double sinTheta = sin(phi / 2);
    const double cosTheta = cos(phi / 2);

   Complex rxGateArray[] = {
        (Complex) {cosTheta, 0.0}, (Complex) {0.0, - sinTheta},
        (Complex) {0.0, -sinTheta}, (Complex) {cosTheta, 0.0}
    };
   Matrix rxGate = matrix_fromRowArray(rxGateArray, 2, 2);

   return qureg_apply1QubitUnitary(qureg, target, rxGate);
}

bool qureg_applyRY(QuantumRegister qureg, size_t target, double phi) {

    const double sinTheta = phi / 2;
    const double cosTheta = phi / 2;

    Complex ryGateArray[] = {
        (Complex) {cosTheta, 0.0}, (Complex) {0.0, - sinTheta},
        (Complex) {sinTheta, 0.0}, (Complex) {cosTheta, 0.0}
    };
    Matrix ryGate = matrix_fromRowArray(ryGateArray, 2, 2);

    return qureg_apply1QubitUnitary(qureg, target, ryGate);
}

bool qureg_applyRZ(QuantumRegister qureg, size_t target, double phi) {

    const double sinTheta = phi / 2;
    const double cosTheta = phi / 2;

    Complex rzGateArray[] = {
        (Complex) {cosTheta, -sinTheta}, (Complex) {0.0, 0.0},
        (Complex) {0.0, 0.0}, (Complex) {cosTheta, sinTheta}
    };
    Matrix rzGate = matrix_fromRowArray(rzGateArray, 2, 2);

    return qureg_apply1QubitUnitary(qureg, target, rzGate);
}

bool qureg_apply1QubitUnitary(QuantumRegister qureg, size_t target, Matrix gateDefinition) {

    /**
     * This will lead to the most inefficient computation i have ever cause
     * -> Maybe optimize later :(
     * Ideas to look into:
     * - Directly work on amplitudes where possible
     * - Use Kron product decomposition where not possible
     * - Use Gate identities to create gates that can be optimized
     * - Use gate identities to rely on efficient Clifford gates as much as possible
     */
    size_t numAmplitudes = 2 << qureg.numQubits;

    Matrix transformationMatrix = matrix_zeros(2, 2);
    Matrix idMatrix = matrix_identity(2);

    for (size_t qubitIndex = 0; qubitIndex < target - 1; qubitIndex++) {
        transformationMatrix = matrix_kron(transformationMatrix, idMatrix);
    }

    transformationMatrix = matrix_kron(transformationMatrix, gateDefinition);
    for (size_t qubitIndex = target + 1; qubitIndex < qureg.numQubits; qubitIndex++) {
        transformationMatrix = matrix_kron(transformationMatrix, idMatrix);
    }
    qureg.stateVector = vector_matrixMultiplication(qureg.stateVector, transformationMatrix);

    return true;
}

bool qureg_apply2QubitUnitary(QuantumRegister qureg, size_t control, size_t target, Matrix gateDefinition) {

    /**
     * As with the 1qubit case, this is also embarrisngly inefficient.
     * Also, for now, only 2-qubit gates with adjacent control and
     * target qubits are supported
     */

    if (qureg.numQubits < 2) {
        return false;
    }

    if (control - target > 1 || target - control > 1) {
        fprintf(stdout, "Nonadjacent multiqubit gates are not yet supported");
        return false;
    }

    size_t numAmplitudes = 2 << qureg.numQubits;

    Matrix transformationMatrix = matrix_zeros(2, 2);
    Matrix idMatrix = matrix_identity(2);


    /*
     * gateDefinition will be supplied as a 2qubit gate
     * with the assumption that the first qubit is control
     * second qubit is target; If this is not the case,
     * a permutation matrix has to be multiplied to
     * the gate definition
     */
    size_t firstIndex = control;
    size_t secondIndex = firstIndex + 1;
    if (target < control) {
        // TODO: implement permutation of the matrix
        fprintf(stdout, "Multi-qubit gates with targetIndex < controlIndex are not yet supported");
        return false;
        //firstIndex = target;
        //Matrix permutationMatrix = matrix_permutation(4, 1, 0);
    }

    for (size_t qubitIndex = 0; qubitIndex < firstIndex - 1; qubitIndex++) {
        transformationMatrix = matrix_kron(transformationMatrix, idMatrix);
    }

    transformationMatrix = matrix_kron(transformationMatrix, gateDefinition);

    for (size_t qubitIndex = secondIndex + 1; qubitIndex < qureg.numQubits; secondIndex++) {
        transformationMatrix = matrix_kron(transformationMatrix, idMatrix);
    }

    qureg.stateVector = vector_matrixMultiplication(qureg.stateVector, transformationMatrix);

    return true;
}





