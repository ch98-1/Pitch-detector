#ifndef CALCULATE_FREQUENCY_H
#define CALCULATE_FREQUENCY_H

#include <stdio.h>
#include <math.h>

#include "kiss_fft.h"
#include "kiss_fftr.h"


/* struct for storing frequency data */
typedef struct {
  float* frequency; /* frequency list ordered by weight */
  float* weight; /* frequency weight */
  long int num; /* number of frequency found */
} frequency_list;

/* estimate the frequency of the end of the data */
frequency_list calculate_frequency(float* data, long int length, float sample_rate);

/* use single fft to calculate the frequency, with weight being the peak squared to average ratio */
frequency_list calculate_frequency_fft(float* data, long int length, long int fft_length, float sample_rate);

int clean_frequency_list(frequency_list frequencies);

#endif /* CALCULATE_FREQUENCY_H */
