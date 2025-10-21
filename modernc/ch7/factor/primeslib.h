#ifndef PRIMESLIB_H
#define PRIMESLIB_H

#include <stddef.h>

typedef struct wheelStruct wheelStruct;
struct wheelStruct;

wheelStruct * create_wheel();

void get_array_of_primes(
            wheelStruct *wheel,
            size_t size,
            int primes[size]);

int get_smallest_factor(wheelStruct *wheel, int n);

int next_prime(int start);

#endif // PRIMESLIB_H
