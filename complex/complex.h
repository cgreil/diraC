#ifndef COMPLEX_H_
#define COMPLEX_H_

#include <stdbool.h>
#include <math.h>

#include "common/types.h"

#define MAX_COMPLEX_DISP_SIZE 64

#define EPSILON 0.000001

/** 
 * Struct definition for complex data type
 * @member re ... real value of complex number
 * @member im ... imaginary value of complex number
 *
 *
 */

typedef struct {
    TYPE_FLOAT re;
    TYPE_FLOAT im;
} Complex;


typedef struct {
    Complex value;
    bool valid;
} OptComplex;


typedef struct {
    TYPE_FLOAT r;
    TYPE_FLOAT theta;
} Polar;



Complex complex_conjugate(Complex c);

TYPE_FLOAT complex_modulus(Complex c);

Polar polar(Complex c);

OptComplex complex_power(Complex c, int pow);

Complex complex_addition(Complex c1, Complex c2);

Complex complex_addition(Complex c1, Complex c2);

Complex complex_subtraction(Complex c1, Complex c2);

Complex complex_multiplication(Complex c1, Complex c2);

OptComplex complex_division(Complex c1, Complex c2);

Complex complex_clone(Complex c);

Complex complex_exp(Complex c);

Complex complex_sin(Complex c);

Complex complex_cos(Complex c);

Complex complex_root(Complex c);

bool complex_nearlyEqual(Complex c1, Complex c2);


#endif
