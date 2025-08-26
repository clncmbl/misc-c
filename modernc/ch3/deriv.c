
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

const double h = 1E-8;

typedef double (*doublefunc)(double);

double get_deriv_val(doublefunc func, double atval) {
  double deriv = (func(atval+h) - func(atval))/h;
  return deriv;
}

typedef double complex (*doublecfunc)(double complex);

double get_deriv_val_complex_step(doublecfunc func, double atval) {
  //double complex ih = h*I;
  double deriv = cimag(func(atval+I*h)/h);
  //return cimag(atval + I*h);
  return deriv;
}

double square(double x) {
  return x*x;
}

double complex csquare(double complex x) {
  return x*x;
}

int main(int argc, char* argv[argc+1]) {
  double deriv = get_deriv_val(square, 3);

  printf("%.12f\n", deriv);

  double deriv_cs = get_deriv_val_complex_step(csquare, 3);
  printf("%.12f\n", deriv_cs);

  return EXIT_SUCCESS; 
}

