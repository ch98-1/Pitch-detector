#include "audio_system.h"

/* audio is stored such that earliest time/oldest audio sample is at 0 and latest time/youngest audio sample is at the biggest address */

/* initialise audio system
also sets up audio driver and device stuff */
audio_system* init_audio(){

  audio_system *system = malloc(sizeof(audio_system));

  system->audio_system_mutex = SDL_CreateMutex(); /* set up mutexes for various operations */
  if (!system->audio_system_mutex) { /* handle error if mutex breaks */
    printf("Unable to create mutex: %s\n", SDL_GetError());
  }
  system->audio_input_mutex = SDL_CreateMutex();
  if (!system->audio_input_mutex) { /* handle error if mutex breaks */
    printf("Unable to create mutex: %s\n", SDL_GetError());
  }
  system->audio_output_mutex = SDL_CreateMutex();
  if (!system->audio_output_mutex) { /* handle error if mutex breaks */
    printf("Unable to create mutex: %s\n", SDL_GetError());
  }
  system->audio_frequency_mutex = SDL_CreateMutex();
  if (!system->audio_frequency_mutex) { /* handle error if mutex breaks */
    printf("Unable to create mutex: %s\n", SDL_GetError());
  }
  system->audio_volume_mutex = SDL_CreateMutex();
  if (!system->audio_volume_mutex) { /* handle error if mutex breaks */
    printf("Unable to create mutex: %s\n", SDL_GetError());
  }
  system->system_quit_mutex = SDL_CreateMutex();
  if (!system->system_quit_mutex) { /* handle error if mutex breaks */
    printf("Unable to create mutex: %s\n", SDL_GetError());
  }

  system->quit = 0; /* not quitting from the start */

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

  system->wanted_audiospec.freq = SAMPLE_RATE; /* set wanted audio spec */
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

  set_audio_driver(0, system); /* set the audio driver to first one */
  set_input_audio_device(0, system); /* also set the audio device input and output to first one */
  set_output_audio_device(0, system);

  turn_on_audio_input(system); /* turn inputs and outpust on now that everything is set uo */
  turn_on_audio_output(system);

  system->play_tone_end = 0; /* don't play any tone at start */
  system->play_tone_frequency = 0;

  system->input_monitor = 0; /* don't monitor input by default */


  system->playback_content = 0; /* don't playback anything by default */

  system->input_frequency_detected = 0; /* no input frequency */
  system->input_frequency = 0; /* initialise all input frequency stuff to 0 for now */
  system->input_midi_note = 0;
  system->input_midi_note_difference = 0;

  return system;
}

/* audio loop for running in a different thread */
extern int audio_loop(void *ptr){

  Uint64 SDL_time_Hz = SDL_GetPerformanceFrequency();

  audio_system* system = (audio_system*) ptr; /* typecast to system */
  int quit = 0;

  Uint64 last_t = SDL_GetPerformanceCounter(); /* timer for keeping loop speed in check */

  if (SDL_LockMutex(system->system_quit_mutex) == 0) { /* handle mutex for quit */
    quit = system->quit; /* set quit to audio system's quit */
  SDL_UnlockMutex(system->system_quit_mutex);
  }
  else { /* error message for when mutex breaks */
    printf("Unable to lock mutex: %s\n", SDL_GetError());
  }

  while (!quit){ /* main audio loop */

    step_audio(system); /* step audio system */

    /* add delay if loop is going too fast */
    if ( MIN_LOOP_DELAY > ((SDL_GetPerformanceCounter() - last_t)/(double)SDL_time_Hz)) {
      SDL_Delay(1000*MIN_LOOP_DELAY);
    }
    last_t = SDL_GetPerformanceCounter();

    if (SDL_LockMutex(system->system_quit_mutex) == 0) { /* handle mutex for quit */
      quit = system->quit; /* set quit to audio system's quit */
    SDL_UnlockMutex(system->system_quit_mutex);
    }
    else { /* error message for when mutex breaks */
      printf("Unable to lock mutex: %s\n", SDL_GetError());
    }
  }

  return 0;

}

/* audio frequency detection loop for running in a different thread */
extern int frequency_loop(void *ptr){

  Uint64 SDL_time_Hz = SDL_GetPerformanceFrequency();

  audio_system* system = (audio_system*) ptr; /* typecast to system */
  int quit = 0;
  int measure_frequency_value = 0;

  Uint64 last_t = SDL_GetPerformanceCounter(); /* timer for keeping loop speed in check */

  if (SDL_LockMutex(system->system_quit_mutex) == 0) { /* handle mutex for quit */
    quit = system->quit; /* set quit to audio system's quit */
  SDL_UnlockMutex(system->system_quit_mutex);
  }
  else { /* error message for when mutex breaks */
    printf("Unable to lock mutex: %s\n", SDL_GetError());
  }

  if (SDL_LockMutex(system->audio_system_mutex) == 0) { /* handle mutex for quit */
    measure_frequency_value = system->measure_frequency_value; /* set measure_frequency_value */
    SDL_UnlockMutex(system->audio_system_mutex);
  }
  else { /* error message for when mutex breaks */
    printf("Unable to lock mutex: %s\n", SDL_GetError());
  }

  while (!quit){ /* frequency detection loop */

    if(measure_frequency_value){
      get_input_frequency(system); /* do the input frequency calculation */
    }

    /* add delay if loop is going too fast */
    if ( MIN_LOOP_DELAY > ((SDL_GetPerformanceCounter() - last_t)/(double)SDL_time_Hz)) {
      SDL_Delay(1000*MIN_LOOP_DELAY);
    }
    last_t = SDL_GetPerformanceCounter();

    if (SDL_LockMutex(system->system_quit_mutex) == 0) { /* handle mutex for quit */
      quit = system->quit; /* set quit to audio system's quit */
    SDL_UnlockMutex(system->system_quit_mutex);
    }
    else { /* error message for when mutex breaks */
      printf("Unable to lock mutex: %s\n", SDL_GetError());
    }

    if (SDL_LockMutex(system->audio_system_mutex) == 0) { /* handle mutex for quit */
      measure_frequency_value = system->measure_frequency_value; /* set measure_frequency_value */
      SDL_UnlockMutex(system->audio_system_mutex);
    }
    else { /* error message for when mutex breaks */
      printf("Unable to lock mutex: %s\n", SDL_GetError());
    }

  }

  return 0;

}

/* set audio device */
int set_audio_driver(int index, audio_system* system){
  if (SDL_LockMutex(system->audio_system_mutex) == 0) { /* handle mutex for audio system */

    if (index < SDL_GetNumAudioDrivers()) {
      SDL_AudioQuit();
      if (SDL_AudioInit(SDL_GetAudioDriver(index))) { /* initialise audio driver and check for error */
        SDL_Log("Audio driver failed to initialize: %s", SDL_GetError());
      }
      SDL_Log("Audio driver is: %s", SDL_GetAudioDriver(index)); /* log for testing */
      system->audio_driver_index = index;
      SDL_UnlockMutex(system->audio_system_mutex);
      return 0;
    }

    SDL_UnlockMutex(system->audio_system_mutex);
  }
  else { /* error message for when mutex breaks */
    printf("Unable to lock mutex: %s\n", SDL_GetError());
  }

  return 1;
}

/* set audio driver */
int set_input_audio_device(int index, audio_system* system){
  if (SDL_LockMutex(system->audio_system_mutex) == 0) { /* handle mutex for audio system */

    if (index < SDL_GetNumAudioDevices(1)){
      if (system->input_device != 0) {
        SDL_CloseAudioDevice(system->input_device);/* close audio device */
      }
      if(system->input_stream != NULL){
        SDL_FreeAudioStream(system->input_stream);/* free audio stream */
      }
      system->input_device = SDL_OpenAudioDevice(SDL_GetAudioDeviceName(index, 1), 1, &(system->wanted_audiospec), &(system->input_audiospec), SDL_AUDIO_ALLOW_ANY_CHANGE);/* open device */
      if (system->input_device == 0) { /* check for error */
        SDL_Log("Failed to open input device: %s", SDL_GetError());
      }
      SDL_Log("Audio input is: %s", SDL_GetAudioDeviceName(index, 1));  /* log for testing */
      system->input_stream = SDL_NewAudioStream(system->input_audiospec.format, system->input_audiospec.channels, system->input_audiospec.freq, system->wanted_audiospec.format, system->wanted_audiospec.channels, system->wanted_audiospec.freq);/* create new audio stream converter */
      if (system->input_stream == NULL){ /* check for error */
        SDL_Log("Failed to create input stream: %s", SDL_GetError());
      }
      system->audio_input_index = index;
      if(system->input_on){ /* set the on/off state to the correct one */
        turn_on_audio_input(system);
      }
      else {
        turn_off_audio_input(system);
      }
      SDL_UnlockMutex(system->audio_system_mutex);
      return 0;
    }

    SDL_UnlockMutex(system->audio_system_mutex);
  }
  else { /* error message for when mutex breaks */
    printf("Unable to lock mutex: %s\n", SDL_GetError());
  }

  return 1;
}

/* set audio driver */
int set_output_audio_device(int index, audio_system* system){
  if (SDL_LockMutex(system->audio_system_mutex) == 0) { /* handle mutex for audio system */

    if (index < SDL_GetNumAudioDevices(0)){
      if (system->output_device != 0) {
        SDL_CloseAudioDevice(system->output_device);/* close audio device */
      }
      if(system->output_stream != NULL){
        SDL_FreeAudioStream(system->output_stream);/* free audio stream */
      }
      system->output_device = SDL_OpenAudioDevice(SDL_GetAudioDeviceName(index, 0), 0, &(system->wanted_audiospec), &(system->output_audiospec), SDL_AUDIO_ALLOW_ANY_CHANGE);/* open device */
      if (system->output_device == 0) { /* check for error */
        SDL_Log("Failed to open output device: %s", SDL_GetError());
      }
      SDL_Log("Audio output is: %s", SDL_GetAudioDeviceName(index, 0));  /* log for testing */
      system->output_stream = SDL_NewAudioStream(system->wanted_audiospec.format, system->wanted_audiospec.channels, system->wanted_audiospec.freq, system->output_audiospec.format, system->output_audiospec.channels, system->output_audiospec.freq);/* create new audio stream converter */
      if (system->output_stream == NULL){ /* check for error */
        SDL_Log("Failed to create output stream: %s", SDL_GetError());
      }
      system->audio_output_index = index;
      if(system->output_on){ /* set the on/off state to the correct one */
        turn_on_audio_output(system);
      }
      else {
        turn_off_audio_output(system);
      }
      SDL_UnlockMutex(system->audio_system_mutex);
      return 0;
    }

    SDL_UnlockMutex(system->audio_system_mutex);
  }
  else { /* error message for when mutex breaks */
    printf("Unable to lock mutex: %s\n", SDL_GetError());
  }

  return 1;
}

/* do one step of the audio system */
int step_audio(audio_system* system){

  int input_on = 0;
  int output_on = 0;

  if (SDL_LockMutex(system->audio_system_mutex) == 0) { /* handle mutex for audio system */

    input_on = system->input_on; /* on off state for audio io system */
    output_on = system->output_on;

    SDL_UnlockMutex(system->audio_system_mutex);
  }
  else { /* error message for when mutex breaks */
    printf("Unable to lock mutex: %s\n", SDL_GetError());
  }

  if (input_on){ /* handle input */

    float* left_samples = NULL; /* pointer for samples */
    float* right_samples = NULL;

    void* data = malloc(WANTED_AUDIO_BUFFER_SIZE*(sizeof(float)*2)); /* allocate memory for data to get */

    if (SDL_LockMutex(system->audio_system_mutex) == 0) { /* handle mutex for audio system */

      Uint32 length = SDL_DequeueAudio(system->input_device, data, WANTED_AUDIO_BUFFER_SIZE*(sizeof(float)*2));
      SDL_AudioStreamPut(system->input_stream, data, length);
      length = SDL_AudioStreamGet(system->input_stream, data, WANTED_AUDIO_BUFFER_SIZE*(sizeof(float)*2));
      system->last_input_len = length/(sizeof(float)*2); /* calculate and store the number of samples */
      left_samples = malloc(system->last_input_len * sizeof(float)); /* make buffer for deinterleaved samples */
      right_samples = malloc(system->last_input_len * sizeof(float));
      deinterleave(left_samples, right_samples, data, system->last_input_len); /* deinterleave the samples */

      SDL_UnlockMutex(system->audio_system_mutex);
    }
    else { /* error message for when mutex breaks */
      printf("Unable to lock mutex: %s\n", SDL_GetError());
    }


    float input_l[AUDIO_PROSESSING_LENGTH];/* copy of input samples for data processing */
    float input_r[AUDIO_PROSESSING_LENGTH];

    if (SDL_LockMutex(system->audio_input_mutex) == 0) { /* handle mutex for input */
      shift_append_audio(system->input_l, left_samples, AUDIO_PROSESSING_LENGTH, system->last_input_len); /* shift append data in to input processing buffer */
      shift_append_audio(system->input_r, right_samples, AUDIO_PROSESSING_LENGTH, system->last_input_len);
      memcpy(input_l, system->input_l, AUDIO_PROSESSING_LENGTH*sizeof(float)); /* copy wanted input */
      memcpy(input_r, system->input_r, AUDIO_PROSESSING_LENGTH*sizeof(float)); /* copy wanted input */
    SDL_UnlockMutex(system->audio_input_mutex);
    }
    else { /* error message for when mutex breaks */
      printf("Unable to lock mutex: %s\n", SDL_GetError());
    }

    float in_l_vol = get_peak(input_l, AUDIO_PROSESSING_LENGTH, PEAK_VALUE_LENGTH); /* calculate input volume values */
    float in_r_vol = get_peak(input_r, AUDIO_PROSESSING_LENGTH, PEAK_VALUE_LENGTH);

    shift_append_audio(system->input_peaks_l, &(in_l_vol), MAX_VALUE_BUFFER_LENGTH, 1); /* shift append volume values in to input max volume processing buffer */
    shift_append_audio(system->input_peaks_r, &(in_r_vol), MAX_VALUE_BUFFER_LENGTH, 1);
    float in_l_vol_max = get_peak(system->input_peaks_l, MAX_VALUE_BUFFER_LENGTH, MAX_VALUE_BUFFER_LENGTH); /* calculate input max volume values */
    float in_r_vol_max = get_peak(system->input_peaks_r, MAX_VALUE_BUFFER_LENGTH, MAX_VALUE_BUFFER_LENGTH);

    if (SDL_LockMutex(system->audio_volume_mutex) == 0) { /* handle mutex for volume */
      system->in_l_vol = in_l_vol; /* set values */
      system->in_r_vol = in_r_vol;
      system->in_l_vol_max = in_l_vol_max;
      system->in_r_vol_max = in_r_vol_max;
    SDL_UnlockMutex(system->audio_volume_mutex);
    }
    else { /* error message for when mutex breaks */
      printf("Unable to lock mutex: %s\n", SDL_GetError());
    }

    if (left_samples != NULL){ /* free allocated memories */
      free(left_samples);
    }
    if (right_samples != NULL){
      free(right_samples);
    }
    if (data != NULL){
      free(data);
    }
  }


  if (output_on){ /* handle output */
    audio_output_step(system); /* do one step of output */

    float out_l_vol; /* volume values to fill */
    float out_r_vol;

    if (SDL_LockMutex(system->audio_output_mutex) == 0) { /* handle mutex for output */
      out_l_vol = get_peak(system->output_l, PEAK_VALUE_LENGTH, PEAK_VALUE_LENGTH); /* set output volume values */
      out_r_vol = get_peak(system->output_r, PEAK_VALUE_LENGTH, PEAK_VALUE_LENGTH);
      SDL_UnlockMutex(system->audio_output_mutex);
    }
    else { /* error message for when mutex breaks */
      printf("Unable to lock mutex: %s\n", SDL_GetError());
    }

    shift_append_audio(system->output_peaks_l, &(out_l_vol), MAX_VALUE_BUFFER_LENGTH, 1); /* shift append volume values in to input max volume processing buffer */
    shift_append_audio(system->output_peaks_r, &(out_r_vol), MAX_VALUE_BUFFER_LENGTH, 1);
    float out_l_vol_max = get_peak(system->output_peaks_l, MAX_VALUE_BUFFER_LENGTH, MAX_VALUE_BUFFER_LENGTH); /* calculate input max volume values */
    float out_r_vol_max = get_peak(system->output_peaks_r, MAX_VALUE_BUFFER_LENGTH, MAX_VALUE_BUFFER_LENGTH);

    if (SDL_LockMutex(system->audio_volume_mutex) == 0) { /* handle mutex for volume */
      system->out_l_vol = out_l_vol; /* set values */
      system->out_r_vol = out_r_vol;
      system->out_l_vol_max = out_l_vol_max;
      system->out_r_vol_max = out_r_vol_max;
      SDL_UnlockMutex(system->audio_volume_mutex);
    }
    else { /* error message for when mutex breaks */
      printf("Unable to lock mutex: %s\n", SDL_GetError());
    }

    void* data = malloc(WANTED_AUDIO_BUFFER_SIZE*(sizeof(float)*2)); /* allocate appropriate data buffer for between stream to output */


    if (SDL_LockMutex(system->audio_system_mutex) == 0) { /* handle mutex for audio system */

      void* data = malloc(WANTED_AUDIO_BUFFER_SIZE*(sizeof(float)*2)); /* allocate appropriate data buffer for between stream to output */
      Uint32 length = SDL_AudioStreamGet(system->output_stream, data, WANTED_AUDIO_BUFFER_SIZE*(sizeof(float)*2));
      SDL_QueueAudio(system->output_device, data, length);

      SDL_UnlockMutex(system->audio_system_mutex);
    }
    else { /* error message for when mutex breaks */
      printf("Unable to lock mutex: %s\n", SDL_GetError());
    }

    if (data != NULL){ /* free allocated memory */
      free(data);
    }
  }
  return 0;
}


/* get the input's frequency and update the systems's internal representation for it */
int get_input_frequency(audio_system* system){

  float input[AUDIO_PROSESSING_LENGTH];

  if (SDL_LockMutex(system->audio_input_mutex) == 0) { /* handle mutex for quit */
    memcpy(input, system->input_channel ? system->input_r : system->input_l, AUDIO_PROSESSING_LENGTH*sizeof(float)); /* copy wanted input */
  SDL_UnlockMutex(system->audio_input_mutex);
  }
  else { /* error message for when mutex breaks */
    printf("Unable to lock mutex: %s\n", SDL_GetError());
  }

  frequency_list frequencies = calculate_frequency(input, AUDIO_PROSESSING_LENGTH, SAMPLE_RATE); /* get frequency for the input */

  if (SDL_LockMutex(system->audio_frequency_mutex) == 0) { /* handle mutex for frequency */

    system->input_frequency_detected = frequencies.num; /* get number of frequencies detected */
    if (system->input_frequency_detected) {
      system->input_frequency = frequencies.frequency[0]; /* get frequency for appropriate channel */
      system->input_midi_note = clamp_midi_english(f_to_midi_rounded(system->input_frequency)); /* get clamped midi note */
      system->input_midi_note_difference = (double)system->input_midi_note - f_to_midi(system->input_frequency); /* calculate frequency difference in midi notes */
    }
    else {
      system->input_frequency = 0; /* set to zero */
      system->input_midi_note = 0;
      system->input_midi_note_difference = 0;
    }

    SDL_UnlockMutex(system->audio_frequency_mutex);
  }
  else { /* error message for when mutex breaks */
    printf("Unable to lock mutex: %s\n", SDL_GetError());
  }


  clean_frequency_list(frequencies); /* clean up after processing is finished */
  return 0;
}


/* deinterleve the stereo audio in to 2 mono arrays */
int deinterleave(float* left, float* right, float* data, Uint32 length){
  Uint32 i;
  for(i = 0; i < length; i++){ /* deinterleave one element at a time */
    left[i] = data[i*2];
    right[i] = data[i*2+1];
  }
  return 0;
}

/* interleve the given length of mono audio array in to stereo */
int interleave(float* left, float* right, float* data, Uint32 length){
  Uint32 i;
  for(i = 0; i < length; i++){ /* interleave one element at a time */
    data[i*2] = left[i];
    data[i*2+1] = right[i];
  }
  return 0;
}

/* get peak value for the last <peak_length> number of value of the data
peak_length must be less than or equal to data_length */
float get_peak(float* data, Uint32 data_length, Uint32 peak_length){
  Uint32 i;
  float max = 0;
  for(i = data_length - peak_length; i < data_length; i++){ /* loop through wanted section of the data */
    float current = fabs(data[i]); /* current data to test */
    if (current > max){
      max = current; /* replace max if larger is found */
    }
  }
  return clamp_value(max); /* return the peak value clamped to within 0 to 1 */
}

/* load audio output for left and right channel
the length must be less than PEAK_VALUE_LENGTH to work */
int load_output_section(float* left, float* right, Uint32 length, audio_system* system){
  Uint32 data_size = length*sizeof(float)*2; /* get length of total data in bytes */
  void* data = malloc(data_size); /* allocate memory for data to be sent in to stream  */

  float* left_copy = malloc(length*sizeof(float)); /* allocate memory for copy of left and right channel data input */
  float* right_copy = malloc(length*sizeof(float));

  memcpy(left_copy, left, length*sizeof(float)); /* copy data */
  memcpy(right_copy, right, length*sizeof(float));

  float output_vol = 0;

  if (SDL_LockMutex(system->audio_system_mutex) == 0) { /* handle mutex for system */
    output_vol = system->output_vol;
    SDL_UnlockMutex(system->audio_system_mutex);
  }
  else { /* error message for when mutex breaks */
    printf("Unable to lock mutex: %s\n", SDL_GetError());
  }

  adjust_volume(left_copy, output_vol, length); /* adjust the output volume for left and right */
  adjust_volume(right_copy, output_vol, length); /* adjust the output volume for left and right */
  interleave(left_copy, right_copy, data, length); /* interleve the audio data */

  if (SDL_LockMutex(system->audio_system_mutex) == 0) { /* handle mutex for system */
    SDL_AudioStreamPut(system->output_stream, data, data_size); /* put audio in to the system */
    SDL_UnlockMutex(system->audio_system_mutex);
  }
  else { /* error message for when mutex breaks */
    printf("Unable to lock mutex: %s\n", SDL_GetError());
  }

  if (SDL_LockMutex(system->audio_output_mutex) == 0) { /* handle mutex for output */
    shift_append_audio(system->output_l, left_copy, PEAK_VALUE_LENGTH, length);/* shift append data in to output peak calculator buffer */
    shift_append_audio(system->output_r, right_copy, PEAK_VALUE_LENGTH, length);
    SDL_UnlockMutex(system->audio_output_mutex);
  }
  else { /* error message for when mutex breaks */
    printf("Unable to lock mutex: %s\n", SDL_GetError());
  }

  if (data != NULL){ /* free allocated memory */
    free(data);
  }
  if (left_copy != NULL){
    free(left_copy);
  }
  if (right_copy != NULL){
    free(right_copy);
  }
  return 0;
}

/* safe version of load output that supports any length */
int load_output(float* left, float* right, Uint32 length, audio_system* system){
  if (length <= PEAK_VALUE_LENGTH){ /* if it can be loaded with single load output section */
    load_output_section(left, right, length, system); /* call it as is */
  }
  else { /* if it can't*/
  Uint32 i; /* split it up in to sections */
    for(i = 0; i < length/(Uint32)PEAK_VALUE_LENGTH; i++){
      load_output_section(left + i*(Uint32)PEAK_VALUE_LENGTH, right + i*(Uint32)PEAK_VALUE_LENGTH, PEAK_VALUE_LENGTH, system);
    }
    load_output_section(left + (length/(Uint32)PEAK_VALUE_LENGTH)*(Uint32)PEAK_VALUE_LENGTH, right + (length/(Uint32)PEAK_VALUE_LENGTH)*(Uint32)PEAK_VALUE_LENGTH, length - (length/(Uint32)PEAK_VALUE_LENGTH)*(Uint32)PEAK_VALUE_LENGTH, system); /* handle last little bit of data */
  }
  return 0;
}

/* adjust the volume of first length samples of the data */
int adjust_volume(float* data, float volume, Uint32 length){
  Uint32 i;
  for(i = 0; i < length; i++){ /* for each element */
    data[i] = volume*data[i]; /* edit volume */
  }
  return 0;
}

/* shift the data and append the input to it */
int shift_append_audio(float* data, float* input, Uint32 data_length, Uint32 input_length){
  Uint32 i;
  for(i = 0; i < data_length - input_length; i++){ /* shift elements */
    data[i] = data[i + input_length];
  }
  for(i = data_length - input_length; i < data_length; i++){ /* append to data */
    data[i] = input[i - data_length + input_length];
  }
  return 0;
}


/* do one step of audio playback */
int audio_output_step(audio_system* system){

  Uint32 queued_audio_size = 0;
  Uint32 play_tone_end = 0;
  int input_monitor = 0;
    Uint64 audio_playback_samples = 0;
  float play_tone_frequency = 0;
  int last_input_len = 0;

  if (SDL_LockMutex(system->audio_system_mutex) == 0) { /* handle mutex for system */
    queued_audio_size = SDL_GetQueuedAudioSize(system->output_device);
    play_tone_end = system->play_tone_end;
    input_monitor = system->input_monitor;
    audio_playback_samples = system->audio_playback_samples;
    play_tone_frequency = system->play_tone_frequency;
    last_input_len = system->last_input_len;
    SDL_UnlockMutex(system->audio_system_mutex);
  }
  else { /* error message for when mutex breaks */
    printf("Unable to lock mutex: %s\n", SDL_GetError());
  }

  if (queued_audio_size/(sizeof(float)*2) < PLAYBACK_BUFFER_LENGTH){/* if more data is needed */

    if (SDL_GetTicks() < play_tone_end ){ /* if supposed to be playing only a tone */
      float data[PLAYBACK_BUFFER_LENGTH];
      int i;
      for( i = 0; i < PLAYBACK_BUFFER_LENGTH; i++){
        data[i] = PLAY_NOTE_VOLUME*sin( (double)(audio_playback_samples + i) * play_tone_frequency * 3.14159 * 2 / (double)SAMPLE_RATE);
      }
      load_output(data, data, PLAYBACK_BUFFER_LENGTH, system); /* load output to be played */

      if (SDL_LockMutex(system->audio_system_mutex) == 0) { /* handle mutex for system */
        system->audio_playback_samples += PLAYBACK_BUFFER_LENGTH; /* shift time as required */
        SDL_UnlockMutex(system->audio_system_mutex);
      }
      else { /* error message for when mutex breaks */
        printf("Unable to lock mutex: %s\n", SDL_GetError());
      }

    }

    else if (input_monitor) {

      float input_l[AUDIO_PROSESSING_LENGTH];/* copy of input samples for data processing */
      float input_r[AUDIO_PROSESSING_LENGTH];

      if (SDL_LockMutex(system->audio_input_mutex) == 0) { /* handle mutex for quit */
        memcpy(input_l, system->input_l, AUDIO_PROSESSING_LENGTH*sizeof(float)); /* copy wanted input */
        memcpy(input_r, system->input_r, AUDIO_PROSESSING_LENGTH*sizeof(float)); /* copy wanted input */
      SDL_UnlockMutex(system->audio_input_mutex);
      }
      else { /* error message for when mutex breaks */
        printf("Unable to lock mutex: %s\n", SDL_GetError());
      }

      load_output(input_l + AUDIO_PROSESSING_LENGTH - last_input_len, input_r + AUDIO_PROSESSING_LENGTH - last_input_len, last_input_len, system); /* load last gotten input */

      if (SDL_LockMutex(system->audio_system_mutex) == 0) { /* handle mutex for system */
        system->audio_playback_samples += last_input_len; /* shift time as required */
        SDL_UnlockMutex(system->audio_system_mutex);
      }
      else { /* error message for when mutex breaks */
        printf("Unable to lock mutex: %s\n", SDL_GetError());
      }

    }

    else { /* if nothing should be playing */
      float zeros[PLAYBACK_BUFFER_LENGTH]; /* generate zeros and clear output by playing that back */
      int i;
      for( i = 0; i < PLAYBACK_BUFFER_LENGTH; i++){
        zeros[i] = 0;
      }
      load_output(zeros, zeros, PLAYBACK_BUFFER_LENGTH, system); /* load the zeros to clear output */

      if (SDL_LockMutex(system->audio_system_mutex) == 0) { /* handle mutex for system */
        system->audio_playback_samples += PLAYBACK_BUFFER_LENGTH; /* shift time as required */
        SDL_UnlockMutex(system->audio_system_mutex);
      }
      else { /* error message for when mutex breaks */
        printf("Unable to lock mutex: %s\n", SDL_GetError());
      }

    }

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
  clear_audio_input(system); /* clear internal data for device ans stream */
  system->input_on = 1;
  return 0;
}

int turn_off_audio_input(audio_system* system){
  SDL_PauseAudioDevice(system->input_device, 1);
  clear_audio_input(system); /* clear internal data for device ans stream */
  system->input_on = 0;
  return 0;
}

int turn_on_audio_output(audio_system* system){
  SDL_PauseAudioDevice(system->output_device, 0);
  clear_audio_output(system); /* clear internal data for device ans stream */
  system->output_on = 1;
  return 0;
}

int turn_off_audio_output(audio_system* system){
  SDL_PauseAudioDevice(system->output_device, 1);
  clear_audio_output(system); /* clear internal data for device ans stream */
  system->output_on = 0;
  return 0;
}

/* clear audio input*/
int clear_audio_input(audio_system* system){
  SDL_ClearQueuedAudio(system->input_device);/* clear internal data for device ans stream */
  SDL_AudioStreamClear(system->input_stream);
  return 0;
}

/* clear audio output */
int clear_audio_output(audio_system* system){
  SDL_ClearQueuedAudio(system->output_device);/* clear internal data for device ans stream */
  SDL_AudioStreamClear(system->output_stream);
  system->audio_playback_samples = 0;
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

    if (system->audio_system_mutex != NULL) { /* free mutexes */
      if (SDL_LockMutex(system->audio_system_mutex) == 0) { /* make sure mutex is cleared */
        SDL_UnlockMutex(system->audio_system_mutex);
      }
      else { /* error message for when mutex breaks */
        printf("Unable to lock mutex: %s\n", SDL_GetError());
      }
      SDL_DestroyMutex(system->audio_system_mutex);
    }
    if (system->audio_input_mutex != NULL) { /* free mutexes */
      if (SDL_LockMutex(system->audio_input_mutex) == 0) { /* make sure mutex is cleared */
        SDL_UnlockMutex(system->audio_input_mutex);
      }
      else { /* error message for when mutex breaks */
        printf("Unable to lock mutex: %s\n", SDL_GetError());
      }
      SDL_DestroyMutex(system->audio_input_mutex);
    }
    if (system->audio_output_mutex != NULL) { /* free mutexes */
      if (SDL_LockMutex(system->audio_output_mutex) == 0) { /* make sure mutex is cleared */
        SDL_UnlockMutex(system->audio_output_mutex);
      }
      else { /* error message for when mutex breaks */
        printf("Unable to lock mutex: %s\n", SDL_GetError());
      }
      SDL_DestroyMutex(system->audio_output_mutex);
    }
    if (system->audio_frequency_mutex != NULL) { /* free mutexes */
      if (SDL_LockMutex(system->audio_frequency_mutex) == 0) { /* make sure mutex is cleared */
        SDL_UnlockMutex(system->audio_frequency_mutex);
      }
      else { /* error message for when mutex breaks */
        printf("Unable to lock mutex: %s\n", SDL_GetError());
      }
      SDL_DestroyMutex(system->audio_frequency_mutex);
    }
    if (system->audio_volume_mutex != NULL) { /* free mutexes */
      if (SDL_LockMutex(system->audio_volume_mutex) == 0) { /* make sure mutex is cleared */
        SDL_UnlockMutex(system->audio_volume_mutex);
      }
      else { /* error message for when mutex breaks */
        printf("Unable to lock mutex: %s\n", SDL_GetError());
      }
      SDL_DestroyMutex(system->audio_volume_mutex);
    }
    if (system->system_quit_mutex != NULL) { /* free mutexes */
      if (SDL_LockMutex(system->system_quit_mutex) == 0) { /* make sure mutex is cleared */
        SDL_UnlockMutex(system->system_quit_mutex);
      }
      else { /* error message for when mutex breaks */
        printf("Unable to lock mutex: %s\n", SDL_GetError());
      }
      SDL_DestroyMutex(system->system_quit_mutex);
    }


    SDL_AudioQuit();
    free(system);
  }
  return 0;
}
