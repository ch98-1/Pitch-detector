#include "measure_frequency.h"


int measure_frequency_events(int* program_state, int* updatescreen, SDL_Renderer* renderer, SDL_Event* e, audio_system* system){

  handle_volume_bar_event(renderer, e, system); /* take care of volume bar stuff */



  *updatescreen = 1;
  return  0;
}

int measure_frequency_process(int* program_state, int* updatescreen, audio_system* system){
  system->play_tone_end = 0; /* not playing any tone */
  
  return  0;
}

int measure_frequency_display(int* program_state, SDL_Renderer* renderer, audio_system* system){
  int display_w, display_h;
  SDL_GetRendererOutputSize(renderer, &display_w, &display_h);

  render_volume_bar(renderer, system, VOLUME_BAR_SPACING_Y_LARGE + UI_TOP_HEIGHT);


  render_text_absolute_c(renderer, font_60, "20000.00Hz", C_White, FREQUENCY_X*display_w, FREQUENCY_HEIGHT); /* draw current frequency */
  render_text_absolute_c(renderer, font_60, "MIDI# 128", C_White, MIDI_NUM_X*display_w, FREQUENCY_HEIGHT_2); /* draw current note in midi number*/
  render_text_absolute_c(renderer, font_60, "C♯3/D♭3", C_White, NOTE_X*display_w, FREQUENCY_HEIGHT_2); /* draw current note */
  return 0;
}
