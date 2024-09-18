#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "complex/complex.h"
#include "common/string.h"

Complex complex_conjugate(Complex c) {
    Complex result = { c.re, -c.im };
    return result;
}

TYPE_FLOAT complex_modulus(Complex c) {
    TYPE_FLOAT temp = c.re * c.re + c.im + c.im;
    return sqrt(temp); 
}

Polar polar(Complex c) {


    Polar result = {
        .r = sqrt(complex_modulus(c)),
        .theta = atan2f(c.im, c.re)
    };
    return result;
}

OptComplex complex_power(Complex c, int pow) {
    
    if (pow == 0) {
        return (OptComplex) { 
            .value = (Complex){ 1.0, 0.0 },
            .valid = true
        };
    }

    Complex temp = c;
    // create exponentation
    for (int i = 1; i < abs(pow); ++i) {
        temp = complex_multiplication(temp, c);
    }
    // discern cases for positive and negative powers
    if (pow > 0) {
        return (OptComplex) {
            .value = temp,
            .valid = true
        };
    }
    else {
        OptComplex inverse = complex_division((Complex) {1.0, 0.0}, temp);
        return inverse; 
    } 
}

Complex complex_addition(Complex c1, Complex c2) {
    Complex result = { c1.re + c2.re, c1.im + c2.im };
    return result;
}

Complex complex_subtraction(Complex c1, Complex c2) {
    Complex result = { c1.re - c2.re, c1.im - c2.im };
    return result; 
}

Complex complex_multiplication(Complex c1, Complex c2) {
    // z1 * z2 = (a + ib)(c + id) 
    // = ac + aib + ibc - bd
    Complex result = { c1.re * c2.re - c1.im * c2.im, c1.re * c2.im + c1.im * c2.re };
    return result;
}

OptComplex complex_division(Complex c1, Complex c2) {

    if (fabs(c2.re) < 0.00001 && fabs(c2.im) < 0.00001) {
        OptComplex result = {
            .valid = false
        };

        return result;
    }


    // z1 / z2 = (a + ib) / (c + id)
    // = (a + ib)(c - id) / (c + id)(c - id)
    // = (ac - iad + ibc + bd)(c^2 + d^2)

    TYPE_FLOAT denominator = c2.re * c2.re + c2.im * c2.im;
    TYPE_FLOAT real_part = c1.re * c2.re + c1.im * c2.im;
    TYPE_FLOAT im_part = c1.im * c2.re - c1.re * c2.im;

    OptComplex result = { 
        .value = (Complex){real_part / denominator, im_part / denominator},
        .valid = true
    };
    return result;
}

Complex complex_clone(Complex c) {
    return (Complex) {
        .re = c.re,
        .im = c.im
    };
}


Complex complex_exp(Complex c) {

    // A c number z can be displayed as
    // z = a + ib.
    // One can then use the matrix_identity
    // exp(x + y) = exp(x) * exp(y)
    // where x is already real, and y is of the form
    // ib. We can then decompose exp(y) using the
    // euler matrix_identity:
    // exp(ib) = cos(b) + i*sin(b)

    TYPE_FLOAT a = c.re;
    TYPE_FLOAT b = c.im;

    Complex lhs = (Complex) {
        .re = exp(a),
        .im = 0.0
    };

    Complex rhs = (Complex) {
        .re = cos(b),
        .im = sin(b)
    };

    return complex_multiplication(lhs, rhs);
}

Complex complex_sin(Complex c) {

    return (Complex) {
        .re = sin(c.re) * cosh(c.im),
        .im = cos(c.re) * sinh(c.im)
    };
}

Complex complex_cos(Complex c) {

    return (Complex) {
        .re = cos(c.re) * cosh(c.im),
        .im = - sin(c.re) * sinh(c.im)
    };
}

Complex complex_root(Complex c) {

    // TODO: implement

    return (Complex) { 0 };
}

Complex complex_ln(Complex complex) {

    // TODO: Implement

    return (Complex) { 0 };

}


String complex_display(Arena *arena, Complex complex) {

    char displayBuffer[MAX_COMPLEX_DISP_SIZE];
    int numPrinted = sprintf(displayBuffer, "%.2f + %.2fi", complex.re, complex.im);

    assert(numPrinted > 0);

    String string = string_create(arena, displayBuffer, numPrinted);

    return string;
}

