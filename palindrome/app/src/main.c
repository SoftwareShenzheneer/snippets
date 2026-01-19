#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "main.h"

static bool is_palindrome(char* inp);
static bool is_palindrome(char* inp) {
    bool ret = false;
    bool match = true;
    uint8_t len = strlen(inp);

    for (uint8_t i = 0; i < (len / 2); i++) {
        // printf("Comparing %d with %d\r\n", inp[i], inp[len - i - 1]);
        if (inp[i] != inp[len - i - 1]) {
            ret = false;
            break;
        }

        if (i == (len / 2) - 1) {
            ret = true;
        }
    }

    return ret;
}

int main(void) {
  char* msg = "racecar";
  printf("%d\r\n", is_palindrome("racecar"));
  printf("%d\r\n", is_palindrome("Tomi"));
  printf("%d\r\n", is_palindrome("TomimoT"));
  printf("%d\r\n", is_palindrome("racecars"));
  printf("%d\r\n", is_palindrome(""));
  // printf("%d\r\n", is_palindrome(NULL));
  return 0;
}

