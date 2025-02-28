#include "main.h"

/*#define SIZE 10*/
#define SIZE 3

static int bs(int* arr, int size, int target);
static void bubblesort(int* arr, int size);

static int bs(int* arr, int size, int target) {
  int start = 0;
  int end = size;
  int mid = 0;
  int ret = -1;

  while (start <= end) {
    mid = start + (end - start) / 2;
   if (target == arr[mid]) {
     ret = arr[mid];
     break;
   } else if (target < arr[mid]) {
     end = mid - 1;
   } else {
     start = mid + 1;
   }
  }

  return ret;
}

static void bubblesort(int* arr, int size) {
  bool swapped = false;
  int it = 0;

  for (int i = 0; i < size; i++) {
    swapped = false;
    for (int j = 0; j < (size - 1); j++) {
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

  printf("Number of iterations: %d\r\n", it);
}

int main(void)
{
  int arr[SIZE] = { 0 };
  printf("Original array:\r\n");
  for (int i = 0; i < SIZE; i++) {
    /*arr[i] = rand() % 100;*/
    arr[i] = i;
    printf("%d, ", arr[i]);
  }

  int size = sizeof(arr) / sizeof(arr[0]);
  int target = 0;

  bubblesort(arr, size);
  printf("Sorted array:\r\n");
  for (int i = 0; i < size; i++) {
    printf("%d, ", arr[i]);
  }
  printf("\r\n");

  target = 0;
  printf("idx: %d\r\n", bs(arr, size, target));

  target = 1;
  printf("idx: %d\r\n", bs(arr, size, target));

  target = 2;
  printf("idx: %d\r\n", bs(arr, size, target));

  /*target = 4;*/
  /*printf("idx: %d\r\n", bs(arr, size, target));*/
  /**/
  /*target = 10;*/
  /*printf("idx: %d\r\n", bs(arr, size, target));*/

  return 0;
}
