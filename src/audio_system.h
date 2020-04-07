#ifndef AUDIO_SYSTEMS_H
#define AUDIO_SYSTEMS_H

#include <stdlib.h>

#include "kiss_fft.h"
#include "SDL.h"


/* struct for storing audio system data */
typedef struct {
  int input_on; /* on off state for audio io system */
  int output_on;

  float output_vol; /* output volume */

  char *data_path;

  float in_l_vol; /* volumes to display on volume bar */
  float in_r_vol;
  float in_l_vol_max;
  float in_r_vol_max;
  float out_l_vol;
  float out_r_vol;
  float out_l_vol_max;
  float out_r_vol_max;

} audio_system;

/* initialise audio system */
audio_system* init_audio();

/* turn on and off audio input and output */
int turn_on_audio_input(audio_system* system);
int turn_off_audio_input(audio_system* system);

int turn_on_audio_output(audio_system* system);
int turn_off_audio_output(audio_system* system);

/* clean audio system */
int clean_audio(audio_system* system);

#endif /* AUDIO_SYSTEMS_H */
