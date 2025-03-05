#include "main.h"

#define SIZE 3

static void printmat(int mat[SIZE][SIZE]);
static void multmat(int mata[SIZE][SIZE], int matb[SIZE][SIZE], int matres[SIZE][SIZE]);

static void printmat(int mat[SIZE][SIZE]) {
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      printf("%.2d, ", mat[i][j]);
    }
    printf("\r\n");
  }
}

static void multmat(int mata[SIZE][SIZE], int matb[SIZE][SIZE], int matres[SIZE][SIZE]) {
  int res = 0;
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      res = 0;
      for (int k = 0; k < SIZE; k++) {
        res = res + mata[i][k] * matb[k][j];
      }
      matres[i][j] = res;
    }
  }
}

int main(void)
{
  int mata[SIZE][SIZE] = {
    { 1, 2, 3 },
    { 4, 5, 6 }
    /*{ 7, 8, 9 }*/
  };
  int matb[SIZE][SIZE] = {
    { 9, 8 },
    { 6, 5 },
    { 3, 2 }
    /*{ 9, 8, 7 },*/
    /*{ 6, 5, 4 },*/
    /*{ 3, 2, 1 }*/
  };
  int matres[SIZE][SIZE] = { 0 };

  printf("Mat a:\r\n");
  printmat(mata);
  printf("\r\n");
  printf("Mat b:\r\n");
  printmat(matb);
  printf("\r\n");
  printf("Mat res:\r\n");
  printmat(matres);
  printf("\r\n");

  multmat(mata, matb, matres);

  printf("Mat a:\r\n");
  printmat(mata);
  printf("\r\n");
  printf("Mat b:\r\n");
  printmat(matb);
  printf("\r\n");
  printf("Mat res:\r\n");
  printmat(matres);
  printf("\r\n");

  return 0;
}
