#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "primeslib.h"

typedef struct sizetList sizetList;
struct sizetList {
  size_t size;
  size_t* data;
  size_t count;
};

sizetList primes;


bool isnextprime(size_t testnum) {
  for (size_t i = 0; i < primes.count; ++i) {
    if (testnum % primes.data[i] == 0)
      return false;
  }
  if (primes.count == primes.size) {
    
  }

  return true;
}

size_t nextprime(size_t n) {

  size_t curr_max = primes.data[n-1];
  assert(curr_max >= 3);
  for (size_t next = curr_max + 2; ; next += 2) {
    printf("%zu\n", next);
    //if (isprime(next, n, primes)) {
    //  return next;
    //}
  }
}

size_t smallest_prime_factor(size_t n) {
  if (n % 2 == 0) return 2;
  if (n % 3 == 0) return 3;
  if (n % 5 == 0) return 5;
  size_t inc[8] = { 7, 11, 13, 17, 19, 23, 29, 31 };
  for (size_t i = 0; ; ++i) {
    size_t p = inc[i];
    printf("%zu\n", p);
    break;
  }
  return 0;  // placeholder for now.
}


int main(int argc, char* argv[argc+1]) {

  if (argc > 1) 
    printf("%s\n", argv[1]);
  else {
    fprintf(stderr, "Missing command parameter\n");
    exit(EXIT_FAILURE);
  }

  primes.size = 2;
  primes.data = malloc(primes.size * sizeof(size_t));
  primes.data[0] = 2;
  primes.data[1] = 3;
  primes.count = 2;

  // n is the number for which we want the factor.
  size_t divisor = smallest_prime_factor(48);
  printf("%zu\n", divisor);

  int p = next_prime(6);
  printf("%d\n", p);

  wheelStruct *wheel = create_wheel();
  size_t size = 100;

  int primevals[size] = {};
  get_array_of_primes(wheel, size, primevals);
  for (size_t i = 0; i < size; ++i) {
    printf("%d  ", primevals[i]);
  }
  printf("\n");
}

