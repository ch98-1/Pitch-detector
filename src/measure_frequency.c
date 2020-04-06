#include "measure_frequency.h"


int measure_frequency_events(int* program_state, int* updatescreen, SDL_Event* e){
  *updatescreen = 1;
  return  0;
}

int measure_frequency_process(int* program_state, int* updatescreen){

  return  0;
}

int measure_frequency_display(int* program_state, SDL_Renderer* renderer){
  int display_w, display_h;
  SDL_GetRendererOutputSize(renderer, &display_w, &display_h);
  return 0;
}
