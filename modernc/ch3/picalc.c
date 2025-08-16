#include <stdlib.h>
#include <stdio.h>

#define __USE_GNU
#include <math.h>  // for M_PIl

int main(int argc, char* argv[argc+1]) {
  if (argc < 2) {
    printf("precision operand required\n");
    exit(EXIT_FAILURE);
  }

  int precision = atoi(argv[1]);
  if (precision > 17) {
    printf("precision must be no greater than 17\n");
    exit(EXIT_FAILURE);
  }

  const long double eps = powl(0.1L, precision);

  long double calcpi(int prec) {
    // Given a regular polygon inscribed in a unit circle.
    //   s = the length of each side (chord)
    //   a = the apothem (chord midpoint to center)
    //   g = the sagitta (chord midpoint to circle)
    //   inner = perimeter of inscribed
    //   outer = perimeter of circumscribed

    long int sides = 6;
    long double s = 1L;

    for (;;) {

      // s/2 squared = s*s/4
      //const long double a = sqrtl(1 - s*s/4);
      const long double a = sqrtl((1 - s/2)*(1+s/2));
      if (a >= 1) {
        printf("%.30Lf  %.65Lf  %.65Lf  %.65Lf\n", s, a, 1-s/2, 1+s/2);
        fprintf(stderr, "Floating point limit exceeded when squaring the side length.\n");
        exit(EXIT_FAILURE);
      }

      // We now have enough info to get inner and outer
      // for previously calculated (or initialized) s.

      const long double inner = sides * s;
      const long double outer = inner / a;

      // pi is approximately the approximated circumference
      // divided by the diameter, which is 2.
      const long double pi_i = inner/2;
      const long double pi_o = outer/2;
      printf("%.*Lf  %.30Lf  %.65Lf\n", precision, pi_o - pi_i, s, a);



      if ((pi_o - pi_i) < eps) {
        return pi_i;
      }
      
      sides = sides * 2;
      const long double g = 1 - a;
      s = sqrtl(g*g + s*s/4);
    }
  }

  const long double pi = calcpi(precision);

  printf("%.*Lf\n", 65, pi); //precision, pi);
  printf("%.*Lf\n", 65, M_PIl); //precision, pi);
  printf("..12345678901234567890\n");


}
