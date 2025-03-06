#include "main.h"

#define ROWS 3

static int** imageSmoother(int** img, int imgSize, int* imgColSize, int* returnSize, int** returnColumnSizes);
static int** imageSmoother(int** img, int imgSize, int* imgColSize, int* returnSize, int** returnColumnSizes) {
  int** result = (int**)malloc(imgSize * sizeof(int*));
  int sum = 0;
  int num = 0;

  for (int i = 0; i < imgSize; i++) {
    result[i] = (int*)malloc(imgColSize[i] * sizeof(int));
  }

  for (int rows = 0; rows < 3; rows++) {
    for (int cols = 0; cols < 3; cols++) {
      sum = 0;
      num = 0;
      for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
          int x = i + rows;
          int y = j + cols;

          if ((x >= 0) && (x < 3) && (y >= 0) && (y < 3)) {
            sum = sum + img[x][y];
            num++;
          }
        }
      }
      result[rows][cols] = (sum / num);
    }
  }

  return result;
}

int main(void)
{
  int imgRows = ROWS;
  int imgCols[] = { 3, 3, 3 };
  int** img = (int**)malloc(imgRows * sizeof(int*));
  for (int i = 0; i < imgRows; i++) {
    img[i] = (int*)malloc(imgCols[i] * sizeof(int));
  }

  img[0][0] = 100; img[0][1] = 200; img[0][2] = 100;
  img[1][0] = 200; img[1][1] =  50; img[1][2] = 200;
  img[2][0] = 100; img[2][1] = 200; img[2][2] = 100;

  for (int i = 0; i < imgRows; i++) {
    for (int j = 0; j < imgCols[i]; j++) {
      printf("%3d, ", img[i][j]);
    }
    printf("\r\n");
  }
  printf("\r\n");


  int returnSize = 0;
  int** returnColumnSizes = NULL;

  int** newImg = imageSmoother(img, imgRows, imgCols, &returnSize, returnColumnSizes);

  for (int i = 0; i < imgRows; i++) {
    for (int j = 0; j < imgCols[i]; j++) {
      printf("%3d, ", newImg[i][j]);
    }
    printf("\r\n");
  }

  return 0;
}
