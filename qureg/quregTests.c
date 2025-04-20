//
// Created by christoph on 02.12.24.
//
#include <stdio.h>
#include <time.h>
#include <check.h>

#include "utils/testutils.h"
#include "utils/dataArena.h"

#include "qureg/qureg.h"
#include "utils/dataArena.h"
#include "logging/Logger.h"

Arena* arena;
Logger* logger;

START_TEST(quregHadamardSingleQubitTest) {

    size_t numQubits = 1;
    QuantumRegister qureg = qureg_new(numQubits);

    qureg = qureg_applyHadamard(qureg, 0);

    Complex actualFirst = qureg.stateVector.dataArray.values[0];
    Complex expectedFirst = {0.7071067811865475, 0.0};

    Complex actualSecond = qureg.stateVector.dataArray.values[1];
    Complex expectedSecond = {0.7071067811865475, 0.0};
    // assert directly on statevector amplitudes
    ck_assert_complex_eq(actualFirst, expectedFirst);
    ck_assert_complex_eq(actualSecond, expectedSecond);


} END_TEST


START_TEST(qureg1QubitGateOutOfCircuitTest) {

    size_t numQubits = 2;
    QuantumRegister qureg = qureg_new(numQubits);

    qureg = qureg_applyPauliZ(qureg, 2);
    
    QuantumRegister emptyQureg = (QuantumRegister) { 0 };
    
    ck_assert_qureg_eq(emptyQureg, qureg);

} END_TEST


START_TEST(qureg2QubitTargetOutOfCircuitTest) {

    size_t numQubits = 5;
    QuantumRegister qureg = qureg_new(numQubits);

    qureg = qureg_applyCNOT(qureg, 4, 5);

    QuantumRegister emptyQureg = (QuantumRegister) { 0 };

    ck_assert_qureg_eq(emptyQureg, qureg);
} END_TEST


START_TEST(qureg2QubitControlOutOfCircuitTest) {

    size_t numQubits = 4;
    QuantumRegister qureg = qureg_new(numQubits);

    qureg = qureg_applyCNOT(qureg, 4, 5);

    QuantumRegister emptyQureg = (QuantumRegister) { 0 };

    ck_assert_qureg_eq(emptyQureg, qureg);
} END_TEST


START_TEST(quregBellCircuitTest) {

    // small test circuit consisting of two qubits, 
    // which will be transformed from 
    // the |00> state to the bell state |++>

    size_t numQubits = 2;
    QuantumRegister qureg = qureg_new(numQubits);

    qureg = qureg_applyHadamard(qureg, 0);
    qureg = qureg_applyCNOT(qureg, 0, 1);

    Complex expectedValues[] = {
        (Complex) {0.7071067811865475, 0.0},
        (Complex) {0.0, 0.0},
        (Complex) {0.0, 0.0},
        (Complex) {0.7071067811865475, 0.0},
    };
    Vector expectedVector = vector_fromArray(expectedValues, 4);

    Vector actualVector = qureg.stateVector;

    ck_assert_vectorValues_eq(expectedVector, actualVector);
} END_TEST



Suite* quantumRegisterBasicSuite(void) {

    Suite *suite = suite_create("quantumRegisterBasicSuite");

    TCase* testcase1 = tcase_create("quregHadamardSingleQubitTest");
    TCase* testcase2 = tcase_create("quregBellCircuitTest");
    TCase* testcase3 = tcase_create("qureg1QubitGateOutOfCircuitTest");
    TCase* testcase4 = tcase_create("qureg2QubitTargetOutOfCircuitTest");
    TCase* testcase5 = tcase_create("qureg2QubitControlOutOfCircuitTest");

    tcase_add_test(testcase1, quregHadamardSingleQubitTest);
    tcase_add_test(testcase2, quregBellCircuitTest);
    tcase_add_test(testcase3, qureg1QubitGateOutOfCircuitTest);
    tcase_add_test(testcase4, qureg2QubitTargetOutOfCircuitTest);
    tcase_add_test(testcase5, qureg2QubitControlOutOfCircuitTest);

    suite_add_tcase(suite, testcase1);
    suite_add_tcase(suite, testcase2);
    suite_add_tcase(suite, testcase3);
    suite_add_tcase(suite, testcase4);
    suite_add_tcase(suite, testcase5);

    return suite;
}

// -----------------------------------------------
// END OF BASIC TESTS
// -----------------------------------------------


START_TEST(quantumMeasurementTrivialZeroTest) {

    // Trivial testcase for measurement
    // regardless of the number of executions, a 
    // state prepared in the |0> state 
    // has to collapse into the |0> state again 

    size_t numQubits = 1;
    QuantumRegister qureg = qureg_new(numQubits);

    MeasurementResult measurementResult;

    qureg_applyZMeasurement(qureg, 0, &measurementResult);

    Complex expectedValues[] = {
        (Complex) {1.0, 0.0},
        (Complex) {0.0, 0.0}
    };
    Vector expectedVector = vector_fromArray(expectedValues, 2);

    ck_assert_vectorValues_eq(expectedVector, qureg.stateVector);
    ck_assert_double_eq(measurementResult.measuredValue, 0.0);

} END_TEST

START_TEST(quantumMeasurementTrivialOneTest) {


    size_t numQubits = 1;
    QuantumRegister qureg = qureg_new(numQubits);

    qureg = qureg_applyPauliX(qureg, 0);


    MeasurementResult measurementResult; 
    qureg = qureg_applyZMeasurement(qureg, 0, &measurementResult);

    Complex expectedValues[] = {
        (Complex) {0.0, 0.0},
        (Complex) {1.0, 0.0}
    };
    Vector expectedVector = vector_fromArray(expectedValues, 2);

    ck_assert_vectorValues_eq(expectedVector, qureg.stateVector);
    ck_assert_double_eq(measurementResult.measuredValue, 1.0);

} END_TEST

START_TEST(quantumMeasurementParallelizationTest) {

    size_t numQubits = 2;
    QuantumRegister qureg = qureg_new(numQubits);

    qureg = qureg_applyHadamard(qureg, 0);
    qureg = qureg_applyHadamard(qureg, 1);

    MeasurementResult measurementResult;
    // Perform measurement only on the first qubit, leaving the super-
    // position of the second one intact
    qureg = qureg_applyZMeasurement(qureg, 0, &measurementResult);

    // TODO: expected (calculated with YAO.jl) and actual do differ
    // outside of the global phase. Find out what is the issue here
    // Maybe measurement is not correctly implemented?

    Complex expectedValues[] = {
        (Complex) {0.0, 0.0},
        (Complex) {0.0, 0.0},
        (Complex) {0.7071067811865476, 0.0},
        (Complex) {0.7071067811865476, 0.0}
    };
    Vector expectedVec = vector_fromArray(expectedValues, 4);

    ck_assert_statevectors_eq(expectedVec, qureg.stateVector);

} END_TEST


START_TEST(quantumMeasurementBellTest) {

    size_t numQubits = 2;
    QuantumRegister qureg = qureg_new(numQubits);

    qureg = qureg_applyHadamard(qureg, 0);
    qureg = qureg_applyCNOT(qureg, 0, 1);

    Complex expectedValues[] = {
        (Complex) {0.7071067811865475, 0.0},
        (Complex) {0.0, 0.0},
        (Complex) {0.0, 0.0},
        (Complex) {0.7071067811865475, 0.0} 
    };
    Vector expectedVec = vector_fromArray(expectedValues, 4);

    ck_assert_statevectors_eq(expectedVec, qureg.stateVector);

} END_TEST


Suite* quantumRegisterMeasurementSuite(void) {

    Suite* suite = suite_create("quantumRegisterMeasurementSuite");


    TCase* testcase1 = tcase_create("quantumMeasurementTrivialZeroTest");
    TCase* testcase2 = tcase_create("quantumMeasurementTrivialOneTest");
    TCase* testcase3 = tcase_create("quantumMeasurementParallelizationTest");
    TCase* testcase4 = tcase_create("quantumMeasurementBellTest");

    tcase_add_test(testcase1, quantumMeasurementTrivialZeroTest);
    tcase_add_test(testcase2, quantumMeasurementTrivialOneTest);
    tcase_add_test(testcase3, quantumMeasurementParallelizationTest);
    tcase_add_test(testcase4, quantumMeasurementBellTest);

    suite_add_tcase(suite, testcase1);
    suite_add_tcase(suite, testcase2);
    suite_add_tcase(suite, testcase3);
    suite_add_tcase(suite, testcase4);

    return suite;
}



int main(void) {

    arena = arena_init();
    logger = logger_init(STDOUT);

    Suite* testSuites[] = {
        quantumRegisterBasicSuite(),
        quantumRegisterMeasurementSuite()
    };

    size_t numSuites = sizeof(testSuites) / sizeof(Suite*);
    SRunner* suiteRunner;

    for(size_t i = 0; i < numSuites; i++) {

        fprintf(stdout, "Running suite (%zu/%zu) \n", i+1, numSuites);

        Suite *currentSuite = testSuites[i];
        suiteRunner = srunner_create(currentSuite);

        time_t startTime = time(NULL);
        srunner_run_all(suiteRunner, CK_NORMAL);
        time_t endTime = time(NULL);

        int numFailed = srunner_ntests_failed(suiteRunner);
        fprintf(stdout, "Finished with %d failed tests in %f seconds", numFailed, difftime(endTime, startTime));
    }

    arena_destroy(arena);

    return 0;
}

