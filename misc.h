#include <stddef.h>

#include <sys/time.h>

// Assorted miscellaneous functions

// Returns how many seconds passed between |a| and |b|.
double timediff(struct timeval* a, struct timeval* b);

// Check if |arr| containing |count| elements is sorted.
int CheckSorted(int* arr, size_t count);
