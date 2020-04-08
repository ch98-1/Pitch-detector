#include "select_audio_driver.h"


int select_audio_device_events(int* program_state, int* updatescreen, SDL_Renderer* renderer, SDL_Event* e, audio_system* system){

    handle_volume_bar_event(renderer, e, system); /* take care of volume bar stuff */






  *updatescreen = 1;
  return  0;
}

int select_audio_device_process(int* program_state, int* updatescreen, audio_system* system){
  /*int i;

  for (i = 0; i < SDL_GetNumAudioDrivers(); ++i) {
      printf("Audio driver %d: %s\n", i, SDL_GetAudioDriver(i));
  }*/

  return  0;
}

int select_audio_device_display(int* program_state, SDL_Renderer* renderer, audio_system* system){
  int display_w, display_h;
  SDL_GetRendererOutputSize(renderer, &display_w, &display_h);

  render_volume_bar(renderer, system, VOLUME_BAR_SPACING_Y_LARGE + UI_TOP_HEIGHT);


  return 0;
}
