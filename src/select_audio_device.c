#include "select_audio_driver.h"


int select_audio_device_events(int* program_state, int* updatescreen, SDL_Renderer* renderer, SDL_Event* e, audio_system* system){

  handle_volume_bar_event(renderer, e, system); /* take care of volume bar stuff */

  int display_w, display_h;
  SDL_GetRendererOutputSize(renderer, &display_w, &display_h);

  if (e->type == SDL_MOUSEBUTTONDOWN ){ /* if it is a mouse button click */
    int i;
    for (i = 0; i < SDL_GetNumAudioDevices(1); i++){ /* loop through input device buttons */
      int x = display_w / 4 - DEVICE_BUTTON_WIDTH / 2; /* x and y for testing the mouse click */
      int y = DEVICE_CONTENT_HEIGHT + (i + 1) * (DEVICE_BUTTON_HEIGHT + DEVICE_BUTTON_HEIGHT_SPACING);
      if(e->button.y > y && e->button.y < y + DEVICE_BUTTON_HEIGHT && e->button.x > x && e->button.x < x + DEVICE_BUTTON_WIDTH){ /* if click is within the button */
        set_input_audio_device(i, system); /* set audio input */
      }
    }
    for (i = 0; i < SDL_GetNumAudioDevices(0); i++){ /* loop through output device buttons */
      int x = display_w * 3 / 4 - DEVICE_BUTTON_WIDTH / 2; /* x and y for testing the mouse click */
      int y = DEVICE_CONTENT_HEIGHT + (i + 1) * (DEVICE_BUTTON_HEIGHT + DEVICE_BUTTON_HEIGHT_SPACING);
      if(e->button.y > y && e->button.y < y + DEVICE_BUTTON_HEIGHT && e->button.x > x && e->button.x < x + DEVICE_BUTTON_WIDTH){ /* if click is within the button */
        set_output_audio_device(i, system); /* set audio output */
      }
    }

    /* output test button */
    int x = display_w /2 - TEST_BUTTON_WIDTH / 2; /* x and y for testing the mouse click */
    int y = display_h - TEST_BUTTON_HEIGHT - TEST_BUTTON_HEIGHT_SPACING;
    if(e->button.y > y && e->button.y < y + TEST_BUTTON_HEIGHT && e->button.x > x && e->button.x < x + TEST_BUTTON_WIDTH){ /* if click is within the button */
      system->play_tone_end = SDL_GetTicks() + 2000; /* play 2 second of 440Hz Tone */
      system->play_tone_frequency = 440;
    }
  }


  *updatescreen = 1;
  return  0;
}

int select_audio_device_process(int* program_state, int* updatescreen, audio_system* system){

  return  0;
}

int select_audio_device_display(int* program_state, SDL_Renderer* renderer, audio_system* system){
  int display_w, display_h;
  SDL_GetRendererOutputSize(renderer, &display_w, &display_h);

  render_volume_bar(renderer, system, VOLUME_BAR_SPACING_Y_LARGE + UI_TOP_HEIGHT);

  Uint8 or, og, ob, oa; /* old render draw colors */
  SDL_GetRenderDrawColor(renderer, &or, &og, &ob, &oa); /* fill old color */
  SDL_Rect dstrect;/* make rectang */


  /* input buttons */
  dstrect.w = DEVICE_BUTTON_WIDTH; /* set rectangle height */
  dstrect.h = DEVICE_BUTTON_HEIGHT;
  dstrect.x = display_w / 4 - DEVICE_BUTTON_WIDTH / 2; /* set rectangle position */
  dstrect.y = DEVICE_CONTENT_HEIGHT;

  render_text_absolute_c(renderer, font_18, "Input", C_Text_Gray, dstrect.x + DEVICE_BUTTON_WIDTH / 2, dstrect.y + DEVICE_BUTTON_HEIGHT / 2); /* draw text */

  int i;
  for (i = 0; i < SDL_GetNumAudioDevices(1); i++){
    dstrect.x = display_w / 4 - DEVICE_BUTTON_WIDTH / 2; /* set rectangle position */
    dstrect.y = DEVICE_CONTENT_HEIGHT + (i + 1) * (DEVICE_BUTTON_HEIGHT + DEVICE_BUTTON_HEIGHT_SPACING);
    if (system->audio_input_index == i) {
      SDL_SetRenderDrawColor(renderer, C_UIL_Gray.r, C_UIL_Gray.g, C_UIL_Gray.b, C_UIL_Gray.a); /* set new color */
      SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */
      SDL_SetRenderDrawColor(renderer, C_UID_Gray.r, C_UID_Gray.g, C_UID_Gray.b, C_UID_Gray.a); /* set new color */
      SDL_RenderDrawRect(renderer, &dstrect); /* draw background rectangle */
      render_text_absolute_c(renderer, font_18, SDL_GetAudioDeviceName(i, 1), C_Text_Gray, dstrect.x + DEVICE_BUTTON_WIDTH / 2, dstrect.y + DEVICE_BUTTON_HEIGHT / 2); /* draw text */
    }
    else {
      SDL_SetRenderDrawColor(renderer, C_UI_Gray.r, C_UI_Gray.g, C_UI_Gray.b, C_UI_Gray.a); /* set new color */
      SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */
      SDL_SetRenderDrawColor(renderer, C_UID_Gray.r, C_UID_Gray.g, C_UID_Gray.b, C_UID_Gray.a); /* set new color */
      SDL_RenderDrawRect(renderer, &dstrect); /* draw background rectangle */
      render_text_absolute_c(renderer, font_18, SDL_GetAudioDeviceName(i, 1), C_Text_Gray, dstrect.x + DEVICE_BUTTON_WIDTH / 2, dstrect.y + DEVICE_BUTTON_HEIGHT / 2); /* draw text */
    }
  }


  /* output buttons */
  dstrect.w = DEVICE_BUTTON_WIDTH; /* set rectangle height */
  dstrect.h = DEVICE_BUTTON_HEIGHT;
  dstrect.x = display_w * 3 / 4 - DEVICE_BUTTON_WIDTH / 2; /* set rectangle position */
  dstrect.y = DEVICE_CONTENT_HEIGHT;

  render_text_absolute_c(renderer, font_18, "Output", C_Text_Gray, dstrect.x + DEVICE_BUTTON_WIDTH / 2, dstrect.y + DEVICE_BUTTON_HEIGHT / 2); /* draw text */

  for (i = 0; i < SDL_GetNumAudioDevices(0); i++){
    dstrect.x = display_w * 3 / 4 - DEVICE_BUTTON_WIDTH / 2; /* set rectangle position */
    dstrect.y = DEVICE_CONTENT_HEIGHT + (i + 1) * (DEVICE_BUTTON_HEIGHT + DEVICE_BUTTON_HEIGHT_SPACING);
    if (system->audio_output_index == i) {
      SDL_SetRenderDrawColor(renderer, C_UIL_Gray.r, C_UIL_Gray.g, C_UIL_Gray.b, C_UIL_Gray.a); /* set new color */
      SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */
      SDL_SetRenderDrawColor(renderer, C_UID_Gray.r, C_UID_Gray.g, C_UID_Gray.b, C_UID_Gray.a); /* set new color */
      SDL_RenderDrawRect(renderer, &dstrect); /* draw background rectangle */
      render_text_absolute_c(renderer, font_18, SDL_GetAudioDeviceName(i, 0), C_Text_Gray, dstrect.x + DEVICE_BUTTON_WIDTH / 2, dstrect.y + DEVICE_BUTTON_HEIGHT / 2); /* draw text */
    }
    else {
      SDL_SetRenderDrawColor(renderer, C_UI_Gray.r, C_UI_Gray.g, C_UI_Gray.b, C_UI_Gray.a); /* set new color */
      SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */
      SDL_SetRenderDrawColor(renderer, C_UID_Gray.r, C_UID_Gray.g, C_UID_Gray.b, C_UID_Gray.a); /* set new color */
      SDL_RenderDrawRect(renderer, &dstrect); /* draw background rectangle */
      render_text_absolute_c(renderer, font_18, SDL_GetAudioDeviceName(i, 0), C_Text_Gray, dstrect.x + DEVICE_BUTTON_WIDTH / 2, dstrect.y + DEVICE_BUTTON_HEIGHT / 2); /* draw text */
    }
  }

  /* output test button */
  dstrect.w = TEST_BUTTON_WIDTH; /* set rectangle height */
  dstrect.h = TEST_BUTTON_HEIGHT;
  dstrect.x = display_w /2 - TEST_BUTTON_WIDTH / 2; /* set rectangle position */
  dstrect.y = display_h - TEST_BUTTON_HEIGHT - TEST_BUTTON_HEIGHT_SPACING;

  SDL_SetRenderDrawColor(renderer, or, og, ob, oa); /* restore old color */

  SDL_SetRenderDrawColor(renderer, C_UI_Gray.r, C_UI_Gray.g, C_UI_Gray.b, C_UI_Gray.a); /* set new color */
  SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */
  SDL_SetRenderDrawColor(renderer, C_UID_Gray.r, C_UID_Gray.g, C_UID_Gray.b, C_UID_Gray.a); /* set new color */
  SDL_RenderDrawRect(renderer, &dstrect); /* draw background rectangle */
  render_text_absolute_c(renderer, font_18, "Test Output", C_Text_Gray, dstrect.x + TEST_BUTTON_WIDTH / 2, dstrect.y + TEST_BUTTON_HEIGHT / 2); /* draw text */

  return 0;
}
