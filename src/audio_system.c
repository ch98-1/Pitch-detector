#include "audio_system.h"

/* initialise audio system
also sets up audio driver and device stuff */
audio_system* init_audio(){
  audio_system *system = malloc(sizeof(audio_system));

  system->data_path = SDL_GetBasePath();/* get data path */
  if (!system->data_path) {
      system->data_path = SDL_strdup("./");
  }

  system->input_on = 0; /* keep inputs and outputs off at start */
  system->output_on = 0;

  system->input_stream = NULL;/* null out streams */
  system->output_stream = NULL;

  system->input_device = 0;/* set devices to nothing */
  system->output_device = 0;

  SDL_memset(&(system->wanted_audiospec), 0, sizeof(system->wanted_audiospec)); /* clear audiospecs */
  SDL_memset(&(system->input_audiospec), 0, sizeof(system->input_audiospec));
  SDL_memset(&(system->output_audiospec), 0, sizeof(system->output_audiospec));
  SDL_memset(&(system->wav_audiospec), 0, sizeof(system->wav_audiospec));

  system->wanted_audiospec.freq = 48000; /* set wanted audio spec */
  system->wanted_audiospec.format = AUDIO_F32SYS;
  system->wanted_audiospec.channels = 2;
  system->wanted_audiospec.samples = WANTED_AUDIO_BUFFER_SIZE;
  system->wanted_audiospec.callback = NULL;

  system->output_vol = 1; /* set output volume to full */

  system->in_l_vol = 0.07; /* set initial volumes as zero */
  system->in_r_vol = 0.002;
  system->in_l_vol_max = 0.6;
  system->in_r_vol_max = 0.024;
  system->out_l_vol = 0;
  system->out_r_vol = 0.19;
  system->out_l_vol_max = 0;
  system->out_r_vol_max = 0.85;

  set_audio_driver(0, system);
  set_input_audio_device(0, system);
  set_output_audio_device(0, system);

  turn_on_audio_input(system); /* turn inputs adn outpust on now that everything is set uo */
  turn_on_audio_output(system);

  return system;
}

/* functions to set audio device and drivers */
int set_audio_driver(int index, audio_system* system){
  if (index < SDL_GetNumAudioDrivers()) {
    SDL_AudioQuit();
    SDL_AudioInit(SDL_GetAudioDriver(index));
    system->audio_driver_index = index;
    return 0;
  }
  return 1;
}

int set_input_audio_device(int index, audio_system* system){
  if (index < SDL_GetNumAudioDevices(1)){
    if (system->input_device != 0) {
      SDL_CloseAudioDevice(system->input_device);/* close audio device */
    }
    if(system->input_stream != NULL){
      SDL_FreeAudioStream(system->input_stream);/* free audio stream */
    }
    system->input_device = SDL_OpenAudioDevice(SDL_GetAudioDeviceName(index, 1), 1, &(system->wanted_audiospec), &(system->input_audiospec), SDL_AUDIO_ALLOW_ANY_CHANGE);/* open device */
    system->input_stream = SDL_NewAudioStream(system->input_audiospec.format, system->input_audiospec.channels, system->input_audiospec.freq, system->wanted_audiospec.format, system->wanted_audiospec.channels, system->wanted_audiospec.freq);/* create new audio stream converter */
    system->audio_input_index = index;
    if(system->input_on){ /* set the on/off state to the correct one */
      turn_on_audio_input(system);
    }
    else {
      turn_off_audio_input(system);
    }
    return 0;
  }
  return 1;
}

int set_output_audio_device(int index, audio_system* system){
  if (index < SDL_GetNumAudioDevices(0)){
    if (system->output_device != 0) {
      SDL_CloseAudioDevice(system->output_device);/* close audio device */
    }
    if(system->output_stream != NULL){
      SDL_FreeAudioStream(system->output_stream);/* free audio stream */
    }
    system->output_device = SDL_OpenAudioDevice(SDL_GetAudioDeviceName(index, 0), 0, &(system->wanted_audiospec), &(system->output_audiospec), SDL_AUDIO_ALLOW_ANY_CHANGE);/* open device */
    system->output_stream = SDL_NewAudioStream(system->wanted_audiospec.format, system->wanted_audiospec.channels, system->wanted_audiospec.freq, system->output_audiospec.format, system->output_audiospec.channels, system->output_audiospec.freq);/* create new audio stream converter */
    system->audio_output_index = index;
    if(system->output_on){ /* set the on/off state to the correct one */
      turn_on_audio_output(system);
    }
    else {
      turn_off_audio_output(system);
    }
    return 0;
  }
  return 1;
}

/* do one step of the audio system */
int step_audio(audio_system* system){
  if (system->input_on){ /* handle input */

  }
  if (system->output_on){ /* handle output */

  }
  return 0;
}


/* clamp value to 0 to 1 */
float clamp_value(float in){
  if (in < 0){
    return 0;
  }
  else if (in > 1){
    return 1;
  }
  else {
    return in;
  }
}


/* turn on and off audio input and output */
int turn_on_audio_input(audio_system* system){
  SDL_PauseAudioDevice(system->input_device, 0);
  SDL_ClearQueuedAudio(system->input_device);
  system->input_on = 1;
  return 0;
}

int turn_off_audio_input(audio_system* system){
  SDL_PauseAudioDevice(system->input_device, 1);
  SDL_ClearQueuedAudio(system->input_device);
  system->input_on = 0;
  return 0;
}

int turn_on_audio_output(audio_system* system){
  SDL_PauseAudioDevice(system->output_device, 0);
  SDL_ClearQueuedAudio(system->output_device);
  system->output_on = 1;
  return 0;
}

int turn_off_audio_output(audio_system* system){
  SDL_PauseAudioDevice(system->output_device, 1);
  SDL_ClearQueuedAudio(system->output_device);
  system->output_on = 0;
  return 0;
}


/* clean audio system
also cleans for audio driver and device stuff */
int clean_audio(audio_system* system){



  if (system != NULL) {
    if (system->input_stream != NULL) {/* free audio streams */
      SDL_FreeAudioStream(system->input_stream);
    }
    if (system->output_stream != NULL) {
      SDL_FreeAudioStream(system->output_stream);
    }
    if (system->input_device != 0) {/* close audio device */
      SDL_CloseAudioDevice(system->input_device);
    }
    if (system->output_device != 0) {
      SDL_CloseAudioDevice(system->output_device);
    }
    if (system->data_path != NULL) {
      SDL_free(system->data_path);/* free data path variable */
    }
    SDL_AudioQuit();
    free(system);
  }
  return 0;
}
