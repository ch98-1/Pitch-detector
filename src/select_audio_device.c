#include "select_audio_driver.h"


int select_audio_device_events(int* program_state, int* updatescreen, SDL_Renderer* renderer, SDL_Event* e, audio_system* system){


  if (e->type == SDL_MOUSEBUTTONDOWN /* if clicing on right column for audio io on-off switch */
      && e->button.x >= VOLUME_BAR_SPACING_X
      && e->button.x <= VOLUME_BAR_SPACING_X + VOLUME_BAR_HEIGHT * 2 + VOLUME_BAR_SPACING_Y){
        printf("a");
    if (e->button.y >= VOLUME_BAR_SPACING_Y_LARGE + UI_TOP_HEIGHT /* input on-off */
        && e->button.y <= VOLUME_BAR_SPACING_Y_LARGE + UI_TOP_HEIGHT + VOLUME_BAR_HEIGHT * 2 + VOLUME_BAR_SPACING_Y){
      if (system->input_on){
        turn_off_audio_input(system);
      }
      else {
        turn_on_audio_input(system);
      }
    }
    else if(e->button.y >= VOLUME_BAR_SPACING_Y_LARGE + UI_TOP_HEIGHT + VOLUME_BAR_HEIGHT * 2 + VOLUME_BAR_SPACING_Y + VOLUME_BAR_SPACING_Y_LARGE/* output on-off */
            && e->button.y <= VOLUME_BAR_SPACING_Y_LARGE + UI_TOP_HEIGHT + VOLUME_BAR_HEIGHT * 2 + VOLUME_BAR_SPACING_Y + VOLUME_BAR_SPACING_Y_LARGE + VOLUME_BAR_HEIGHT * 2 + VOLUME_BAR_SPACING_Y){
      if (system->output_on){
        turn_off_audio_output(system);
      }
      else {
        turn_on_audio_output(system);
      }
    }
  }






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
