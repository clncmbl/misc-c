#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>


typedef struct sizetList sizetList;
struct sizetList {
  size_t* data;
  size_t size;
};


bool isprime(size_t testnum, size_t n, size_t primes[static n]) {
  for (size_t i = 0; i < n; ++i) {
    if (testnum % primes[i] == 0) {
      return false;
    }
  }
  return true;
}

size_t nextprime(size_t n, size_t primes[static n]) {

  size_t curr_max = primes[n-1];
  assert(curr_max >= 3);
  for (size_t next = curr_max + 2; ; next += 2) {
    printf("%zu\n", next);
    if (isprime(next, n, primes)) {
      return next;
    }
  }
}


int main(int argc, char* argv[argc+1]) {

  sizetList primes2;
  primes2.size = 2;
  primes2.data = malloc(primes2.size * sizeof(size_t));
  primes2.data[0] = 2;
  primes2.data[1] = 3;

  size_t psize = 3;
  size_t* primes = malloc(psize * sizeof(size_t));
  primes[0] = 2;
  primes[1] = 3;

  size_t divisor = nextprime(primes2.size, primes2.data);
  printf("%zu\n", divisor);

}

