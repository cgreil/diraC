#ifndef COMPLEX_H_
#define COMPLEX_H_

#include <stdbool.h>

#include "../common/types.h"


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

Complex root(Complex c);

Complex add(Complex c1, Complex c2);

Complex subtract(Complex c1, Complex c2);

Complex multiply(Complex c1, Complex c2);

OptComplex divide(Complex c1, Complex c2);

Complex complex_clone(Complex complex);



#endif
