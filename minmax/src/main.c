#include "main.h"

#define MAX_ADC_INDEX 10

static uint16_t adcAdvg = 0;
static uint16_t adcBuffer[MAX_ADC_INDEX] = { 0 };

static uint16_t average(const uint16_t* buf, const uint8_t size);

static uint16_t average(const uint16_t* buf, const uint8_t size) {
  uint8_t i = 0;
  uint16_t min = 0xffff;
  uint16_t max = 0;
  uint16_t result = 0;
  uint32_t sum = 0;

  for (i = 0; i < size; i++) {
    if (buf[i] < min) {
      min = buf[i];
    }
    if (buf[i] > max) {
      max = buf[i];
    }
    sum = sum + buf[i];
  }

  result = (sum - min - max) / (size - 2);
  return result;
}

int main(void) {
  uint8_t i = 0;
  uint16_t min = 0xffff;
  uint16_t max = 0;

  printf("Original array:\r\n");
  for (i = 0; i < MAX_ADC_INDEX; i++) {
    adcBuffer[i] = rand() % 4095;
    if (adcBuffer[i] < min) {
      min = adcBuffer[i];
    }
    if (adcBuffer[i] > max) {
      max = adcBuffer[i];
    }
    printf("%d, ", adcBuffer[i]);
  }
  printf("\r\nmin: %d | max: %d\r\n", min, max);

  printf("\r\n\r\n");

  printf("Average: %d\r\n", average((const uint16_t*)adcBuffer, MAX_ADC_INDEX));

  return 0;
}
