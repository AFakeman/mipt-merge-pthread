#include "thread_pool.h"

#include <assert.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdlib.h>

ThreadTask* PopTask(ThreadPool* self) {
  void* result;
  pthread_mutex_lock(&(self->stack_mutex_));
  result = StackPop(&(self->stack_));
  pthread_mutex_unlock(&(self->stack_mutex_));
  return result;
}

void PushTask(ThreadPool* self, ThreadTask* data) {
  pthread_mutex_lock(&(self->stack_mutex_));
  StackPush(&(self->stack_), data);
  pthread_mutex_unlock(&(self->stack_mutex_));
}

void* ThreadPoolThreadLoop(void* in) {
  ThreadPool* pool = in;
  ThreadTask* task = NULL;

  while ((task = PopTask(pool)) || !atomic_load(&(pool->shutdown_))) {
    if (!task)
      continue;
    task->task(task->data);
    if (task->dep_) {
      if (atomic_fetch_sub(&(task->dep_->pending_), 1) == 1) {
        ThreadPoolAddTask(pool, task->dep_);
      }
    }
    free(task);
  }
  return NULL;
}

void ThreadPoolInit(ThreadPool* self, size_t thread_count) {
  self->thread_count_ = thread_count;
  self->threads_ = malloc(thread_count * sizeof(pthread_t));
  atomic_store(&(self->shutdown_), 0);
  atomic_store(&(self->done_), 0);
  StackInit(&(self->stack_));
  pthread_mutex_init(&(self->stack_mutex_), NULL);
}

void ThreadPoolDestroy(ThreadPool* self) {
  if (!atomic_load(&(self->done_))) {
    ThreadPoolShutdown(self);
    ThreadPoolJoin(self);
  }
  free(self->threads_);
  assert(StackEmpty(&(self->stack_)));
  pthread_mutex_destroy(&(self->stack_mutex_));
}

void ThreadPoolStart(ThreadPool* self) {
  size_t thread;
  for (thread = 0; thread < self->thread_count_; thread++) {
    pthread_create(self->threads_ + thread, NULL, ThreadPoolThreadLoop, self);
  }
}

void ThreadPoolShutdown(ThreadPool* self) {
  atomic_store(&(self->shutdown_), 1);
}

void ThreadPoolCreateTask(ThreadTask* task, void* data, void (*func)(void*)) {
  task->data = data;
  atomic_store(&(task->pending_), 0);
  task->dep_ = NULL;
  task->task = func;
}

void ThreadPoolSetDependant(ThreadTask* parent, ThreadTask* dep) {
  parent->dep_ = dep;
  atomic_fetch_add(&(dep->pending_), 1);
}

void ThreadPoolAddTask(ThreadPool* self, ThreadTask* task) {
  if (atomic_load(&(self->shutdown_))) {
    return;
  }
  PushTask(self, task);
}

void ThreadPoolJoin(ThreadPool* self) {
  int i;
  for (i = 0; i < self->thread_count_; ++i) {
    pthread_join(self->threads_[i], NULL);
  }
  atomic_store(&(self->done_), 1);
}