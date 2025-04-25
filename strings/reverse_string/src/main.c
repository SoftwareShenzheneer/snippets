#include <string.h>
#include "main.h"

static void reverse_string(char* inp);

static void reverse_string(char* inp) {
  int len = strlen(inp);
  for (int i = 0; i < len / 2; i++) {
    inp[i] = inp[i] + inp[len - i - 1];
    inp[len - i - 1] = inp[i] - inp[len - i -1];
    inp[i] = inp[i] - inp[len - i -1];
  }
}

int main(void) {
  char string[] = "Hello";
  printf("%s\r\n", string);
  reverse_string(string);
  printf("%s\r\n", string);
  return 0;
}

