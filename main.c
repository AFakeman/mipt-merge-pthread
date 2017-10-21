#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "misc.h"
#include "sort.h"
#include "thread_pool.h"

char* kStatsFile = "stats.txt";
char* kDataFile = "data.txt";

void RandomFill(int* arr, size_t size) {
  size_t i;
  for (i = 0; i < size; i++) {
    arr[i] = rand() % 16;
  }
}

void PrintArray(int* arr, size_t size, FILE* file) {
  size_t i;
  for (i = 0; i < size; i++) {
    fprintf(file, "%d ", arr[i]);
  }
  fprintf(file, "\n");
}

int main(int argc, char* argv[]) {
  int* array_merge;
  int* array_qsort;
  size_t size;
  size_t chunk_size;
  int threads;
  double time_spent_merge;
  double time_spent_qsort;

  FILE* stats = fopen(kStatsFile, "w");
  FILE* data = fopen(kDataFile, "w");

  srand(time(NULL));

  assert(argc == 4);
  sscanf(argv[1], "%lu",  &size);
  sscanf(argv[2], "%lu", &chunk_size);
  sscanf(argv[3], "%d", &threads);

  array_merge = malloc(sizeof(int) * size);
  array_qsort = malloc(sizeof(int) * size);
  RandomFill(array_merge, size);
  memcpy(array_qsort, array_merge, sizeof(int) * size);
  PrintArray(array_merge, size, data);

  assert(!CheckSorted(array_merge, size));
  assert(!CheckSorted(array_qsort, size));

  SetChunkSize(chunk_size);
  time_spent_merge = MergeSort(array_merge, size, threads);
  PrintArray(array_merge, size, data);
  fclose(data);
  fprintf(stats, "n: %lu, m: %lu, P: %d, t: %lf\n", size, chunk_size, threads,
          time_spent_merge);
  fclose(stats);

  assert(CheckSorted(array_merge, size));
  assert(!CheckSorted(array_qsort, size));

  time_spent_qsort = QuickSort(array_qsort, size);

  printf("Merge: %lf\nqsort: %lf\n", time_spent_merge, time_spent_qsort);

  assert(CheckSorted(array_merge, size));
  assert(CheckSorted(array_qsort, size));

  free(array_merge);
  free(array_qsort);
}
