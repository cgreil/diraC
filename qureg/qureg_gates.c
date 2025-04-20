#include <stdio.h>

#include "qureg/qureg.h"
#include "logging/Logger.h"


static Matrix expandMatrixToQuregSize(QuantumRegister qureg, Matrix initialMatrix, size_t initialMatrixSize, size_t initialMatrixTarget) {

    // TODO: expand for usage when initialMatrixSize != 1
    if (initialMatrixSize != 1) {
        fprintf(stderr, "InitialMatrixSize not equal to 1 is not yet supported in expandMatrixToQuregSize \n");
        return (Matrix) { 0 };
    };


    Matrix resultMatrix;
    if (initialMatrixTarget == 0){
        // If the target is in the first qubit, start with the gateDefinition
        // as first element in the Pauli String
        resultMatrix = initialMatrix;
    } 
    else {
        resultMatrix = matrix_identity(2);
    }
     
    size_t qubitCounter = 1;
    while (qubitCounter < qureg.numQubits) {
        Matrix matrix;
        if (qubitCounter == initialMatrixTarget) {
            matrix = initialMatrix;
        }
        else {
            matrix = matrix_identity(2);
        }
        resultMatrix = matrix_kron(resultMatrix, matrix); 
        qubitCounter++;
    }  

    return resultMatrix;

}

QuantumRegister qureg_applyPauliX(QuantumRegister qureg, size_t target) {

    Complex pauliXArr[] = {
        (Complex) {0.0, 0.0}, (Complex) {1.0, 0.0},
        (Complex) {1.0, 0.0}, (Complex) {0.0, 0.0}
    };
    Matrix pauliX = matrix_fromRowArray(pauliXArr, 2, 2);

    return qureg_apply1QubitUnitary(qureg, target, pauliX);
}

QuantumRegister qureg_applyPauliY(QuantumRegister qureg, size_t target) {

    Complex pauliYArr[] = {
        (Complex) {0.0, 0.0}, (Complex) {0.0, -1.0},
        (Complex) {0.0, 1.0}, (Complex) {0.0, 0.0}
    };
    Matrix pauliY = matrix_fromRowArray(pauliYArr, 2, 2);

    return qureg_apply1QubitUnitary(qureg, target, pauliY);
}

QuantumRegister qureg_applyPauliZ(QuantumRegister qureg, size_t target) {

    Complex pauliZArr[] = {
        (Complex) {1.0, 0.0}, (Complex) {0.0, 0.0},
        (Complex) {0.0, 0.0}, (Complex) {-1.0, 0.0}
    };
    Matrix pauliZ = matrix_fromRowArray(pauliZArr, 2, 2);

    return qureg_apply1QubitUnitary(qureg, target, pauliZ);
}

QuantumRegister qureg_applyHadamard(QuantumRegister qureg, size_t target) {

    Complex hadamardArray[] = {
        (Complex) {(1.0)/(sqrt(2.0)), 0.0}, (Complex) {(1.0)/sqrt(2.0)},
        (Complex) {(1.0)/(sqrt(2.0)), 0.0}, (Complex) {(-1.0)/sqrt(2.0)}
    };
    Matrix hadamard = matrix_fromRowArray(hadamardArray, 2, 2);

    return qureg_apply1QubitUnitary(qureg, target, hadamard);
}

QuantumRegister qureg_applyT(QuantumRegister qureg, size_t target) {

    double imagComponent = 1 / sin(M_PI_4);
    double realComponent = 1 / cos(M_PI_4);

    Complex tGateArray[] = {
        (Complex) {1.0, 0.0}, (Complex) {0.0, 0.0},
        (Complex) {0.0, 0.0}, (Complex) {realComponent, imagComponent}
    };
    Matrix tGate = matrix_fromRowArray(tGateArray, 2, 2);

    return qureg_apply1QubitUnitary(qureg, target, tGate);
}

QuantumRegister qureg_applyS(QuantumRegister qureg, size_t target) {

    Complex sGateArray[] = {
        (Complex) {1.0, 0.0}, (Complex) {0.0, 0.0},
        (Complex) {0.0, 0.0}, (Complex) {0.0, 1.0}
    };
    Matrix sGate = matrix_fromRowArray(sGateArray, 2, 2);

    return qureg_apply1QubitUnitary(qureg, target, sGate);
}

QuantumRegister qureg_applySWAP(QuantumRegister qureg, size_t target1, size_t target2) {

    Complex cnotGateArray[] = {
        (Complex) {1.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0},
        (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {1.0, 0.0}, (Complex) {0.0, 0.0},
        (Complex) {0.0, 0.0}, (Complex) {1.0, 0.0}, (Complex) {1.0, 0.0}, (Complex) {0.0, 0.0},
        (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {1.0, 0.0},
    };
    Matrix cnotGate = matrix_fromRowArray(cnotGateArray, 4, 4);

    return qureg_apply2QubitUnitary(qureg, target1, target2, cnotGate);
}

QuantumRegister qureg_applyCNOT(QuantumRegister qureg, size_t control, size_t target) {

    Complex cnotGateArray[] = {
        (Complex) {1.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0},
        (Complex) {0.0, 0.0}, (Complex) {1.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0},
        (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {1.0, 0.0},
        (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {1.0, 0.0}, (Complex) {0.0, 0.0},
    };
    Matrix cnotGate = matrix_fromRowArray(cnotGateArray, 4, 4);

    return qureg_apply2QubitUnitary(qureg, control, target, cnotGate);
}

QuantumRegister qureg_applyCZ(QuantumRegister qureg, size_t control, size_t target) {

    Complex czGateArray[] = {
        (Complex) {1.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0},
        (Complex) {0.0, 0.0}, (Complex) {1.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0},
        (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {1.0, 0.0}, (Complex) {0.0, 0.0},
        (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {0.0, 0.0}, (Complex) {-1.0, 0.0},
    };
    Matrix czGate = matrix_fromRowArray(czGateArray, 4, 4);

    return qureg_apply2QubitUnitary(qureg, control, target, czGate);
}

QuantumRegister qureg_applyRX(QuantumRegister qureg, size_t target, double phi) {

    const double sinTheta = sin(phi / 2);
    const double cosTheta = cos(phi / 2);

   Complex rxGateArray[] = {
        (Complex) {cosTheta, 0.0}, (Complex) {0.0, - sinTheta},
        (Complex) {0.0, -sinTheta}, (Complex) {cosTheta, 0.0}
    };
   Matrix rxGate = matrix_fromRowArray(rxGateArray, 2, 2);

   return qureg_apply1QubitUnitary(qureg, target, rxGate);
}

QuantumRegister qureg_applyRY(QuantumRegister qureg, size_t target, double phi) {

    const double sinTheta = sin(phi / 2);
    const double cosTheta = cos(phi / 2);

    Complex ryGateArray[] = {
        (Complex) {cosTheta, 0.0}, (Complex) {-sinTheta, 0.0},
        (Complex) {sinTheta, 0.0}, (Complex) {cosTheta, 0.0}
    };
    Matrix ryGate = matrix_fromRowArray(ryGateArray, 2, 2);

    return qureg_apply1QubitUnitary(qureg, target, ryGate);
}

QuantumRegister qureg_applyRZ(QuantumRegister qureg, size_t target, double phi) {

    const double sinTheta = sin(phi / 2);
    const double cosTheta = cos(phi / 2);

    Complex rzGateArray[] = {
        (Complex) {cosTheta, -sinTheta}, (Complex) {0.0, 0.0},
        (Complex) {0.0, 0.0}, (Complex) {cosTheta, sinTheta}
    };
    Matrix rzGate = matrix_fromRowArray(rzGateArray, 2, 2);

    return qureg_apply1QubitUnitary(qureg, target, rzGate);
}

QuantumRegister qureg_apply1QubitUnitary(QuantumRegister qureg, size_t target, Matrix gateDefinition) {

    if (target >= qureg.numQubits) {
        fprintf(stderr, "Supplied Qubit operation has a target qubit outside of the supplied register \n");
        return (QuantumRegister) { 0 };
    }

    if (!vector_isNormalized(qureg.stateVector)) {
        LOG_ERROR(LOGOBJ("[qureg_apply1QubitUnitary] Precondition Error: Vector not normalized"));
    }
    /**
     * This will lead to the most inefficient computation i have ever caused
     * -> Maybe optimize later :(
     * Ideas to look into:
     * - Directly work on amplitudes where possible
     * - Use Kron product decomposition where not possible
     * - Use Gate identities to create gates that can be optimized
     * - Use gate identities to rely on efficient Clifford gates as much as possible
     */

     
    Matrix transformationMatrix = expandMatrixToQuregSize(qureg, gateDefinition, 1, target);
    
    LOG_INFO(
        LOGOBJ("[qureg_apply1QubitUnitary] Vector before matrix application:"),
        LOGOBJ(qureg.stateVector),
        LOGOBJ("[qureg_apply1QubitUnitary] Matrix to apply: "),
        LOGOBJ(transformationMatrix)
    );

    qureg.stateVector = vector_matrixMultiplication(qureg.stateVector, transformationMatrix);

    LOG_INFO(
        LOGOBJ("[qureg_apply1QubitUnitary] Vector after matrix application:"),
        LOGOBJ(qureg.stateVector)
    );

    if (!vector_isNormalized(qureg.stateVector)) {
        LOG_ERROR(LOGOBJ("[qureg_apply1QubitUnitary] Postcondition error: Vector not normalized"));
    }

   return qureg;
}

QuantumRegister qureg_apply2QubitUnitary(QuantumRegister qureg, size_t control, size_t target, Matrix gateDefinition) {

    /**
     * As with the 1qubit case, this is also embarrisngly inefficient.
     * Also, for now, only 2-qubit gates with adjacent control and
     * target qubits are supported
     */

    if (qureg.numQubits < 2 || control >= qureg.numQubits || target >= qureg.numQubits) {
        fprintf(stderr, "The supplied quantum register of size %zu cannot fit a gate"
            "with control index %zu and target index %zu \n", qureg.numQubits, control, target);
        return (QuantumRegister) { 0 };
    }

    if (abs((int) control - (int) target) > 1) {
        fprintf(stderr, "Nonadjacent multiqubit gates are not yet supported \n");
        return (QuantumRegister) { 0 };
    }

    /*
     * gateDefinition will be supplied as a 2qubit gate
     * with the assumption that the first qubit is control
     * second qubit is target; If this is not the case,
     * a permutation matrix has to be multiplied to
     * the gate definition
     */ 
    if (target < control) {
        // TODO: implement permutation of the matrix
        fprintf(stderr, "Multi-qubit gates with targetIndex < controlIndex are not yet supported \n");
        return (QuantumRegister) { 0 };
    }

    Matrix transformationMatrix;
    size_t qubitCounter = 0;
    // If target index corresponds to the first qubit, immediately use 
    // the gate definition
    if (control == 0) {
        transformationMatrix = gateDefinition;
        qubitCounter += 2;
    } else {
        transformationMatrix = matrix_identity(2);
        qubitCounter++;
    }

    Matrix idMatrix = matrix_identity(2);
    while(qubitCounter < qureg.numQubits) {
        if (control == qubitCounter) {
            transformationMatrix = matrix_kron(transformationMatrix, gateDefinition);
            qubitCounter += 2;
        } 
        else {
            transformationMatrix = matrix_kron(transformationMatrix, idMatrix);
            qubitCounter++;
        }
    }

    if (!vector_isNormalized(qureg.stateVector)) {
        LOG_ERROR(LOGOBJ("[qureg_apply2QubitUnitary] Postcondition error: Vector not normalized"));
    }

    LOG_INFO(
        LOGOBJ("[qureg_apply2QubitUnitary] Vector before matrix application:"),
        LOGOBJ(qureg.stateVector),
        LOGOBJ("[qureg_apply2QubitUnitary] Matrix to apply: "),
        LOGOBJ(transformationMatrix)
    );

    qureg.stateVector = vector_matrixMultiplication(qureg.stateVector, transformationMatrix);

    LOG_INFO(
        LOGOBJ("[qureg_apply2QubitUnitary] Vector after matrix application:"),
        LOGOBJ(qureg.stateVector)
    );

    if (!vector_isNormalized(qureg.stateVector)) {
        LOG_ERROR(LOGOBJ("[qureg_apply2QubitUnitary] Postcondition error: Vector not normalized"));
    }

    return qureg;
}

QuantumRegister qureg_applyZMeasurement(QuantumRegister qureg, size_t target, MeasurementResult* measurementResult) {

    // Apply a projective Measurement for the Z basis onto the qubit at qubit index <target>.
    // This involves calculating the full probability vector,
    // randomly picking a measurement outcome according to it and change the state according
    // to the measurement theorem 
    // p(m) = <psi|P_m|psi>
    // |psi'> = (P_m |psi>) / sqrt(p(m))
    
    //calculate P_0 and P_1 matrices
    Vector zeroVector = vector_fromArray((Complex[]) {(Complex) {1.0, 0.0}, (Complex) {0.0, 0.0}}, 2);
    Vector oneVector = vector_fromArray((Complex[]) {(Complex) {0.0, 0.0}, (Complex) {1.0, 0.0}}, 2);

    Matrix zeroProjector = vector_outerProduct(zeroVector, zeroVector);
    Matrix oneProjector = vector_outerProduct(oneVector, oneVector);

    Matrix zeroProjectorExpanded = expandMatrixToQuregSize(qureg, zeroProjector, 1, target);
    Matrix oneProjectorExpanded = expandMatrixToQuregSize(qureg, oneProjector, 1, target);


    if (!matrix_isHermitian(zeroProjectorExpanded) || !matrix_isProjector(zeroProjectorExpanded)) {
        LOG_ERROR(LOGOBJ("Zero Projector is not a hermitian projector \n"));
    } 
    if (!matrix_isHermitian(oneProjectorExpanded) || !matrix_isProjector(oneProjectorExpanded)) {
        LOG_ERROR(LOGOBJ("One Proejector is not a hermitian projector \n"));
    }

    double zeroProbability = matrix_braket_product(zeroProjectorExpanded, qureg.stateVector, qureg.stateVector).re;
    double oneProbability = matrix_braket_product(oneProjectorExpanded, qureg.stateVector, qureg.stateVector).re;


    // TODO: Add possitibility to log primitives
    LOG_DEBUG(
        LOGOBJ("[qureg_applyZMeasurement] Applying Z measurement with likelyhood of |0> result: \n"),
        LOGOBJ("[qureg_applyZMeasurement] Applying Z measurement with likelyhood of |1> result: \n")
    );

    if (fabs(zeroProbability + oneProbability - 1.0) > 0.0001) {
        LOG_ERROR(LOGOBJ("[qureg_applyZMeasurement] Sum of measurement probabilities does not equal 1.0 \n"));
    }

   // randomly choose whether the state will collapse into |0> or |1> state according to the 
    // calculated probability distribution

    measurementResult->measuredQubitIndex = target;

    Matrix measurementMatrix;
    double probability;
    // create a random number in the range [0, 1]
    double randomNumber = (double) rand() / (double) RAND_MAX;
    if (randomNumber <= zeroProbability) {
        // random experiment decides that qubit state will collapse into |0> state
        measurementMatrix = zeroProjector;
        probability = zeroProbability;
        measurementResult->measuredValue = 0.0;
    } else {
        // collapse into |1> state
        measurementMatrix = oneProjector;
        probability = oneProbability;
        measurementResult->measuredValue = 1.0;
    }
    measurementMatrix = matrix_scaleINP(measurementMatrix, (Complex) {1/sqrt(probability), 0.0});

    return qureg_apply1QubitUnitary(qureg, target, measurementMatrix);
}




