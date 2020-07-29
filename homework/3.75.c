#include <complex.h>

double c_imag(double complex x) {
    return cimag(x);
}

double c_real(double complex x) {
    return creal(x);
}

double complex c_sub(double complex x, double complex y) {
    return x - y;
}

/*
double c_imag(double complex x)
c_imag:
  movapd  %xmm1, %xmm0
  ret

double c_real(double complex x)
c_real:
  rep; ret

double complex c_sub(double complex x, double complex y)
c_sub:
  subsd  %xmm2, %xmm0
  subsd  %xmm3, %xmm1
  ret

A.
Real part is stored in %xmm0 and imaginary part is stored in %xmm1. The next number is stored in the same way in registers %xmm2 and %xmm3.

B.
A function passes the real part in %xmm0 and the imaginary part in %xmm1
*/
