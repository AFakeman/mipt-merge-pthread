#include <stdatomic.h>
#include <stddef.h>

// Sorts |array| of |size| in ascending order
// using |threads| threads. Small subarrays are
// sorted using qsort() (default threshold is 64,
// can be set using SetChunkSize()).
// Returns time spent sorting (in seconds).
double MergeSort(int* array, size_t size, size_t threads);


// Wrapper over default qsort().
// Returns time spent sorting (in seconds).
double QuickSort(int* arr, size_t size);

void SetChunkSize(size_t size);