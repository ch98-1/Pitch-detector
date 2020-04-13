#include "calculate_frequency.h"

/* estimate the frequency of the end of the data */
double calculate_frequency(float* data, long int length){
  static double i;
  i = i * 1.0004;
  i += 0.01;
  return i; /* return increasing frequency to test values */
}
