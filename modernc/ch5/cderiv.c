

#include <stdio.h>
#include <tgmath.h>

double PI = acos(-1.0);

typedef double complex (*dblcfunc)(double complex);

double complex cderiv(dblcfunc func, double complex z) {
  double h = 1E-5;

  return (func(z+h) - func(z-h))/(2*h);
}

double complex csquare(double complex z) {
  return z*z;
}

int main(int argc, char* argv[argc+1]) {

  //double complex z = PI;
  double complex z = -.3 + I*.6;

  double complex deriv = cderiv(ccos, z);

  printf("%.9f+%.9fi\n", creal(deriv), cimag(deriv));
}

