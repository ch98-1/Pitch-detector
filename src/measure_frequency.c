#include "measure_frequency.h"


int measure_frequency_events(int* program_state, int* updatescreen, SDL_Renderer* renderer, SDL_Event* e, audio_system* system){

  handle_volume_bar_event(renderer, e, system); /* take care of volume bar stuff */

  /* input frequency measurement channel select button */
  if(e->type == SDL_MOUSEBUTTONDOWN && e->button.y > CHANNEL_BUTTON_TOP_SPACING && e->button.y < CHANNEL_BUTTON_TOP_SPACING + CHANNEL_BUTTON_HEIGHT && e->button.x > CHANNEL_BUTTON_LEFT_SPACING && e->button.x < CHANNEL_BUTTON_TOP_SPACING + CHANNEL_BUTTON_WIDTH){ /* if clicking  within the button */
    system->input_channel = !system->input_channel;
  }

  *updatescreen = 1;
  return  0;
}

int measure_frequency_process(int* program_state, int* updatescreen, audio_system* system){
  system->play_tone_end = 0; /* not playing any tone */

  get_input_frequency(system); /* do the input frequency calculation */
  
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
