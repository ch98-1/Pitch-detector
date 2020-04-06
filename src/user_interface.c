#include "user_interface.h"

int draw_top_ui(int* program_state, SDL_Renderer* renderer){

  int display_w, display_h;
  SDL_GetRendererOutputSize(renderer, &display_w, &display_h); /* get display w and h */

  Uint8 or, og, ob, oa; /* old render draw colors */
  SDL_GetRenderDrawColor(renderer, &or, &og, &ob, &oa); /* fill old color */
  SDL_Rect dstrect;/* make rectang */


  dstrect.x = 0*display_w; /* set rectangle */
  dstrect.y = 0;
  dstrect.w = display_w/4.0;
  dstrect.h = UI_TOP_HEIGHT;
  if (*program_state == MEASURE_FREQUENCY) {
    SDL_SetRenderDrawColor(renderer, C_UIL_Gray.r, C_UIL_Gray.g, C_UIL_Gray.b, C_UIL_Gray.a); /* set new color */
    SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */
    SDL_SetRenderDrawColor(renderer, C_UID_Gray.r, C_UID_Gray.g, C_UID_Gray.b, C_UID_Gray.a); /* set new color */
    SDL_RenderDrawRect(renderer, &dstrect); /* draw background rectangle */
    render_text_absolute_c(renderer, font_18, "Frequency", C_Text_Gray, dstrect.x + display_w/(4.0*2.0), dstrect.y + UI_TOP_HEIGHT / 2); /* draw text */
  }
  else {
    SDL_SetRenderDrawColor(renderer, C_UI_Gray.r, C_UI_Gray.g, C_UI_Gray.b, C_UI_Gray.a); /* set new color */
    SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */
    SDL_SetRenderDrawColor(renderer, C_UID_Gray.r, C_UID_Gray.g, C_UID_Gray.b, C_UID_Gray.a); /* set new color */
    SDL_RenderDrawRect(renderer, &dstrect); /* draw background rectangle */
    render_text_absolute_c(renderer, font_18, "Frequency", C_Text_Gray, dstrect.x + display_w/(4.0*2.0), dstrect.y + UI_TOP_HEIGHT / 2); /* draw text */
  }

  dstrect.x = 1.0/4.0*display_w; /* set rectangle position*/
  if (*program_state == SELECT_AUDIO_DRIVER) {
    SDL_SetRenderDrawColor(renderer, C_UIL_Gray.r, C_UIL_Gray.g, C_UIL_Gray.b, C_UIL_Gray.a); /* set new color */
    SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */
    SDL_SetRenderDrawColor(renderer, C_UID_Gray.r, C_UID_Gray.g, C_UID_Gray.b, C_UID_Gray.a); /* set new color */
    SDL_RenderDrawRect(renderer, &dstrect); /* draw background rectangle */
    render_text_absolute_c(renderer, font_18, "Driver", C_Text_Gray, dstrect.x + display_w/(4.0*2.0), dstrect.y + UI_TOP_HEIGHT / 2); /* draw text */
  }
  else {
    SDL_SetRenderDrawColor(renderer, C_UI_Gray.r, C_UI_Gray.g, C_UI_Gray.b, C_UI_Gray.a); /* set new color */
    SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */
    SDL_SetRenderDrawColor(renderer, C_UID_Gray.r, C_UID_Gray.g, C_UID_Gray.b, C_UID_Gray.a); /* set new color */
    SDL_RenderDrawRect(renderer, &dstrect); /* draw background rectangle */
    render_text_absolute_c(renderer, font_18, "Driver", C_Text_Gray, dstrect.x + display_w/(4.0*2.0), dstrect.y + UI_TOP_HEIGHT / 2); /* draw text */
  }

  dstrect.x = 2.0/4.0*display_w; /* set rectangle position*/
  if (*program_state == SELECT_AUDIO_DEVICE) {
    SDL_SetRenderDrawColor(renderer, C_UIL_Gray.r, C_UIL_Gray.g, C_UIL_Gray.b, C_UIL_Gray.a); /* set new color */
    SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */
    SDL_SetRenderDrawColor(renderer, C_UID_Gray.r, C_UID_Gray.g, C_UID_Gray.b, C_UID_Gray.a); /* set new color */
    SDL_RenderDrawRect(renderer, &dstrect); /* draw background rectangle */
    render_text_absolute_c(renderer, font_18, "Device", C_Text_Gray, dstrect.x + display_w/(4.0*2.0), dstrect.y + UI_TOP_HEIGHT / 2); /* draw text */
  }
  else {
    SDL_SetRenderDrawColor(renderer, C_UI_Gray.r, C_UI_Gray.g, C_UI_Gray.b, C_UI_Gray.a); /* set new color */
    SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */
    SDL_SetRenderDrawColor(renderer, C_UID_Gray.r, C_UID_Gray.g, C_UID_Gray.b, C_UID_Gray.a); /* set new color */
    SDL_RenderDrawRect(renderer, &dstrect); /* draw background rectangle */
    render_text_absolute_c(renderer, font_18, "Device", C_Text_Gray, dstrect.x + display_w/(4.0*2.0), dstrect.y + UI_TOP_HEIGHT / 2); /* draw text */
  }

  dstrect.x = 3.0/4.0*display_w; /* set rectangle position*/
  if (*program_state == CREDITS) {
    SDL_SetRenderDrawColor(renderer, C_UIL_Gray.r, C_UIL_Gray.g, C_UIL_Gray.b, C_UIL_Gray.a); /* set new color */
    SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */
    SDL_SetRenderDrawColor(renderer, C_UID_Gray.r, C_UID_Gray.g, C_UID_Gray.b, C_UID_Gray.a); /* set new color */
    SDL_RenderDrawRect(renderer, &dstrect); /* draw background rectangle */
    render_text_absolute_c(renderer, font_18, "Credits", C_Text_Gray, dstrect.x + display_w/(4.0*2.0), dstrect.y + UI_TOP_HEIGHT / 2); /* draw text */
  }
  else {
    SDL_SetRenderDrawColor(renderer, C_UI_Gray.r, C_UI_Gray.g, C_UI_Gray.b, C_UI_Gray.a); /* set new color */
    SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */
    SDL_SetRenderDrawColor(renderer, C_UID_Gray.r, C_UID_Gray.g, C_UID_Gray.b, C_UID_Gray.a); /* set new color */
    SDL_RenderDrawRect(renderer, &dstrect); /* draw background rectangle */
    render_text_absolute_c(renderer, font_18, "Credits", C_Text_Gray, dstrect.x + display_w/(4.0*2.0), dstrect.y + UI_TOP_HEIGHT / 2); /* draw text */
  }

  SDL_SetRenderDrawColor(renderer, or, og, ob, oa); /* restore old color */
  return 0;
}

int handle_top_mouseclick(int* program_state, int* updatescreen, float x){
  if (x < 1.0/4.0){ /* set state based on where the mouse clicked */
    *program_state = MEASURE_FREQUENCY;
  }
  else if ( x < 2.0/4.0){
    *program_state = SELECT_AUDIO_DRIVER;
  }
  else if ( x < 3.0/4.0){
    *program_state = SELECT_AUDIO_DEVICE;
  }
  else{
    *program_state = CREDITS;
  }

  *updatescreen = 1;

  return 0;
}
