#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "complex.h"

Complex conjugate(Complex c) {
    Complex result = { c.re, -c.im };
    return result;
}

TYPE_FLOAT modulus(Complex c) {
    TYPE_FLOAT temp = c.re * c.re + c.im + c.im;
    return sqrt(temp); 
}

Polar polar(Complex c) {


    Polar result = {
        .r = sqrt(modulus(c)),
        .theta = atan2f(c.im, c.re)
    };
    return result;
}

OptComplex power(Complex c, int pow) {
    
    if (pow == 0) {
        return (OptComplex) { 
            .value = (Complex){ 1.0, 0.0 },
            .valid = true
        };
    }

    Complex temp = c;
    // create exponentation
    for (int i = 1; i < abs(pow); ++i) {
        temp = multiply(temp, c);
    }
    // discern cases for positive and negative powers
    if (pow > 0) {
        return (OptComplex) {
            .value = temp,
            .valid = true
        };
    }
    else {
        OptComplex inverse = divide((Complex) { 1.0, 0.0 }, temp);
        return inverse; 
    } 
}

Complex add(Complex c1, Complex c2) {
    Complex result = { c1.re + c2.re, c1.im + c2.im };
    return result;
}

Complex subtract(Complex c1, Complex c2) {
    Complex result = { c1.re - c2.re, c1.im - c2.im };
    return result; 
}

Complex multiply(Complex c1, Complex c2) {
    // z1 * z2 = (a + ib)(c + id) 
    // = ac + aib + ibc - bd
    Complex result = { c1.re * c2.re - c1.im * c2.im, c1.re * c2.im + c1.im * c2.re };
    return result;
}

OptComplex divide(Complex c1, Complex c2) {

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




