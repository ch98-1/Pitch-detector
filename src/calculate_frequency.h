#ifndef CALCULATE_FREQUENCY_H
#define CALCULATE_FREQUENCY_H

#include <stdio.h>
#include <math.h>

#include "kiss_fft.h"

/* estimate the frequency of the end of the data */
double calculate_frequency(float* data, long int length);

#endif /* CALCULATE_FREQUENCY_H */
