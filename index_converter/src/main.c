#include "main.h"

#define INPUT 300
#define OUTPUT 180

static void remap_index(uint16_t in, uint16_t out);
static void remap_index(uint16_t in, uint16_t out) {
  for (uint16_t i = 0; i < out; i++) {
    uint16_t remapped = (INPUT * 100) / OUTPUT * i / 100;
    printf("%d: %d\r\n", i, remapped);
  }
}

int main(void) {
  remap_index(INPUT, OUTPUT);

  return 0;
}

