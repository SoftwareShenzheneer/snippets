#include "main.h"
#include <math.h>

#define SIZE 10

static int cb(bool* arr, int size);
static void print(bool* arr, int size);
static void fillArr(bool* arr, int size, int idx);

static int cb(bool* arr, int size) {
  int ret = -1;
  int root = sqrt(size);

  int i = root;
  for (; i < size; i += root) {
    if (arr[i]) {
      break;
    }
  }

  i -= root;

  for (int j = 0; j < root && j < size; i++, j++) {
    if (arr[i]) {
      ret = i;
      break;
    }
  }

  return ret;
}

static void fillArr(bool* arr, int size, int idx) {
  for (int i = idx; i < size; i++) {
    arr[i] = 1;
  }
}

static void print(bool* arr, int size) {
  for (int i = 0; i < size; i++) {
    if (arr[i]) {
      printf("arr[%d]: true\r\n", i + 1);
    } else {
      printf("arr[%d]: false\r\n", i + 1);
    }
  }
}

int main(void)
{
  bool arr[SIZE] = { 0 };
  int size = sizeof(arr) / sizeof(arr[0]);
  int breakpoint = 0;

  srand(8);
  int idx = rand() % SIZE;
  fillArr(arr, size, idx);

  breakpoint = cb(arr, size);
  printf("Breakpoint: %d\r\n", breakpoint);
  print(arr, size);

  return 0;
}
