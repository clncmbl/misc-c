#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "primeslib.h"

int main(int argc, char* argv[argc+1]) {

  if (argc < 1) {
    fprintf(stderr, "Missing command parameter\n");
    exit(EXIT_FAILURE);
  }


  wheelStruct *wheel = create_wheel();
  size_t size = 10;

  int primevals[size] = {};
  get_array_of_primes(wheel, size, primevals);
  for (size_t i = 0; i < size; ++i) {
    printf("%d  ", primevals[i]);
  }
  printf("\n");

  for (int i = 1; i < argc; ++i) {
    int n = atoi(argv[i]);
    //int n = 205193;
    printf("%d: ", n);

    int f = 1;
    do {
      // The first division is essentially a no-op.
      n = n / f; 
      f = get_smallest_factor(wheel, n);
      printf("%d ", f);
    } while (f != n);
    puts("");
  }
}

