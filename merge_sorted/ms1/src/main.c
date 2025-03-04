#include "main.h"

static void printArray(int* arr, int size) {
  for (int i = 0; i < size; i++) {
    printf("%d ", arr[i]);
  }
  printf("\r\n");
}

static void merge_sorted(int* arr1, int m, int* arr2, int n) {
  int ind1 = m - 1;
  int ind2 = n - 1;
  int index = m + n - 1;

  for (; index >= 0; index--) {
    if (ind1 >= 0 && (ind2 < 0 || arr1[ind1] > arr2[ind2])) {
      arr1[index] = arr1[ind1];
      ind1--;
    } else if (ind2 >= 0) {
      arr1[index] = arr2[ind2];
      ind2--;
    }
  }
}

int main(void)
{
  // Test Case 1: Normal Case
  int nums1[] = { 2, 2, 3, 0, 0, 0 };
  int nums2[] = { 1, 5, 6 };
  int m = 3;
  int n = 3;
  printArray(nums1, (m + n));
  printArray(nums2, n);
  merge_sorted(nums1, m, nums2, n);
  printArray(nums1, (m + n));
  printf("\r\n\r\n");

  // Test Case 2: nums1 is empty
  int nums1_2[] = { 0, 0, 0, 0, 0, 0 };
  int nums2_2[] = { 1, 2, 3 };
  m = 0;
  n = 3;
  printArray(nums1_2, (m + n));
  printArray(nums2_2, n);
  merge_sorted(nums1_2, m, nums2_2, n);
  printArray(nums1_2, (m + n));
  printf("\r\n\r\n");

  // Test Case 3: nums2 is empty
  int nums1_3[] = { 1, 2, 3, 0, 0, 0 };
  int nums2_3[] = {};
  m = 3;
  n = 0;
  printArray(nums1_3, (m + n));
  printArray(nums2_3, n);
  merge_sorted(nums1_3, m, nums2_3, n);
  printArray(nums1_3, (m + n));
  printf("\r\n\r\n");

  // Test Case 4: nums1 has larger elements at the start
  int nums1_4[] = { 10, 20, 30, 0, 0, 0 };
  int nums2_4[] = { 5, 15, 25 };
  m = 3;
  n = 3;
  printArray(nums1_4, (m + n));
  printArray(nums2_4, n);
  merge_sorted(nums1_4, m, nums2_4, n);
  printArray(nums1_4, (m + n));
  printf("\r\n\r\n");

  // Test Case 5: All elements in nums2 are smaller than nums1
  int nums1_5[] = { 5, 6, 7, 0, 0, 0 };
  int nums2_5[] = { 1, 2, 3 };
  m = 3;
  n = 3;
  printArray(nums1_5, (m + n));
  printArray(nums2_5, n);
  merge_sorted(nums1_5, m, nums2_5, n);
  printArray(nums1_5, (m + n));
  printf("\r\n\r\n");

  // Test Case 6: Arrays with duplicate elements
  int nums1_6[] = { 1, 3, 3, 0, 0, 0 };
  int nums2_6[] = { 1, 2, 2 };
  m = 3;
  n = 3;
  printArray(nums1_6, (m + n));
  printArray(nums2_6, n);
  merge_sorted(nums1_6, m, nums2_6, n);
  printArray(nums1_6, (m + n));
  printf("\r\n\r\n");

  // Test Case 7: Large arrays
  int nums1_7[] = { 1, 3, 5, 0, 0, 0, 0, 0, 0, 0 };
  int nums2_7[] = { 2, 4, 6, 7, 8, 9, 10 };
  m = 3;
  n = 7;
  printArray(nums1_7, (m + n));
  printArray(nums2_7, n);
  merge_sorted(nums1_7, m, nums2_7, n);
  printArray(nums1_7, (m + n));
  printf("\r\n\r\n");

  // Test Case 8: nums1 and nums2 have all the same values
  int nums1_8[] = { 3, 3, 3, 0, 0, 0 };
  int nums2_8[] = { 3, 3, 3 };
  m = 3;
  n = 3;
  printArray(nums1_8, (m + n));
  printArray(nums2_8, n);
  merge_sorted(nums1_8, m, nums2_8, n);
  printArray(nums1_8, (m + n));
  printf("\r\n\r\n");

  // Test Case 9: One element in each array
  int nums1_9[] = { 1, 0 };
  int nums2_9[] = { 2 };
  m = 1;
  n = 1;
  printArray(nums1_9, (m + n));
  printArray(nums2_9, n);
  merge_sorted(nums1_9, m, nums2_9, n);
  printArray(nums1_9, (m + n));
  printf("\r\n\r\n");

  // Test Case 10: Large numbers in arrays
  int nums1_10[] = { 1000000, 2000000, 3000000, 0, 0, 0 };
  int nums2_10[] = { 1500000, 2500000, 3500000 };
  m = 3;
  n = 3;
  printArray(nums1_10, (m + n));
  printArray(nums2_10, n);
  merge_sorted(nums1_10, m, nums2_10, n);
  printArray(nums1_10, (m + n));
  printf("\r\n\r\n");

  return 0;
}
