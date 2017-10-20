typedef struct StackNode {
  void* data;
  struct StackNode* next;
} StackNode;

typedef struct Stack {
  StackNode* top_;
} Stack;

// Initialize an empty stack.
void StackInit(Stack* self);

// Destroy a stack. It's assumed to be empty.
void StackDestroy(Stack* self);

// Return a topmost stack element without removing it
// from the stack. Empty stack returns NULL.
void* StackTop(const Stack* self);

// Return a topmost stack element, removing it from the stack.
// Empty stack returns NULL.
void* StackPop(Stack* self);

// Add an element to the top of the stack.
void StackPush(Stack* self, void* value);

// Returns 1 if the stack is empty, 0 otherwise.
int StackEmpty(Stack* self);
