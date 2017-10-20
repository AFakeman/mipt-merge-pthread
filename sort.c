#include "sort.h"

#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/time.h>

#include "thread_pool.h"
#include "misc.h"

typedef struct {
  int* array;
  int* buffer;
  size_t size;
} SortTask;

static size_t chunk_size = 64;

int sort_compare(const void* a, const void* b) {
  int a_val = *((const int*)a);
  int b_val = *((const int*)b);
  return a_val - b_val;
}

// Merge two sequential arrays, one with |left_count| elements starting at
// zero, another at |left_count| with |right_count| elements.
// Buffer is assumed to have enough memory to store |left_count + right_count|.
void Merge(int* left, int* buffer, size_t left_count, size_t right_count) {
  size_t length = left_count + right_count;
  size_t left_cursor = 0;
  size_t right_cursor = left_count;
  size_t buffer_cursor = 0;
  while (buffer_cursor < length) {
    if (left_cursor == left_count ||
        (right_cursor < length && left[right_cursor] < left[left_cursor])) {
      buffer[buffer_cursor] = left[right_cursor];
      ++right_cursor;
    } else if (right_cursor == length ||
               left[right_cursor] >= left[left_cursor]) {
      buffer[buffer_cursor] = left[left_cursor];
      ++left_cursor;
    }
    buffer_cursor++;
  }
  memcpy(left, buffer, sizeof(int) * length);
}

// Sort an array |arr| with length |count| recursively. If an array is
// not bigger than |min_count|, apply quick sort algorithm.
void SortJob(void* in) {
  SortTask* task = in;
  if (task->size <= chunk_size) {
    qsort(task->array, task->size, sizeof(int), sort_compare);
  } else {
    Merge(task->array, task->buffer, task->size / 2,
          task->size - task->size / 2);
  }
  free(task);
}

void StopJob(void* in) {
  ThreadPool* pool = in;
  ThreadPoolShutdown(pool);
}

// Recursively add tasks to sort an array.
// Returns the parent task.
ThreadTask* AddRecursiveTask(ThreadPool* pool,
                             int* array,
                             int* buffer,
                             size_t size) {
  ThreadTask* task = malloc(sizeof(ThreadTask));
  SortTask* data = malloc(sizeof(SortTask));

  data->array = array;
  data->buffer = buffer;
  data->size = size;

  ThreadPoolCreateTask(task, data, SortJob);

  if (size >= chunk_size) {
    size_t left_size = size / 2;
    size_t right_size = size - left_size;

    ThreadTask* left_task = AddRecursiveTask(pool, array, buffer, left_size);
    ThreadTask* right_task = AddRecursiveTask(pool, array + left_size,
                                              buffer + left_size, right_size);

    ThreadPoolSetDependant(left_task, task);
    ThreadPoolSetDependant(right_task, task);
  } else {
    ThreadPoolAddTask(pool, task);
  }

  return task;
}

void SetChunkSize(size_t size) {
  chunk_size = size;
}

double QuickSort(int* arr, size_t size) {
  struct timeval begin;
  struct timeval end;
  gettimeofday(&begin, NULL);
  qsort(arr, size, sizeof(int), sort_compare);
  gettimeofday(&end, NULL);
  return timediff(&end, &begin);
}

double MergeSort(int* array, size_t size, size_t threads) {
  int* buffer = malloc(sizeof(int) * size);
  struct timeval time_begin;
  struct timeval time_end;
  ThreadPool thread_pool;
  ThreadTask* end_task = malloc(sizeof(ThreadTask));
  ThreadTask* sort_task;
  gettimeofday(&time_begin, NULL);
  ThreadPoolInit(&thread_pool, threads);

  ThreadPoolCreateTask(end_task, &thread_pool, StopJob);
  sort_task = AddRecursiveTask(&thread_pool, array, buffer, size);
  ThreadPoolSetDependant(sort_task, end_task);
  ThreadPoolStart(&thread_pool);
  ThreadPoolJoin(&thread_pool);

  ThreadPoolDestroy(&thread_pool);
  free(buffer);
  gettimeofday(&time_end, NULL);
  return timediff(&time_end, &time_begin);
}
