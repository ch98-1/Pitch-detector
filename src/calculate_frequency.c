#include "calculate_frequency.h"

/* estimate the frequency of the end of the data */
frequency_list calculate_frequency(float* data, long int length){
  frequency_list frequencies; /* frequency list to list */

  static double i;
  i = i * 1.0004;
  i += 0.01;

  frequencies.num = 1; /* fill data */
  frequencies.frequency = malloc(sizeof(double));
  *(frequencies.frequency) = i;
  frequencies.weight = malloc(sizeof(double));
  *(frequencies.weight) = 1;

  return frequencies; /* return increasing frequency to test values */
}

int clean_frequency_list(frequency_list frequencies){
  if (frequencies.frequency != NULL){ /* free data in frequency list */
    free(frequencies.frequency);
  }
  if (frequencies.weight != NULL){
    free(frequencies.weight);
  }
  return 0;
}
