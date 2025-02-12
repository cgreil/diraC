//
// Created by christoph on 02.12.24.
//
#include <stdio.h>
#include <time.h>

#include "check.h"
#include "utils/testutils.h"
#include "utils/dataArena.h"

#include "qureg/qureg.h"
#include "utils/dataArena.h"

Arena* arena;

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

Suite* quantumRegisterBasicSuite(void) {

    Suite *suite = suite_create("quantumRegisterBasicSuite");

    TCase* testcase1 = tcase_create("quregHadamardSingleQubitTest");

    tcase_add_test(testcase1, quregHadamardSingleQubitTest);

    suite_add_tcase(suite, testcase1);

    return suite;
}




int main(void) {

    arena = arena_init();

    Suite* testSuites[] = {
        quantumRegisterBasicSuite()
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

