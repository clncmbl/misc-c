#include <stdlib.h>
#include <stdio.h>
#include <string.h>  // for memcpy
#include <stdbool.h>

int main(int argc, char* argv[argc+1]) {

  void strings_to_doubles(
      size_t count,
      char* input_strings[static count],
      double doubles[static count]) {

    for (size_t i = 0; i < count; i++) {
      doubles[i] = strtod(input_strings[i], NULL);
    }
  }

  void merge_sort(size_t count, double doubles[static count]) {
    double workingcopy[count] = {};
    memcpy(workingcopy, doubles, count*sizeof(double));
    size_t rightcnt = count/2;
    size_t leftcnt = count - rightcnt;
    double* left = workingcopy;
    double* right = workingcopy + leftcnt;

    if (leftcnt > 1) {
      merge_sort(leftcnt, left);
    }
    if (rightcnt > 1) {
      merge_sort(rightcnt, right);
    }

    for (size_t i=0, j=0, k=0; i < count; i++) {
      // j is left counter, k is right counter
      //printf("i=%zu, j=%zu, k=%zu\n", i, j, k);
      
      if (k == rightcnt ||
          (j < leftcnt && left[j] <= right[k])) {
        doubles[i] = left[j++];
      } else {
        doubles[i] = right[k++];
      }
      //printf("i=%zu, j=%zu, k=%zu\n", i, j, k);
    }

    /*
    printf("--------\n");
    for (size_t i = 0; i < leftcnt; i++) {
      printf("left %f\n", left[i]);
    }
    for (size_t i = 0; i < rightcnt; i++) {
      printf("right %f\n", right[i]);
    }
    */
  }

  double numbers[argc-1];

  strings_to_doubles(argc-1, &argv[1], numbers);

  merge_sort(argc-1, numbers);

  bool error = false;
  for (size_t i = 0; i < argc-1; i++) {
    printf("%f\n", numbers[i]);

    double lastval;
    if (i) {
      if (numbers[i] < lastval) {
        error = true;
        printf("Not in order\n");
      }
    }
    lastval = numbers[i];
  }

  if (!error) {
    printf("SUCCESS!\n");
  }
}

