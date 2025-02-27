#include "main.h"

struct Node {
  uint8_t id;
  struct Node* next;
};

struct Stack {
  uint8_t size;
  struct Node* head;
};

static void printStack(struct Stack* stack);
static struct Stack* createStack(void);
static void push(struct Stack* stack, uint8_t id);
static void pop(struct Stack* stack);

static void reverseStack(struct Stack* stack);

static void printStack(struct Stack* stack) {
  if (stack) {
    printf("\r\n");
    if (stack->head) {
      printf("Current stack:\r\n");
      printf("Stack size: %d\r\n", stack->size);
      struct Node* current = stack->head;
      while (current) {
        printf("Node: %d\r\n", current->id);
        current = current->next;
      }
    } else {
      printf("Stack empty..\r\n");
      printf("Stack size: %d\r\n", stack->size);
    }
    printf("\r\n");
  }
}

static struct Stack* createStack(void) {
  struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
  stack->size = 0;
  stack->head = NULL;

  return stack;
}

static void push(struct Stack* stack, uint8_t id) {
  if (stack) {
    printf("Pushing %d onto stack..\r\n", id);
    struct Node* new = (struct Node*)malloc(sizeof(struct Node));
    if (!new) {
      printf("Error creating new node..\r\n");
      exit(1);
    }
    new->id = id;
    if (!stack->head) {
      printf("Creating head: %d..\r\n", id);
      new->next = NULL;
    } else {
      printf("Pushing head: %d..\r\n", id);
      new->next = stack->head;
    }
    stack->head = new;
    stack->size++;
  }
}

static void pop(struct Stack* stack) {
  if (stack) {
    if (stack->head) {
      printf("Popping head: %d..\r\n", stack->head->id);
      struct Node* temp = stack->head;
      stack->head = stack->head->next;
      stack->size--;
      free(temp);
      temp = NULL;
    }
  }
}

static void reverseStack(struct Stack* stack) {
  if (stack->head) {
    printf("Reversing stack..\r\n");
    struct Node* current = stack->head;
    struct Node* next = NULL;
    struct Node* prev = NULL;

    while (current) {
      next = current->next;
      current->next = prev;
      prev = current;
      current = next;
    }

    stack->head = prev;
  }
}

int main(void)
{
  struct Stack* stack = createStack();
  if (!stack) {
    printf("Error creating stack..\r\n");
    exit(1);
  }

  printStack(stack);

  push(stack, 1);
  push(stack, 2);
  push(stack, 3);
  push(stack, 4);
  printStack(stack);
  
  reverseStack(stack);
  printStack(stack);

  pop(stack);
  pop(stack);
  pop(stack);
  pop(stack);
  pop(stack);
  pop(stack);
  pop(stack);
  printStack(stack);

  return 0;
}
