#include <stdlib.h>
#include <threads.h>

typedef struct intDynamicArr intDynamicArr;
struct intDynamicArr {
  size_t capacity;
  int* data;
  size_t count;
};

static intDynamicArr primes;


static once_flag init_flag = ONCE_FLAG_INIT;

static void init_primes(void) {
  primes.capacity = 2;
  primes.data = malloc(primes.capacity * sizeof(int));
  primes.data[0] = 2;
  primes.data[1] = 3;
  primes.count = 2;
}

int next_prime(int start) {
  call_once(&init_flag, init_primes);
  return start;
}

