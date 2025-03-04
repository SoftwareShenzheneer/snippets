#include "main.h"

static int compare(const void* a, const void* b);
static int partition_problem(int* arr, int size);

static int compare(const void* a, const void* b) {
  return (*(int*)b - *(int*)a);
}

static int partition_problem(int* arr, int size) {
  int sum = 0;
  for (int i = 0; i < size; i++) {
    sum += arr[i];
  }

  if (sum % 2) {
    return 0;
  }

  int target = sum / 2;
  int subsetA = 0;
  int subsetB = 0;

  qsort(arr, size, sizeof(int), compare);

  for (int i = 0; i < size; i++) {
    if (subsetA + arr[i] <= target) {
      subsetA += arr[i];
    } else if (subsetB + arr[i] <= target) {
      subsetB += arr[i];
    } else {
      return 0;
    }
  }

  return (subsetA == target && subsetB == target);
}

int main(void)
{
  /*int arr[] = { 2, 4, 6, 8, 10, 12, 14, 16, 18, 20 };*/
  int arr[] = { 1, 5, 11, 5 };
  int size = sizeof(arr) / sizeof(arr[0]);
  int ret = partition_problem(arr, size);
  printf("Res: %d\r\n", ret);

  return 0;
}
