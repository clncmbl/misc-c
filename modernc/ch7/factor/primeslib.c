
#include "primeslib.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

/*
typedef struct intDynamicArr intDynamicArr;
struct intDynamicArr {
  size_t capacity;
  int* data;
  size_t count;
};
static intDynamicArr primes;
*/

struct wheelStruct {
  size_t capacity;
  bool *data;
  size_t idxmax;
};

static const int wheel_offsets[] = {1, 7, 11, 13, 17, 19, 23, 29};

static inline int num_from_wheel(size_t idx) {
  return 30 * (idx/8) + wheel_offsets[idx % 8];
}

static inline size_t idx_for_wheel_num(int num) {
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
    default: fprintf(stderr,
                     "ERROR: %d does not correspond to a wheel index\n",
                      num);
             exit(EXIT_FAILURE);
  }
  return idx_base + idx_offset;
}


// A ceiling_index function that will find
// the lowest index that includes a given number.
// For example, passing num 20 should return an index of 6
// because index 6 corresponds to num 23, which is the next
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


static inline void set_wheel_idx_composite(
    wheelStruct *wheel,
    size_t idx) {
  assert(idx < wheel->capacity);
  wheel->data[idx] = true;
}

// This is the core of the wheeling operation.
static void set_wheel_composites(
    wheelStruct *wheel,
    size_t maxidx) {

  // TODO: Check for memory allocation and expand, if necessary.


  int targmaxnum = num_from_wheel(maxidx);
  size_t currmaxidx = wheel->idxmax;
  int currmaxnum = num_from_wheel(currmaxidx);

  int inumstop = sqrt(targmaxnum); 

  for (size_t i = 1; ; ++i) {
    // This iterates over all the prime numbers starting with
    // 7.  We skip over composites.  The first one we skip
    // will be 49.
    if (wheel->data[i])
      continue;
    assert(i < wheel->capacity);
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

      set_wheel_idx_composite(wheel, idx_for_wheel_num(prod));
      printf("%zu %zu     %d %d %d\n", i, j, n, wheelnum, prod);
    }
  }
  wheel->idxmax = maxidx;
}

void ensure_mem_fit(wheelStruct *wheel, size_t sizeNeeded) {
  if (wheel->capacity <= sizeNeeded) {
    size_t oldCapacity = wheel->capacity;
    size_t newCapacity = 2*oldCapacity;
    if (newCapacity <= sizeNeeded)
      newCapacity = sizeNeeded;

    bool *new_mem = (bool *)realloc(
                              wheel->data,
                              newCapacity * sizeof(bool));
    if (new_mem == NULL) {
      perror("Failed to reallocate memory");
      exit(EXIT_FAILURE);
    }
    size_t added_byte_size =
      (newCapacity - oldCapacity) * sizeof(bool);
    memset(new_mem + oldCapacity, 0, added_byte_size);
    wheel->data = new_mem;
    wheel->capacity = newCapacity;
  }
}


void ensure_wheel_size(wheelStruct *wheel, size_t nprimes) {
  // Ensure that our wheel has been checked for composites
  // far enough.  Over-estimating is okay.
  // Rosser, J. Barkley, and Lowell Schoenfeld.  "Approximate
  // Formulas for Some Functions of Prime Numbers."
  int maxPrimeUpperBound = nprimes*(log(nprimes) + log(log(nprimes)));

  size_t maxIdxUpperBound = ceiling_index(maxPrimeUpperBound);
  if (maxIdxUpperBound > wheel->idxmax) {
    // The size of the array must be one greater than
    // the largest index (to account for zero).
    ensure_mem_fit(wheel, maxIdxUpperBound + 1);
    set_wheel_composites(wheel, maxIdxUpperBound);
  }
}

wheelStruct * create_wheel() {
  wheelStruct *wheel = malloc(sizeof(wheelStruct));

  // NOT FOR NOW: Initialize the wheel data by setting 7*7.
  wheel->idxmax = 0; // A value of 1 represents 7.
  //size_t idxFor49 = idx_for_wheel_num(49);
  //wheel->capacity = idxFor49 + 1;
  wheel->capacity = 1;
  wheel->data = calloc(wheel->capacity, sizeof(bool));
  //set_wheel_idx_composite(wheel, idxFor49);

  //ensure_wheel_size(wheel, 100);
  return wheel;
}


void get_array_of_primes(
            wheelStruct *wheel,
            size_t size,
            int primes[size]) {

  switch (size) {
    case 3: primes[2] = 5; [[fallthrough]];
    case 2: primes[1] = 3; [[fallthrough]];
    case 1: primes[0] = 2;
            return;
    default:
            primes[2] = 5;
            primes[1] = 3;
            primes[0] = 2;
  }

  ensure_wheel_size(wheel, size);

  // i iterates over the primes array (being filled) starting
  // with the fourth.
  // j iterates over the wheel (with skips for composites)
  // starting with the second (because the first is 1).
  for (size_t i=3, j=0; i < size; ++i) {
    do {
      ++j;
      assert(j <= wheel->idxmax);
    } while (wheel->data[j]);

    primes[i] = num_from_wheel(j);
  }
}


int next_prime(int start) {

  printf("%d\n", num_from_wheel(10));
  printf("%zu\n", idx_for_wheel_num(37));
  //printf("%zu\n", idx_for_wheel_num(39));
  //printf("%zu\n", idx_for_wheel_num(11111137));


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

