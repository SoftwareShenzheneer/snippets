#include "main.h"

static int number_of_patches(int* nums, int size, int target);

static int number_of_patches(int* nums, int size, int target) {
  int patches = 0;
  int index = 0;
  int sum = 0;

  while (sum < target) {
    if ((index < size) && (nums[index] <= sum + 1)) {
      sum = sum + nums[index];
      index++;
    } else {
      sum += sum + 1;
      patches++;
    }
  }

  return patches;
}

int main(void)
{
  int nums[] = { 1 };
  int size = sizeof(nums) / sizeof(nums[0]);
  int n = 6;

  printf("Min patches: %d\r\n", number_of_patches(nums, size, n));

  return 0;
}

