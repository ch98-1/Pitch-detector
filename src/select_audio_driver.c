#include "select_audio_driver.h"


int select_audio_driver_events(int* program_state, int* updatescreen, SDL_Renderer* renderer, SDL_Event* e, audio_system* system){

  handle_volume_bar_event(renderer, e, system); /* take care of volume bar stuff */

  int display_w, display_h;
  SDL_GetRendererOutputSize(renderer, &display_w, &display_h);

  if (e->type == SDL_MOUSEBUTTONDOWN ){ /* if it is a mouse button click */
    int i;
    for (i = 0; i < SDL_GetNumAudioDrivers(); i++){
      int x = display_w / 2 - DRIVER_BUTTON_WIDTH / 2; /* x and y for testing the mouse click */
      int y = DRIVER_CONTENT_HEIGHT + i * (DRIVER_BUTTON_HEIGHT + DRIVER_BUTTON_HEIGHT_SPACING);
      if(e->button.y > y && e->button.y < y + DRIVER_BUTTON_HEIGHT && e->button.x > x && e->button.x < x + DRIVER_BUTTON_WIDTH){ /* if click is within the button */
        set_audio_driver(i, system); /* set driver to appropriate value */
        set_input_audio_device(0, system); /* also set the audio device input and output to first one */
        set_output_audio_device(0, system);
      }
    }
  }

  *updatescreen = 1;
  return  0;
}

int select_audio_driver_process(int* program_state, int* updatescreen, audio_system* system){
    system->play_tone_end = 0; /* not playing any tone */

  return  0;
}

int select_audio_driver_display(int* program_state, SDL_Renderer* renderer, audio_system* system){
  int display_w, display_h;
  SDL_GetRendererOutputSize(renderer, &display_w, &display_h);

  render_volume_bar(renderer, system, VOLUME_BAR_SPACING_Y_LARGE + UI_TOP_HEIGHT);

  char driver_name_string[DRIVER_TEXT_LENGTH + 2]; /* string for driver name to be turnicated */
  memcpy(driver_name_string, "\n", DRIVER_TEXT_LENGTH); /* end of the string */

  Uint8 or, og, ob, oa; /* old render draw colors */
  SDL_GetRenderDrawColor(renderer, &or, &og, &ob, &oa); /* fill old color */
  SDL_Rect dstrect;/* make rectangle */

  dstrect.w = DRIVER_BUTTON_WIDTH; /* set rectangle height */
  dstrect.h = DRIVER_BUTTON_HEIGHT;

  int i;
  for (i = 0; i < SDL_GetNumAudioDrivers(); i++){
    dstrect.x = display_w / 2 - DRIVER_BUTTON_WIDTH / 2; /* set rectangle position */
    dstrect.y = DRIVER_CONTENT_HEIGHT + i * (DRIVER_BUTTON_HEIGHT + DRIVER_BUTTON_HEIGHT_SPACING);
    if (system->audio_driver_index == i) {
      SDL_SetRenderDrawColor(renderer, C_UIL_Gray.r, C_UIL_Gray.g, C_UIL_Gray.b, C_UIL_Gray.a); /* set new color */
      SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */
      SDL_SetRenderDrawColor(renderer, C_UID_Gray.r, C_UID_Gray.g, C_UID_Gray.b, C_UID_Gray.a); /* set new color */
      SDL_RenderDrawRect(renderer, &dstrect); /* draw background rectangle */
      render_text_absolute_c(renderer, font_18, strncpy(driver_name_string, SDL_GetAudioDriver(i), DRIVER_TEXT_LENGTH), C_Text_Gray, dstrect.x + DRIVER_BUTTON_WIDTH / 2, dstrect.y + DRIVER_BUTTON_HEIGHT / 2); /* draw text */
    }
    else {
      SDL_SetRenderDrawColor(renderer, C_UI_Gray.r, C_UI_Gray.g, C_UI_Gray.b, C_UI_Gray.a); /* set new color */
      SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */
      SDL_SetRenderDrawColor(renderer, C_UID_Gray.r, C_UID_Gray.g, C_UID_Gray.b, C_UID_Gray.a); /* set new color */
      SDL_RenderDrawRect(renderer, &dstrect); /* draw background rectangle */
      render_text_absolute_c(renderer, font_18, strncpy(driver_name_string, SDL_GetAudioDriver(i), DRIVER_TEXT_LENGTH), C_Text_Gray, dstrect.x + DRIVER_BUTTON_WIDTH / 2, dstrect.y + DRIVER_BUTTON_HEIGHT / 2); /* draw text */
    }
  }

  SDL_SetRenderDrawColor(renderer, or, og, ob, oa); /* restore old color */

  return 0;
}
