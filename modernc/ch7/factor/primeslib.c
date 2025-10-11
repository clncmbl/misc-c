#include <stdlib.h>
#include <stdio.h>
#include <threads.h>
#include <assert.h>
#include <stdint.h>

typedef struct intDynamicArr intDynamicArr;
struct intDynamicArr {
  size_t capacity;
  int* data;
  size_t count;
};
static intDynamicArr primes;

typedef struct wheelStruct wheelStruct;
struct wheelStruct {
  size_t capacity;
  bool* data;
  size_t count;
  size_t idxmaxtrue;
};
static wheelStruct wheel;

static int wheel_offsets[] = {1, 7, 11, 13, 17, 19, 23, 29};

static inline int num_from_wheel(size_t idx) {
  return 30 * (idx/8) + wheel_offsets[idx % 8];
}

static inline size_t idx_from_wheel(int num) {
  size_t idx_base = 8 * (num/30);
  int num_offset = num % 30;
  size_t idx_offset;
  switch (num_offset) {
    case  1: idx_offset = 0; break;
    case  7: idx_offset = 1; break;
    case 11: idx_offset = 2; break;
    case 13: idx_offset = 3; break;
    case 17: idx_offset = 4; break;
    case 19: idx_offset = 5; break;
    case 23: idx_offset = 6; break;
    case 29: idx_offset = 7; break;
    default: return SIZE_MAX;
  }
  return idx_base + idx_offset;
}

static void set_wheel_idx_composite(size_t idx) {
  // This assumes idx is not already set.  This should be okay
  // as long as we do not aim to mark anything greater than
  // x*y^2 where x is the smallest prime after the initial prime
  // (7, here) and y is the smallest prime that follows the last
  // limit of marking.  To simplify logic, we should initially set
  // wheel.data[idx_from_wheel(49)]=true,
  // wheel.idxmaxtrue = idx_from_wheel(49).  Then, x=7 and
  // y = num_from_wheel(wheel.idxmaxtrue + 1) = 53.
  assert(wheel.data[idx] == false);
  assert(idx < wheel.capacity);
  wheel.data[idx] = true;
  ++wheel.count;
}

static once_flag init_flag = ONCE_FLAG_INIT;

static void init_data(void) {
  primes.capacity = 2;
  primes.data = malloc(primes.capacity * sizeof(int));
  primes.data[0] = 2;
  primes.data[1] = 3;
  primes.count = 2;

  // The data are just an array of bools indicating
  // which wheel indexes correspond to composite (that is,
  // not prime) numbers.
  wheel.capacity = 1000;
  wheel.data = calloc(wheel.capacity, sizeof(bool));
  wheel.count = 0;
  wheel.idxmaxtrue = idx_from_wheel(49);
  set_wheel_idx_composite(wheel.idxmaxtrue);
}

int next_prime(int start) {
  call_once(&init_flag, init_data);

  printf("%d\n", num_from_wheel(10));
  printf("%zu\n", idx_from_wheel(37));
  printf("%zu\n", idx_from_wheel(39));
  printf("%zu\n", idx_from_wheel(11111137));

  return start;
}

