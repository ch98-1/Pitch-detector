#include "select_audio_driver.h"


int select_audio_driver_events(int* program_state, int* updatescreen, SDL_Event* e){
  *updatescreen = 1;
  return  0;
}

int select_audio_driver_process(int* program_state, int* updatescreen){
  int i;

  for (i = 0; i < SDL_GetNumAudioDrivers(); ++i) {
      printf("Audio driver %d: %s\n", i, SDL_GetAudioDriver(i));
  }

  return  0;
}

int select_audio_driver_display(int* program_state, SDL_Renderer* renderer){
  int display_w, display_h;
  SDL_GetRendererOutputSize(renderer, &display_w, &display_h);
  return 0;
}
