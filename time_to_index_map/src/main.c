#include <stdlib.h>
#include "main.h"

#define PERCENT_100 100
#define BIT_DEPTH 256
#define TIME_600 600
#define TIME_1200 1200
#define TIME_1800 1800

/* Keep track of previous index so we can prevent bloating the terminal with print statements */
static uint16_t prevIndex = 0xff;
static uint16_t index = 0;

/*
 * @brief
 * Function to calculate the index of the parameter that displays the progress bar.
 * Potential values are between 0 and 255.
 * @params
 * uint16_t time - The remaining time on the timer.
 * uint16_t factor - The division factor * 100 mapped between the initialized time and 255.
 * By making this value a factor 100 bigger than the actual division factor we get rid of floating
 * point numbers.
 * */
static void handletime(uint16_t time, uint16_t factor) {
  index = (BIT_DEPTH - 1) - (time * factor / PERCENT_100);
  if (prevIndex != index) {
    prevIndex = index;
    printf("time: %lu | idx: %lu\r\n", time, index);
  }
}

int main(void) {
  /* Define the 3 cases */
  /*uint16_t time = TIME_600;*/
  /*uint16_t time = TIME_1200;*/
  uint16_t time = TIME_1800;

  /* Calculate the factor to get rid of floating point numbers */
  uint16_t factor = BIT_DEPTH * PERCENT_100 / time;

  while (time) {
    /* Iterate until we hit 0 seconds */
    handletime(time--, factor);
  }

  return 0;
}

