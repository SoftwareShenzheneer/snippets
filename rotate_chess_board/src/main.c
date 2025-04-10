#include "main.h"

#define SIZE 8

static void print_field(int (*mat)[SIZE]);
static void rand_field(int (*mat)[SIZE]);
static void rotate_field(int (*mat)[SIZE]);

static void print_field(int (*mat)[SIZE]) {
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      printf("%2d, ", mat[i][j]);
    }
    printf("\r\n");
  }
  printf("\r\n");
}

static void rand_field(int (*mat)[SIZE]) {
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      mat[i][j] = rand() % 10;
    }
  }
}

static void rotate_field(int (*mat)[SIZE]) {
  for (int i = 0; i < SIZE / 2; i++) {
    for (int j = 0; j < SIZE; j++) {
      mat[i][j] = mat[i][j] + mat[SIZE - i - 1][SIZE - j - 1];
      mat[SIZE - i - 1][SIZE - j - 1] = mat[i][j] - mat[SIZE - i - 1][SIZE - j - 1];
      mat[i][j] = mat[i][j] - mat[SIZE - i - 1][SIZE - j - 1];
    }
  }
  
  if (SIZE % 2) {
    for (int i = 0; i < SIZE / 2; i++) {
      mat[SIZE / 2][i] = mat[SIZE / 2][i] + mat[SIZE / 2][SIZE - i - 1];
      mat[SIZE / 2][SIZE - i - 1] = mat[SIZE / 2][i] - mat[SIZE / 2][SIZE - i - 1];
      mat[SIZE / 2][i] = mat[SIZE / 2][i] - mat[SIZE / 2][SIZE - i - 1];
    }
  }
}

int main(void)
{
  int mat[SIZE][SIZE] = { 0 };
  rand_field(mat);
  print_field(mat);

  rotate_field(mat);
  print_field(mat);

  rotate_field(mat);
  print_field(mat);

  return 0;
}
