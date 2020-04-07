#include "audio_system.h"

/* initialise audio system
also sets up audio driver and device stuff */
audio_system* init_audio(){
  audio_system *system = malloc(sizeof(audio_system));

  system->data_path = SDL_GetBasePath();/* get data path */
  if (!system->data_path) {
      system->data_path = SDL_strdup("./");
  }

  system->input_on = 1; /* turn on input but turn off output */
  system->output_on = 0;

  system->output_vol = 1; /* set output volume to full */

  system->in_l_vol = 0.07; /* set initial volumes as zero */
  system->in_r_vol = 0.002;
  system->in_l_vol_max = 0.6;
  system->in_r_vol_max = 0.024;
  system->out_l_vol = 0;
  system->out_r_vol = 0.19;
  system->out_l_vol_max = 0;
  system->out_r_vol_max = 0.85;

  return system;
}


/* turn on and off audio input and output */
int turn_on_audio_input(audio_system* system){
  system->input_on = 1;
  return 0;
}

int turn_off_audio_input(audio_system* system){
  system->input_on = 0;
  return 0;
}

int turn_on_audio_output(audio_system* system){
  system->output_on = 1;
  return 0;
}

int turn_off_audio_output(audio_system* system){
  system->output_on = 0;
  return 0;
}


/* clean audio system
also cleans for audio driver and device stuff */
int clean_audio(audio_system* system){

  if (system != NULL) {
    if (system->data_path != NULL) {
      SDL_free(system->data_path);/* free data path variable */
    }
    free(system);
  }
  return 0;
}
