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
      ret = mid;
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
    for (int j = 0; j < size - 1; j++) {
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

  printf("Iterations: %d\r\n", it);
}

int main(void)
{
  int arr[10] = { 0 };
  int size = sizeof(arr) / sizeof(arr[0]);
  int target = 0;

  printf("Unsorted array:\r\n");
  for (int i = 0; i < size; i++) {
    arr[i] = rand() % 120;
    printf("%d ", arr[i]);
  }
  printf("\r\n");

  bubblesort(arr, size);

  printf("Sorted array:\r\n");
  for (int i = 0; i < size; i++) {
    printf("%d ", arr[i]);
  }
  printf("\r\n");

  printf("Res: %d\r\n", bs(arr, size, target));

  return 0;
}
