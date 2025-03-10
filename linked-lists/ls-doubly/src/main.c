#include "main.h"

struct Node {
  int id;
  struct Node* prev;
  struct Node* next;
};

static struct Node* createNode(int id);
static void linkNodes(struct Node* node, struct Node* prev, struct Node* next);
static void printList(struct Node* head);
static struct Node* reverseList(struct Node* head);

static struct Node* createNode(int id) {
  struct Node* new = (struct Node*)malloc(sizeof(struct Node));
  new->id = id;
  new->prev = NULL;
  new->next = NULL;

  return new;
}

static void linkNodes(struct Node* node, struct Node* prev, struct Node* next) {
  if (node) {
    if (prev) {
      node->prev = prev;
    }
    if (next) {
      node->next = next;
    }
  }
}

static void printList(struct Node* head) {
  struct Node* current = head;
  while (current) {
    printf("ID: %d\r\n", current->id);
    current = current->next;
  }
}

static struct Node* reverseList(struct Node* head) {
  struct Node* current = head;
  struct Node* temp = NULL;

  while (current) {
    temp = current->prev;
    current->prev = current->next;
    current->next = temp;

    current = current->prev;
  }

  return temp ? temp->prev : head;
}

int main(void)
{
  struct Node* n1 = createNode(1);
  struct Node* n2 = createNode(2);
  struct Node* n3 = createNode(3);
  struct Node* n4 = createNode(4);
  struct Node* n5 = createNode(5);

  linkNodes(n1, NULL, n2);
  linkNodes(n2, n1, n3);
  linkNodes(n3, n2, n4);
  linkNodes(n4, n3, n5);
  linkNodes(n5, n4, NULL);

  printList(n1);
  printf("\r\n");

  struct Node* newHead = NULL;
  newHead = reverseList(n1);
  printList(newHead);
  printf("\r\n");

  return 0;
}
