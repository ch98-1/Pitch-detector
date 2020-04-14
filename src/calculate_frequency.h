#ifndef CALCULATE_FREQUENCY_H
#define CALCULATE_FREQUENCY_H

#include <stdio.h>
#include <math.h>

#include "kiss_fft.h"

/* struct for storing frequency data */
typedef struct {
  double* frequency; /* frequency list ordered by weight */
  double* weight; /* frequency weight */
  long int num; /* number of frequency found */
} frequency_list;

/* estimate the frequency of the end of the data */
frequency_list calculate_frequency(float* data, long int length);

int clean_frequency_list(frequency_list frequencies);

#endif /* CALCULATE_FREQUENCY_H */
