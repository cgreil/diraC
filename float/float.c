#include "float.h"

TYPE_FLOAT squareRoot(TYPE_FLOAT x, TYPE_FLOAT accuracy) {
    TYPE_FLOAT y = x;
    TYPE_FLOAT root;

    int num_iterations = 0;

    while (1) {
        num_iterations++;

        root = 0.5 * (y + (x / y));

        // check if convergence is reached
        if (fabs(root - y) < accuracy) {
            break;
        }

        y = root;
    }

    return root;
}


