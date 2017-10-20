#include "misc.h"

double timediff(struct timeval* a, struct timeval* b) {
  return ((a->tv_sec - b->tv_sec) * 1e6 + (a->tv_usec - b->tv_usec)) / 1.0e6;
}

int CheckSorted(int* arr, size_t count) {
  size_t i = 0;
  for (i = 1; i < count; i++) {
    if (arr[i - 1] > arr[i]) {
      return 0;
    }
  }
  return 1;
}
