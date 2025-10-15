#include <stdlib.h>
#include <stdio.h>
#include <threads.h>
#include <assert.h>
#include <stdint.h>
#include <math.h>

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
  size_t idxmax;
  size_t idxmaxtrue;
};
static wheelStruct wheel;

const static int wheel_offsets[] = {1, 7, 11, 13, 17, 19, 23, 29};

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


// A ceiling_index function that will find
// the lowest index that includes a given number.
// For example, passing passing 20 should return 6
// because 6 corresponds to 23, which is the next
// wheel number.
static size_t ceiling_index(int num) {
  size_t idx_base = 8 * (num/30);
  int num_offset = num % 30;

  size_t ceiling = idx_base +
                       ((num_offset > 23) ? 7 :
                        (num_offset > 19) ? 6 :
                        (num_offset > 17) ? 5 :
                        (num_offset > 13) ? 4 :
                        (num_offset > 11) ? 3 :
                        (num_offset >  7) ? 2 :
                        (num_offset >  1) ? 1 : 0);

  printf("In ceiling_index: %d  %zu  %zu\n", num, idx_base, ceiling);
  return ceiling;
}

static size_t idxstart_from_wheel(int lastmax, int factor) {
  // lastmax is the most recent max value for this wheel.
  // factor is the current factor for which we are about to
  // mark composites.

  // next is the first number that is not yet in our coverage
  // range.
  int next = lastmax / factor + 1;

  // Now we need to find the first index that includes next.
  size_t idx_next = ceiling_index(next);
  return idx_next;
}


static inline void set_wheel_idx_composite(size_t idx) {
  assert(idx < wheel.capacity);
  wheel.data[idx] = true;
}

// This is the core of the wheeling operation.
static void set_wheel_composites(size_t maxidx) {
  int targmaxnum = num_from_wheel(maxidx);
  size_t currmaxidx = wheel.idxmax;
  int currmaxnum = num_from_wheel(currmaxidx);

  int inumstop = sqrt(targmaxnum); 

  for (size_t i = 1; ; ++i) {
    // This iterates over all the prime numbers starting with
    // 7.  We skip over composites.  The first one we skip
    // will be 49.
    if (wheel.data[i])
      continue;
    assert(i < wheel.capacity);
    int n = num_from_wheel(i);
    if (n > inumstop)
      break;
    size_t jstart = idxstart_from_wheel(currmaxnum, n);
    if (i > jstart) jstart = i;
    printf("%d %zu\n", currmaxnum/n, jstart);
    for (size_t j = jstart; ; ++j) {
      // This iterates over all the numbers in the wheel including
      // composites (with all factors >= 7) because we don't want
      // to miss marking products of three or more primes (e.g.,
      // 7*11*11).
      int wheelnum = num_from_wheel(j);
      int prod = n * wheelnum;
      if (prod > targmaxnum)
        break;

      set_wheel_idx_composite(idx_from_wheel(prod));
      printf("%zu %zu     %d %d %d\n", i, j, n, wheelnum, prod);
    }
  }
  wheel.idxmax = maxidx;
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
  wheel.capacity = 1000*1000;
  wheel.data = calloc(wheel.capacity, sizeof(bool));
  wheel.idxmax = 1;  // Represents 7.
  wheel.idxmaxtrue = idx_from_wheel(49);
  set_wheel_idx_composite(wheel.idxmaxtrue);
  set_wheel_composites(130);
  set_wheel_composites(160);
  set_wheel_composites(190);
}

int next_prime(int start) {
  // TODO: For memory allocation, implement initialization
  //       and cleanup functions.  Or use opaque pointer
  //       solution.
  call_once(&init_flag, init_data);

  printf("%d\n", num_from_wheel(10));
  printf("%zu\n", idx_from_wheel(37));
  printf("%zu\n", idx_from_wheel(39));
  printf("%zu\n", idx_from_wheel(11111137));
/*
  printf("> %zu\n", idxstart_from_wheel(29*7+1, 7));
  printf("> %zu\n", idxstart_from_wheel(30*7-1, 7));
  printf("> %zu\n", idxstart_from_wheel(30*7, 7));
  printf("> %zu\n", idxstart_from_wheel(36*7-1, 7));
  printf("> %zu\n", idxstart_from_wheel(37*7  , 7));
  printf("> %zu\n", idxstart_from_wheel(38*7+1, 7));
*/
  return start;
}

