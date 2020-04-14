#include "calculate_frequency.h"

/* estimate the frequency of the end of the data */
frequency_list calculate_frequency(float* data, long int length, float sample_rate){
  frequency_list frequencies; /* frequency list to return */


  float max_mag = 0; /* max magnitude */
  float max_f = 0; /* frequency at current max magnitude */
  long int i;
  for (i = 0; i < 5; i++){ /* loop through different fft sizes */
    frequency_list out = calculate_frequency_fft(data, length, length/pow(2, i), sample_rate); /* run fft for that length */
    if(out.weight[0] > max_mag){ /* if weight is the largest */
      max_mag = out.weight[0]; /* set max magnitude and frequency */
      max_f = out.frequency[0];
    }
    clean_frequency_list(out); /* clean up after each loop */
  }



  if (max_mag > 300) { /* if max is larger than threshold of frequency match */
    frequencies.num = 1; /* fill data */
    frequencies.frequency = malloc(sizeof(float));
    *(frequencies.frequency) = max_f;
    frequencies.weight = malloc(sizeof(float));
    *(frequencies.weight) = max_mag;
  }
  else { /* if no suitable frequency peak is found */
    frequencies.num = 0; /* fill zeros */
    frequencies.frequency = malloc(sizeof(float));
    *(frequencies.frequency) = 0;
    frequencies.weight = malloc(sizeof(float));
    *(frequencies.weight) = 0;
  }

  return frequencies; /* return increasing frequency to test values */
}

/* use single fft to calculate the frequency */
frequency_list calculate_frequency_fft(float* data, long int length, long int fft_length, float sample_rate){
  frequency_list frequencies; /* frequency list to return */

  long int length_short = (fft_length % 2) ? fft_length - 1 : fft_length; /* calculate closest even number not exceeding the wanted fft length */

  float* data_short = data + (length - length_short); /* do some memory math to come up with address for end part of data */

  long int length_used = 32768;

  float* data_used = calloc(length_used, sizeof(float)); /* create zero array */
  memcpy(data_used, data_short, length_short); /* copy wanted data */

  kiss_fftr_cfg  fftr_cfg = kiss_fftr_alloc(length_used ,0 ,NULL ,NULL); /* initialise fft for 1d real  */
  kiss_fft_cpx* f_data = malloc(sizeof(kiss_fft_cpx) * (length_used/2 + 1)); /* allocate memory for frequency data */

  kiss_fftr(fftr_cfg, data_used, f_data);

  float max_mag = 0; /* max magnitude */
  float max_f = 0; /* frequency at current max magnitude */
  float sum = 0; /* sum of all frequency bin magnitudes */
  float average = 0; /* average magnitude */
  long int i;
  for (i = 1; i < length_used/2 + 1; i++){ /* loop through all bins except for dc */
    double magnitude = sqrt(f_data[i].r*f_data[i].r  + f_data[i].i*f_data[i].i); /* calculate magnitude of the complex at that bin */
    sum += magnitude; /* add to sum */
    if (magnitude > max_mag){ /* if it is the bin with biggest magnitude */
      max_mag = magnitude;  /* make nex max frequency and magnitude */
      max_f = (float)i * (sample_rate / length_used);
    }
  }
  average = sum / length_used; /* get average */



    frequencies.num = 1; /* fill data */
    frequencies.frequency = malloc(sizeof(float));
    frequencies.frequency[0] = max_f;
    frequencies.weight = malloc(sizeof(float));
    frequencies.weight[0] = max_mag * max_mag / average;

    free(fftr_cfg); /* free fft configs */

    free(f_data); /* free frequency data */

    free(data_used); /* free copy of input data */

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
