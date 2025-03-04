#include "main.h"

static void merge_sorted(int* arr1, int m, int* arr2, int n, int* res);

static void merge_sorted(int* arr1, int m, int* arr2, int n, int* res) {
  int i = 0;
  int j = 0;

  for (int k = 0; k < (m + n); k++) {
    if (i < m && (j >= n || arr1[i] <= arr2[j])) {
      res[k] = arr1[i++];
    } else {
      res[k] = arr2[j++];
    }
  }
}

int main(void)
{
  int arr1[] = { 1, 3, 5, 6 };
  int arr2[] = { 2, 4, 6, 8, 10 };
  int m = sizeof(arr1) / sizeof(arr1[0]);
  int n = sizeof(arr2) / sizeof(arr2[0]);
  int* res = (int*)malloc((m + n) * sizeof(int));
  int size = m + n;

  printf("m: %d | n: %d | size: %d\r\n", m, n, size);

  merge_sorted(arr1, m, arr2, n, res);

  for (int i = 0; i < size; i++) {
    printf("%d ", res[i]);
  }
  printf("\r\n");

  free(res);
  res = NULL;

  return 0;
}
