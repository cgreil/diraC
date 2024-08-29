#ifndef COMPLEX_H_
#define COMPLEX_H_

#include <stdbool.h>
#include <math.h>

#include "common/types.h"
#include "common/string.h"

#define MAX_COMPLEX_DISP_SIZE 64

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



Complex conjugate(Complex c);

TYPE_FLOAT modulus(Complex c);

Polar polar(Complex c);

OptComplex power(Complex c, int pow);

Complex complex_addition(Complex c1, Complex c2);

Complex complex_addition(Complex c1, Complex c2);

Complex complex_subtraction(Complex c1, Complex c2);

Complex complex_multiplication(Complex c1, Complex c2);

OptComplex divide(Complex c1, Complex c2);

Complex complex_clone(Complex c);

Complex complex_exp(Complex c);

Complex complex_sin(Complex c);

Complex complex_cos(Complex c);

Complex complex_root(Complex c);


String complex_display(Arena *arena, Complex c);

#endif
