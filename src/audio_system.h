#ifndef AUDIO_SYSTEMS_H
#define AUDIO_SYSTEMS_H

#include <stdlib.h>

#include "kiss_fft.h"
#include "SDL.h"

#include "constants.h"

#include "convert_frequency.h"
#include "calculate_frequency.h"


/* audio is stored such that earliest time/oldest audio sample is at 0 and latest time/youngest audio sample is at the biggest address */


/* struct for storing audio system data */
typedef struct {
  int quit; /* quiting or not */

  int input_on; /* on off state for audio io system */
  int output_on;

  float output_vol; /* output volume */

  char *data_path;

  int audio_driver_index;
  int audio_input_index;
  int audio_output_index;

  SDL_AudioStream *input_stream; /* audio stream for converting formats */
  SDL_AudioStream *output_stream;

  SDL_AudioDeviceID input_device; /* input and output audio device */
  SDL_AudioDeviceID output_device;

  SDL_AudioSpec wanted_audiospec;/* audio stream format used for processing */
  SDL_AudioSpec input_audiospec;/* audio spec for input and output */
  SDL_AudioSpec output_audiospec;
  SDL_AudioSpec wav_audiospec;/* audio spec for wave file input */

  float input_l[AUDIO_PROSESSING_LENGTH];/* input samples for data processing */
  float input_r[AUDIO_PROSESSING_LENGTH];
  float input_peaks_l[MAX_VALUE_BUFFER_LENGTH];/* store peak values for max volume finding */
  float input_peaks_r[MAX_VALUE_BUFFER_LENGTH];
  int last_input_len; /* number of samples that was put in in last audio step */

  float output_l[PEAK_VALUE_LENGTH];/* output samples for peak value finding */
  float output_r[PEAK_VALUE_LENGTH];
  float output_peaks_l[MAX_VALUE_BUFFER_LENGTH];/* store peak values for max volume finding */
  float output_peaks_r[MAX_VALUE_BUFFER_LENGTH];

  float in_l_vol; /* volumes to display on volume bar */
  float in_r_vol;
  float in_l_vol_max;
  float in_r_vol_max;
  float out_l_vol;
  float out_r_vol;
  float out_l_vol_max;
  float out_r_vol_max;


  Uint64 audio_playback_samples; /* numer of samples that have been played back */

  Uint32 play_tone_end; /* time in SDL Tick to end playing the sine wave tone */
  float play_tone_frequency; /* frequency to play the tone at */

  int input_monitor; /* monitor the input by directly outputting it */


  int playback_content; /* what to play back. 0 for nothing, 1 for loop back, 2 for rounded to whole note */

  int input_channel; /* input channel to measure the frequency from. 0 for left, 1 for right */

  long int input_frequency_detected; /* number of frequencies detected */
  float input_frequency; /* frequency from processing the latest input audio */
  long int input_midi_note; /* rounded and clamped midi note for that frequency */
  float input_midi_note_difference; /* total difference from the clamped midi note. how much higher or lower the real frequency is in midi note units */


  int measure_frequency_value; /* start frequency measurement */


  SDL_mutex* audio_system_mutex; /* mutexes for various operations */
  SDL_mutex* audio_input_mutex;
  SDL_mutex* audio_output_mutex;
  SDL_mutex* audio_frequency_mutex;
  SDL_mutex* audio_volume_mutex;
  SDL_mutex* system_quit_mutex;

} audio_system;

/* initialise audio system */
audio_system* init_audio();


/* audio loop for running in a different thread */
extern int audio_loop(void *ptr);

/* audio frequency detection loop for running in a different thread */
extern int frequency_loop(void *ptr);


/* functions to set audio device and drivers */
int set_audio_driver(int index, audio_system* system);
int set_input_audio_device(int index, audio_system* system);
int set_output_audio_device(int index, audio_system* system);

/* do one step of the audio system */
int step_audio(audio_system* system);


/* get the input's frequency and update the systems's internal representation for it */
int get_input_frequency(audio_system* system);


/* deinterleve the stereo audio in to 2 mono arrays */
int deinterleave(float* left, float* right, float* data, Uint32 length);

/* interleve the given length of mono audio array in to stereo */
int interleave(float* left, float* right, float* data, Uint32 length);

/* get peak value for the last <peak_length> number of value of the data */
float get_peak(float* data, Uint32 data_length, Uint32 peak_length);

/* load audio output for left and right channel with length less than PEAK_VALUE_LENGTH */
int load_output_section(float* left, float* right, Uint32 length, audio_system* system);

/* safe version of load output that supports any length */
int load_output(float* left, float* right, Uint32 length, audio_system* system);

/* adjust the volume of first length samples of the data */
int adjust_volume(float* data, float volume, Uint32 length);

/* shift the data and append the input to it */
int shift_append_audio(float* data, float* input, Uint32 data_length, Uint32 input_length);


/* do one step of audio playback */
int audio_output_step(audio_system* system);

/* clamp value to 0 to 1 */
float clamp_value(float in);

/* turn on and off audio input and output */
int turn_on_audio_input(audio_system* system);
int turn_off_audio_input(audio_system* system);

int turn_on_audio_output(audio_system* system);
int turn_off_audio_output(audio_system* system);

/* clear audio input and output */
int clear_audio_input(audio_system* system);
int clear_audio_output(audio_system* system);


/* clean audio system */
int clean_audio(audio_system* system);

#endif /* AUDIO_SYSTEMS_H */
