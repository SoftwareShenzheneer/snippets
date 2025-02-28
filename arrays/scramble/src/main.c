#include <string.h>
#include <time.h>

#include "main.h"

static void print(uint64_t* arr, uint64_t* arr2, uint64_t size);
static void scramble(uint64_t* arr, uint64_t size);

static void print(uint64_t* arr, uint64_t* arr2, uint64_t size) {
  uint64_t j = 0;

  for (uint64_t i = 0; i < size; i++) {
    if (!(i % 10)) {
      printf("\r\n%4d | ", j);
      j++;
    }

    printf("%5d ", arr[i]);

    if (9 == (i % 10)) {
      printf(" | ");
      for (uint64_t k = 0; k < 10; k++) {
        uint64_t index = (i - 9) + k;
        printf("%5d ", arr2[index]);
      }
    }
  }
}

static void scramble(uint64_t* arr, uint64_t size) {
  for (uint64_t i = 0; i < size; i++) {
    uint64_t swapIndex = rand() % size;

    arr[i] = arr[i] + arr[swapIndex];
    arr[swapIndex] = arr[i] - arr[swapIndex];
    arr[i] = arr[i] - arr[swapIndex];
  }
}

static int sort(const void* arg1, const void* arg2) {
  int a = *(const int*)arg1;
  int b = *(const int*)arg2;
  int ret = 0;

  if (a > b) {
    ret = 1;
  }
  if (a < b) {
    ret = -1;
  }

  return ret;
}

static void bubblesort(uint8_t* arr, int size) {
  bool swapped = false;
  uint64_t it = 0;

  for (uint64_t i = 0; i < size; i++) {
    swapped = false;
    for (uint64_t j = 0; j < size - 1; j++) {
      it++;

      if (arr[j] > arr[j+1]) {
        arr[j] = arr[j] + arr[j+1];
        arr[j+1] = arr[j] - arr[j+1];
        arr[j] = arr[j] - arr[j+1];

        swapped = true;
      }
    }

    if (!swapped) {
      break;
    }
  }

  printf("Iterations: %lld\r\n", it);
}

int main(void)
{
  time_t seconds = time(NULL);
  srand(seconds);

  uint8_t arr[10000000] = { 0 };
  uint8_t arr2[10000000] = { 0 };
  uint32_t size = sizeof(arr) / sizeof(arr[0]);
  for (uint32_t i = 0; i < size; i++) {
    arr[i] = i;
    arr2[i] = i * 2;
  }

  scramble(arr2, size);
  /*print(arr, arr2, size);*/

  qsort(arr2, size, sizeof(uint64_t), sort);
  /*bubblesort(arr2, size);*/
  /*print(arr, arr2, size);*/


  time_t end = time(NULL);
  printf("\r\nTime spent: %ld\r\n", end - seconds);

  return 0;
}
