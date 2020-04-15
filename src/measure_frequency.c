#include "measure_frequency.h"


int measure_frequency_events(int* program_state, int* updatescreen, SDL_Renderer* renderer, SDL_Event* e, audio_system* system){
  int display_w, display_h;
  SDL_GetRendererOutputSize(renderer, &display_w, &display_h);

  handle_volume_bar_event(renderer, e, system); /* take care of volume bar stuff */

  /* input frequency measurement channel select button */
  if(e->type == SDL_MOUSEBUTTONDOWN && e->button.y > CHANNEL_BUTTON_TOP_SPACING && e->button.y < CHANNEL_BUTTON_TOP_SPACING + CHANNEL_BUTTON_HEIGHT && e->button.x > CHANNEL_BUTTON_LEFT_SPACING && e->button.x < CHANNEL_BUTTON_LEFT_SPACING + CHANNEL_BUTTON_WIDTH){ /* if clicking  within the button */
    system->input_channel = !system->input_channel;
  }

  /* output type select button */
  if(e->type == SDL_MOUSEBUTTONDOWN && e->button.y > PLAYBACK_BUTTON_TOP_SPACING && e->button.y < PLAYBACK_BUTTON_TOP_SPACING + PLAYBACK_BUTTON_HEIGHT && e->button.x > display_w - PLAYBACK_BUTTON_WIDTH - PLAYBACK_BUTTON_RIGHT_SPACING && e->button.x < display_w - PLAYBACK_BUTTON_WIDTH - PLAYBACK_BUTTON_RIGHT_SPACING + PLAYBACK_BUTTON_WIDTH){ /* if clicking  within the button */
    system->playback_content = (system->playback_content + 1) % 3; /* switch to next one */
  }

  *updatescreen = 1;
  return  0;
}

int measure_frequency_process(int* program_state, int* updatescreen, audio_system* system){

  get_input_frequency(system); /* do the input frequency calculation */

  if (system->playback_content == 0){ /* if playing nothing */
    system->play_tone_end = 0; /* not playing any tone */
    system->input_monitor = 0; /* don't monitor input */
  }
  else if (system->playback_content == 1){ /* if playing back input audio */
    system->input_monitor = 1; /* monitor input */
  }
  else if (system->playback_content == 2 && system->input_frequency_detected && system->input_frequency > 40) { /* if playing the closest detected note and note is detected */
    /* also check that the note is actually detected, and is of reasonably high frequency */
    system->input_monitor = 0; /* don't monitor input */
    system->play_tone_end = SDL_GetTicks() + (PLAYBACK_BUFFER_LENGTH * 1000.0)/SAMPLE_RATE;
    system->play_tone_frequency = midi_to_f(system->input_midi_note); /* play back input midi note */
  }

  return  0;
}

int measure_frequency_display(int* program_state, SDL_Renderer* renderer, audio_system* system){
  int display_w, display_h;
  SDL_GetRendererOutputSize(renderer, &display_w, &display_h);

  render_volume_bar(renderer, system, VOLUME_BAR_SPACING_Y_LARGE + UI_TOP_HEIGHT);


  /* frequency measurement left right select button */
  Uint8 or, og, ob, oa; /* old render draw colors */
  SDL_GetRenderDrawColor(renderer, &or, &og, &ob, &oa); /* fill old color */

  SDL_Rect dstrect;/* make rectangle */
  dstrect.w = CHANNEL_BUTTON_WIDTH; /* set rectangle height */
  dstrect.h = CHANNEL_BUTTON_HEIGHT;
  dstrect.x = CHANNEL_BUTTON_LEFT_SPACING; /* set rectangle position */
  dstrect.y = CHANNEL_BUTTON_TOP_SPACING;

  SDL_SetRenderDrawColor(renderer, C_UIL_Gray.r, C_UIL_Gray.g, C_UIL_Gray.b, C_UIL_Gray.a); /* set new color */
  SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */
  SDL_SetRenderDrawColor(renderer, C_UID_Gray.r, C_UID_Gray.g, C_UID_Gray.b, C_UID_Gray.a); /* set new color */
  SDL_RenderDrawRect(renderer, &dstrect); /* draw background rectangle */
  render_text_absolute_c(renderer, font_18, system->input_channel ? "R" : "L", C_Text_Gray, dstrect.x + CHANNEL_BUTTON_WIDTH / 2, dstrect.y + CHANNEL_BUTTON_HEIGHT / 2); /* draw text */

  /* output type select button */
  dstrect.w = PLAYBACK_BUTTON_WIDTH; /* set rectangle height */
  dstrect.h = PLAYBACK_BUTTON_HEIGHT;
  dstrect.x = display_w - PLAYBACK_BUTTON_WIDTH - PLAYBACK_BUTTON_RIGHT_SPACING; /* set rectangle position */
  dstrect.y = PLAYBACK_BUTTON_TOP_SPACING;

  SDL_SetRenderDrawColor(renderer, C_UIL_Gray.r, C_UIL_Gray.g, C_UIL_Gray.b, C_UIL_Gray.a); /* set new color */
  SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */
  SDL_SetRenderDrawColor(renderer, C_UID_Gray.r, C_UID_Gray.g, C_UID_Gray.b, C_UID_Gray.a); /* set new color */
  SDL_RenderDrawRect(renderer, &dstrect); /* draw background rectangle */

  if (system->playback_content == 0){
    render_text_absolute_c(renderer, font_18, "No Out", C_Text_Gray, dstrect.x + PLAYBACK_BUTTON_WIDTH / 2, dstrect.y + PLAYBACK_BUTTON_HEIGHT / 2); /* draw text */
  }
  else if (system->playback_content == 1){
    render_text_absolute_c(renderer, font_18, "Monitor", C_Text_Gray, dstrect.x + PLAYBACK_BUTTON_WIDTH / 2, dstrect.y + PLAYBACK_BUTTON_HEIGHT / 2); /* draw text */
  }
  else if (system->playback_content == 2) {
    render_text_absolute_c(renderer, font_18, "Note", C_Text_Gray, dstrect.x + PLAYBACK_BUTTON_WIDTH / 2, dstrect.y + PLAYBACK_BUTTON_HEIGHT / 2); /* draw text */
  }

  SDL_SetRenderDrawColor(renderer, or, og, ob, oa); /* restore old color */


  if (system->input_frequency_detected) { /* if there is input frequency to display */
    char frequency_text[64];
    sprintf(frequency_text, "%5.2fHz", system->input_frequency); /* make the frequency text */
    char midi_text[64];
    sprintf(midi_text, "MIDI# %li", system->input_midi_note); /* make the midi note number text */

    render_text_absolute_c(renderer, font_60, frequency_text, C_White, FREQUENCY_X*display_w, FREQUENCY_HEIGHT); /* draw current frequency */
    render_text_absolute_c(renderer, font_60, midi_text, C_White, MIDI_NUM_X*display_w, FREQUENCY_HEIGHT_2); /* draw current note in midi number*/
    render_text_absolute_c(renderer, font_60, get_note_name_english(system->input_midi_note), C_White, NOTE_X*display_w, FREQUENCY_HEIGHT_2); /* draw current note */
  }
  else {
    render_text_absolute_c(renderer, font_60, "NFD", C_White, FREQUENCY_X*display_w, FREQUENCY_HEIGHT); /* draw current frequency */
    render_text_absolute_c(renderer, font_60, "MIDI# NA", C_White, MIDI_NUM_X*display_w, FREQUENCY_HEIGHT_2); /* draw current note in midi number*/
    render_text_absolute_c(renderer, font_60, "NA", C_White, NOTE_X*display_w, FREQUENCY_HEIGHT_2); /* draw current note */
  }

  return 0;
}
