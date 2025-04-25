#include <stdio.h>

#include "main.h"

/* pre defined size of matrix - for simplicity I just want cubes */
#define SIZE 3

/* Function definitions for rotation and printing */
static void rotate_matrix(int mat[SIZE][SIZE]);
static void print_matrix(int mat[SIZE][SIZE]);

/* Variable which controls the rotation direction of the matrix */
static int clockwise = 0;

/*
 * @brief: Take in a matrix of size*size and rotates it based on a rotation direction parameter.
 * The matrix provided as input will be updated inside this function.
 * @params: int mat[SIZE][SIZE] - matrix of size*size
 * @return: void
 * */
static void rotate_matrix(int mat[SIZE][SIZE]) {
  int temp[SIZE][SIZE] = { 0 };
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      if (clockwise) {
        temp[i][j] = mat[SIZE - j - 1][i];
      } else {
        temp[i][j] = mat[j][SIZE - i - 1];
      }
    }
  }
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      mat[i][j] = temp[i][j];
    }
  }
}

/*
 * @brief: Iterates through the matrix and prints its content.
 * @params: int mat[SIZE][SIZE] - matrix of size*size
 * @return: void
 * */
static void print_matrix(int mat[SIZE][SIZE]) {
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      printf("%d, ", mat[i][j]);
    }
    printf("\r\n");
  }
  printf("\r\n");
}

/*
 * @brief: Main function.
 * @params: void
 * @return: int - Return code should always be 0.
 * */
int main(void) {
  int mat[SIZE][SIZE] = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
  };
  print_matrix(mat);
  rotate_matrix(mat);
  print_matrix(mat);

  /* Change matrix rotation direction */
  clockwise = 1;
  rotate_matrix(mat);
  print_matrix(mat);
  rotate_matrix(mat);
  print_matrix(mat);
  return 0;
}

