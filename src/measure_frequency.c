#include "measure_frequency.h"


int measure_frequency_events(int* program_state, int* updatescreen, SDL_Renderer* renderer, SDL_Event* e, audio_system* system){

  static int volume_drag;/* dragging volume bar */
  if (e->type == SDL_MOUSEBUTTONDOWN /* if clicing on right column for audio io on-off switch */
      && e->button.x >= VOLUME_BAR_SPACING_X
      && e->button.x <= VOLUME_BAR_SPACING_X + VOLUME_BAR_HEIGHT * 2 + VOLUME_BAR_SPACING_Y){
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
  if (e->type == SDL_MOUSEBUTTONDOWN /* if clicking on volume bar */
      && e->button.y >=
      && e->button.y <= ){
  }
  if (e->type == SDL_MOUSEBUTTONUP){ /* if unclicking */
    volume_drag = 0;
  }
  if (!(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LMASK || SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_MMASK || SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_MRASK)) {
    volume_drag = 0;
    SDL_Log("No mouse button pressed \n");
  }
  if (e->type == SDL_MOUSEMOTION && volume_drag){ /* if dragging the volume bar */
    if (e->button.y >= /* if within the volume bar */
        && e->button.y <= ) {

    }
    else {
      volume_drag = 0;
    }
  }



  *updatescreen = 1;
  return  0;
}

int measure_frequency_process(int* program_state, int* updatescreen, audio_system* system){

  return  0;
}

int measure_frequency_display(int* program_state, SDL_Renderer* renderer, audio_system* system){
  int display_w, display_h;
  SDL_GetRendererOutputSize(renderer, &display_w, &display_h);

  render_volume_bar(renderer, system, VOLUME_BAR_SPACING_Y_LARGE + UI_TOP_HEIGHT);


  render_text_absolute_c(renderer, font_60, "20000.00Hz", C_White, 0.5*display_w, 140); /* draw current frequency */
  render_text_absolute_c(renderer, font_60, "MIDI# 128", C_White, 0.3*display_w, 210); /* draw current note in midi number*/
  render_text_absolute_c(renderer, font_60, "C♯3/D♭3", C_White, 0.8*display_w, 210); /* draw current note */
  return 0;
}
