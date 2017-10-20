#include "stack.h"

#include <stdlib.h>

void StackInit(Stack* self) {
  self->top_ = NULL;
}

void* StackTop(const Stack* self) {
  if (!self->top_)
    return NULL;
  else
    return self->top_->data;
}

void* StackPop(Stack* self) {
  if (!self->top_)
    return NULL;
  void* data = self->top_->data;
  StackNode* to_cleanup = self->top_;
  self->top_ = to_cleanup->next;
  free(to_cleanup);
  return data;
}

void StackPush(Stack* self, void* data) {
  StackNode* new_top = malloc(sizeof(StackNode));
  new_top->next = self->top_;
  new_top->data = data;
  self->top_ = new_top;
}

int StackEmpty(Stack* self) {
  return self->top_ == NULL;
}
