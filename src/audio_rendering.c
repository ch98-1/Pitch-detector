#include "audio_rendering.h"

SDL_Texture* volume_bar_texture;
SDL_Texture* input_icon_texture;
SDL_Texture* output_icon_texture;

float get_db_limited(float volume){/* get db with lower limit */
  float db = 20*log10(volume);
  if (db < DB_LIMIT) {
    return DB_LIMIT - 0.01; /* adjustment factor to fix some graphics stuff */
  }
  else {
    return db;
  }
}

float get_db_limited_normalized(float volume) {/* get db normalised from 0 to 1 */
  return (((-1 * DB_LIMIT) + get_db_limited(volume)) / (-1 * DB_LIMIT));
}

/* render a full width volume bar with top edge at height of y
volumes should be normalised with max at 1
Will strech to full width of the window with some margins*/
int render_volume_bar(SDL_Renderer* renderer, audio_system* system, int y){
  if (!volume_bar_texture){ /* load credit texture if empty */
    char image_dir[DIR_LENGTH];
    strcpy(image_dir, system->data_path);
    strcat(image_dir, "volume_bar.png");
    volume_bar_texture = get_image_texture(renderer, image_dir);
  }
  if (!input_icon_texture){ /* load credit texture if empty */
    char image_dir[DIR_LENGTH];
    strcpy(image_dir, system->data_path);
    strcat(image_dir, "mic_icon.png");
    input_icon_texture = get_image_texture(renderer, image_dir);
  }
  if (!output_icon_texture){ /* load credit texture if empty */
    char image_dir[DIR_LENGTH];
    strcpy(image_dir, system->data_path);
    strcat(image_dir, "headphone_icon.png");
    output_icon_texture = get_image_texture(renderer, image_dir);
  }

  int display_w, display_h;
  SDL_GetRendererOutputSize(renderer, &display_w, &display_h); /* get display w and h */

  Uint8 or, og, ob, oa; /* old render draw colors */
  SDL_GetRenderDrawColor(renderer, &or, &og, &ob, &oa); /* fill old color */
  SDL_Rect dstrect;/* set output rectangle */

  int volume_bar_start_x = VOLUME_BAR_SPACING_X + VOLUME_BAR_HEIGHT * 2 + VOLUME_BAR_SPACING_Y + VOLUME_BAR_SPACING_X * 1.5;
  int volume_bar_width = display_w - volume_bar_start_x - VOLUME_BAR_SPACING_X;

  dstrect.x = VOLUME_BAR_SPACING_X - 1; /* background rectangle */
  dstrect.y = y - 1;
  dstrect.w = display_w-(VOLUME_BAR_SPACING_X * 2) + 2;
  dstrect.h = VOLUME_BAR_HEIGHT * 4 + VOLUME_BAR_SPACING_Y * 2 + VOLUME_BAR_SPACING_Y_LARGE + 2;
  SDL_SetRenderDrawColor(renderer, C_UI_Gray.r, C_UI_Gray.g, C_UI_Gray.b, C_UI_Gray.a); /* set new color */
  SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */

  int input_on = 0;
  int output_on = 0;
  float output_vol = 0;

  if (SDL_LockMutex(system->audio_system_mutex) == 0) { /* handle mutex for system */
    input_on = system->input_on;
    output_on = system->output_on;
    output_vol = system->output_vol;
    SDL_UnlockMutex(system->audio_system_mutex);
  }
  else { /* error message for when mutex breaks */
    printf("Unable to lock mutex: %s\n", SDL_GetError());
  }

  if (input_on){ /* choose color depending on input status */
    SDL_SetRenderDrawColor(renderer, C_UIL_Gray.r, C_UIL_Gray.g, C_UIL_Gray.b, C_UIL_Gray.a);
  }
  else {
    SDL_SetRenderDrawColor(renderer, C_UI_Gray.r, C_UI_Gray.g, C_UI_Gray.b, C_UI_Gray.a);
  }

  dstrect.x = VOLUME_BAR_SPACING_X; /* input volume icon */
  dstrect.y = y;
  dstrect.w = VOLUME_BAR_HEIGHT * 2 + VOLUME_BAR_SPACING_Y;
  dstrect.h = dstrect.w;
  SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */
  SDL_RenderCopy(renderer, input_icon_texture, NULL, &dstrect);


  if (output_on){ /* choose color depending on output status */
    SDL_SetRenderDrawColor(renderer, C_UIL_Gray.r, C_UIL_Gray.g, C_UIL_Gray.b, C_UIL_Gray.a);
  }
  else {
    SDL_SetRenderDrawColor(renderer, C_UI_Gray.r, C_UI_Gray.g, C_UI_Gray.b, C_UI_Gray.a);
  }

  dstrect.y = y + dstrect.w + VOLUME_BAR_SPACING_Y_LARGE; /* output volume icon */
  SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */
  SDL_RenderCopy(renderer, output_icon_texture, NULL, &dstrect);


  float in_l_vol = 0; /* volumes to display on volume bar */
  float in_r_vol = 0;
  float in_l_vol_max = 0;
  float in_r_vol_max = 0;
  float out_l_vol = 0;
  float out_r_vol = 0;
  float out_l_vol_max = 0;
  float out_r_vol_max = 0;

  if (SDL_LockMutex(system->audio_volume_mutex) == 0) { /* handle mutex for volume */
    in_l_vol = system->in_l_vol; /* set values */
    in_r_vol = system->in_r_vol;
    in_l_vol_max = system->in_l_vol_max;
    in_r_vol_max = system->in_r_vol_max;
    out_l_vol = system->out_l_vol;
    out_r_vol = system->out_r_vol;
    out_l_vol_max = system->out_l_vol_max;
    out_r_vol_max = system->out_r_vol_max;
  SDL_UnlockMutex(system->audio_volume_mutex);
  }
  else { /* error message for when mutex breaks */
    printf("Unable to lock mutex: %s\n", SDL_GetError());
  }


  dstrect.x = volume_bar_start_x; /* left input */
  dstrect.y = y;
  dstrect.w = volume_bar_width;
  dstrect.h = VOLUME_BAR_HEIGHT;
  render_text_absolute_tl(renderer, font_10, "L", C_Text_Gray, volume_bar_start_x - VOLUME_BAR_SPACING_X, dstrect.y );
  SDL_RenderCopy(renderer, volume_bar_texture, NULL, &dstrect);
  dstrect.x = volume_bar_start_x + get_db_limited_normalized(in_l_vol) * volume_bar_width; /* left input volume bar */
  dstrect.w = volume_bar_width * (get_db_limited_normalized(in_l_vol_max) - get_db_limited_normalized(in_l_vol));
  SDL_SetRenderDrawColor(renderer, C_UIL_Gray.r, C_UIL_Gray.g, C_UIL_Gray.b, C_UIL_Gray.a); /* set new color */
  SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */
  dstrect.x = volume_bar_start_x + volume_bar_width * get_db_limited_normalized(in_l_vol_max) + 1; /* left input volume bar max */
  dstrect.w = volume_bar_width - volume_bar_width * get_db_limited_normalized(in_l_vol_max);
  SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */

  dstrect.x = volume_bar_start_x; /* right input */
  dstrect.y = y + VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y;
  dstrect.w = volume_bar_width;
  dstrect.h = VOLUME_BAR_HEIGHT;
  render_text_absolute_tl(renderer, font_10, "R", C_Text_Gray, volume_bar_start_x - VOLUME_BAR_SPACING_X, dstrect.y );
  SDL_RenderCopy(renderer, volume_bar_texture, NULL, &dstrect);
  dstrect.x = volume_bar_start_x + get_db_limited_normalized(in_r_vol) * volume_bar_width; /* left input volume bar */
  dstrect.w = volume_bar_width * (get_db_limited_normalized(in_r_vol_max) - get_db_limited_normalized(in_r_vol));
  SDL_SetRenderDrawColor(renderer, C_UIL_Gray.r, C_UIL_Gray.g, C_UIL_Gray.b, C_UIL_Gray.a); /* set new color */
  SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */
  dstrect.x = volume_bar_start_x + volume_bar_width * get_db_limited_normalized(in_r_vol_max) + 1; /* left input volume bar max */
  dstrect.w = volume_bar_width - volume_bar_width * get_db_limited_normalized(in_r_vol_max);
  SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */


  dstrect.x = volume_bar_start_x; /* left output */
  dstrect.y = y + VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y + VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y_LARGE;
  dstrect.w = volume_bar_width;
  dstrect.h = VOLUME_BAR_HEIGHT;
  render_text_absolute_tl(renderer, font_10, "L", C_Text_Gray, volume_bar_start_x - VOLUME_BAR_SPACING_X, dstrect.y );
  SDL_RenderCopy(renderer, volume_bar_texture, NULL, &dstrect);
  dstrect.x = volume_bar_start_x + get_db_limited_normalized(out_l_vol) * volume_bar_width; /* left input volume bar */
  dstrect.w = volume_bar_width * (get_db_limited_normalized(out_l_vol_max) - get_db_limited_normalized(out_l_vol));
  SDL_SetRenderDrawColor(renderer, C_UIL_Gray.r, C_UIL_Gray.g, C_UIL_Gray.b, C_UIL_Gray.a); /* set new color */
  SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */
  dstrect.x = volume_bar_start_x + volume_bar_width * get_db_limited_normalized(out_l_vol_max) + 1; /* left input volume bar max */
  dstrect.w = volume_bar_width - volume_bar_width * get_db_limited_normalized(out_l_vol_max);
  SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */

  dstrect.x = volume_bar_start_x; /* right output */
  dstrect.y = y + VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y + VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y_LARGE + VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y;
  dstrect.w = volume_bar_width;
  dstrect.h = VOLUME_BAR_HEIGHT;
  render_text_absolute_tl(renderer, font_10, "R", C_Text_Gray, volume_bar_start_x - VOLUME_BAR_SPACING_X, dstrect.y );
  SDL_RenderCopy(renderer, volume_bar_texture, NULL, &dstrect);
  dstrect.x = volume_bar_start_x + get_db_limited_normalized(out_r_vol) * volume_bar_width; /* left input volume bar */
  dstrect.w = volume_bar_width * (get_db_limited_normalized(out_r_vol_max) - get_db_limited_normalized(out_r_vol));
  SDL_SetRenderDrawColor(renderer, C_UIL_Gray.r, C_UIL_Gray.g, C_UIL_Gray.b, C_UIL_Gray.a); /* set new color */
  SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */
  dstrect.x = volume_bar_start_x + volume_bar_width * get_db_limited_normalized(out_r_vol_max) + 1; /* left input volume bar max */
  dstrect.w = volume_bar_width - volume_bar_width * get_db_limited_normalized(out_r_vol_max);
  SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */



  int vol_x_pos; /* x position for volume markings */
  int vol_y_pos = y + VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y + VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y_LARGE/2;

  /* number the volume bar */
  SDL_SetRenderDrawColor(renderer, C_Black.r, C_Black.g, C_Black.b, C_Black.a); /* set new color */

  vol_x_pos = volume_bar_start_x + (1 - 0.01/(-1 * DB_LIMIT)) * volume_bar_width; /* 0 db */
  SDL_RenderDrawLine(renderer, vol_x_pos, vol_y_pos - VOLUME_BAR_SPACING_Y_LARGE/2 - VOLUME_BAR_HEIGHT - VOLUME_BAR_SPACING_Y*2, vol_x_pos, vol_y_pos - VOLUME_BAR_SPACING_Y_LARGE/2 - VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y); /* tick on input */
  SDL_RenderDrawLine(renderer, vol_x_pos, vol_y_pos + VOLUME_BAR_SPACING_Y_LARGE/2 + VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y*2 - 1, vol_x_pos, vol_y_pos + VOLUME_BAR_SPACING_Y_LARGE/2 + VOLUME_BAR_HEIGHT - VOLUME_BAR_SPACING_Y - 1); /* tick on input */
  render_text_absolute_c(renderer, font_10, "0", C_Text_Gray, vol_x_pos, vol_y_pos); /* draw text */

  vol_x_pos = volume_bar_start_x + (1 - 6.0/(-1 * DB_LIMIT)) * volume_bar_width; /* -6 db */
  SDL_RenderDrawLine(renderer, vol_x_pos, vol_y_pos - VOLUME_BAR_SPACING_Y_LARGE/2 - VOLUME_BAR_HEIGHT - VOLUME_BAR_SPACING_Y*2, vol_x_pos, vol_y_pos - VOLUME_BAR_SPACING_Y_LARGE/2 - VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y); /* tick on input */
  SDL_RenderDrawLine(renderer, vol_x_pos, vol_y_pos + VOLUME_BAR_SPACING_Y_LARGE/2 + VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y*2 - 1, vol_x_pos, vol_y_pos + VOLUME_BAR_SPACING_Y_LARGE/2 + VOLUME_BAR_HEIGHT - VOLUME_BAR_SPACING_Y - 1); /* tick on input */
  render_text_absolute_c(renderer, font_10, "-6", C_Text_Gray, vol_x_pos, vol_y_pos); /* draw text */

  vol_x_pos = volume_bar_start_x + (1 - 12.0/(-1 * DB_LIMIT)) * volume_bar_width; /* -12 db */
  SDL_RenderDrawLine(renderer, vol_x_pos, vol_y_pos - VOLUME_BAR_SPACING_Y_LARGE/2 - VOLUME_BAR_HEIGHT - VOLUME_BAR_SPACING_Y*2, vol_x_pos, vol_y_pos - VOLUME_BAR_SPACING_Y_LARGE/2 - VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y); /* tick on input */
  SDL_RenderDrawLine(renderer, vol_x_pos, vol_y_pos + VOLUME_BAR_SPACING_Y_LARGE/2 + VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y*2 - 1, vol_x_pos, vol_y_pos + VOLUME_BAR_SPACING_Y_LARGE/2 + VOLUME_BAR_HEIGHT - VOLUME_BAR_SPACING_Y - 1); /* tick on input */
  render_text_absolute_c(renderer, font_10, "-12", C_Text_Gray, vol_x_pos, vol_y_pos); /* draw text */

  vol_x_pos = volume_bar_start_x + (1 - 18.0/(-1 * DB_LIMIT)) * volume_bar_width; /* -18 db */
  SDL_RenderDrawLine(renderer, vol_x_pos, vol_y_pos - VOLUME_BAR_SPACING_Y_LARGE/2 - VOLUME_BAR_HEIGHT - VOLUME_BAR_SPACING_Y*2, vol_x_pos, vol_y_pos - VOLUME_BAR_SPACING_Y_LARGE/2 - VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y); /* tick on input */
  SDL_RenderDrawLine(renderer, vol_x_pos, vol_y_pos + VOLUME_BAR_SPACING_Y_LARGE/2 + VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y*2 - 1, vol_x_pos, vol_y_pos + VOLUME_BAR_SPACING_Y_LARGE/2 + VOLUME_BAR_HEIGHT - VOLUME_BAR_SPACING_Y - 1); /* tick on input */
  render_text_absolute_c(renderer, font_10, "-18", C_Text_Gray, vol_x_pos, vol_y_pos); /* draw text */

  vol_x_pos = volume_bar_start_x + (1 - 24.0/(-1 * DB_LIMIT)) * volume_bar_width; /* -24 db */
  SDL_RenderDrawLine(renderer, vol_x_pos, vol_y_pos - VOLUME_BAR_SPACING_Y_LARGE/2 - VOLUME_BAR_HEIGHT - VOLUME_BAR_SPACING_Y*2, vol_x_pos, vol_y_pos - VOLUME_BAR_SPACING_Y_LARGE/2 - VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y); /* tick on input */
  SDL_RenderDrawLine(renderer, vol_x_pos, vol_y_pos + VOLUME_BAR_SPACING_Y_LARGE/2 + VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y*2 - 1, vol_x_pos, vol_y_pos + VOLUME_BAR_SPACING_Y_LARGE/2 + VOLUME_BAR_HEIGHT - VOLUME_BAR_SPACING_Y - 1); /* tick on input */
  render_text_absolute_c(renderer, font_10, "-24", C_Text_Gray, vol_x_pos, vol_y_pos); /* draw text */

  vol_x_pos = volume_bar_start_x + (1 - 30.0/(-1 * DB_LIMIT)) * volume_bar_width; /* -30 db */
  SDL_RenderDrawLine(renderer, vol_x_pos, vol_y_pos - VOLUME_BAR_SPACING_Y_LARGE/2 - VOLUME_BAR_HEIGHT - VOLUME_BAR_SPACING_Y*2, vol_x_pos, vol_y_pos - VOLUME_BAR_SPACING_Y_LARGE/2 - VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y); /* tick on input */
  SDL_RenderDrawLine(renderer, vol_x_pos, vol_y_pos + VOLUME_BAR_SPACING_Y_LARGE/2 + VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y*2 - 1, vol_x_pos, vol_y_pos + VOLUME_BAR_SPACING_Y_LARGE/2 + VOLUME_BAR_HEIGHT - VOLUME_BAR_SPACING_Y - 1); /* tick on input */
  render_text_absolute_c(renderer, font_10, "-30", C_Text_Gray, vol_x_pos, vol_y_pos); /* draw text */

  vol_x_pos = volume_bar_start_x + (1 - 36.0/(-1 * DB_LIMIT)) * volume_bar_width; /* -36 db */
  SDL_RenderDrawLine(renderer, vol_x_pos, vol_y_pos - VOLUME_BAR_SPACING_Y_LARGE/2 - VOLUME_BAR_HEIGHT - VOLUME_BAR_SPACING_Y*2, vol_x_pos, vol_y_pos - VOLUME_BAR_SPACING_Y_LARGE/2 - VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y); /* tick on input */
  SDL_RenderDrawLine(renderer, vol_x_pos, vol_y_pos + VOLUME_BAR_SPACING_Y_LARGE/2 + VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y*2 - 1, vol_x_pos, vol_y_pos + VOLUME_BAR_SPACING_Y_LARGE/2 + VOLUME_BAR_HEIGHT - VOLUME_BAR_SPACING_Y - 1); /* tick on input */
  render_text_absolute_c(renderer, font_10, "-36", C_Text_Gray, vol_x_pos, vol_y_pos); /* draw text */

  vol_x_pos = volume_bar_start_x + (1 - 42.0/(-1 * DB_LIMIT)) * volume_bar_width; /* -42 db */
  SDL_RenderDrawLine(renderer, vol_x_pos, vol_y_pos - VOLUME_BAR_SPACING_Y_LARGE/2 - VOLUME_BAR_HEIGHT - VOLUME_BAR_SPACING_Y*2, vol_x_pos, vol_y_pos - VOLUME_BAR_SPACING_Y_LARGE/2 - VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y); /* tick on input */
  SDL_RenderDrawLine(renderer, vol_x_pos, vol_y_pos + VOLUME_BAR_SPACING_Y_LARGE/2 + VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y*2 - 1, vol_x_pos, vol_y_pos + VOLUME_BAR_SPACING_Y_LARGE/2 + VOLUME_BAR_HEIGHT - VOLUME_BAR_SPACING_Y - 1); /* tick on input */
  render_text_absolute_c(renderer, font_10, "-42", C_Text_Gray, vol_x_pos, vol_y_pos); /* draw text */

  vol_x_pos = volume_bar_start_x + (1 - 48.0/(-1 * DB_LIMIT)) * volume_bar_width; /* -48 db */
  SDL_RenderDrawLine(renderer, vol_x_pos, vol_y_pos - VOLUME_BAR_SPACING_Y_LARGE/2 - VOLUME_BAR_HEIGHT - VOLUME_BAR_SPACING_Y*2, vol_x_pos, vol_y_pos - VOLUME_BAR_SPACING_Y_LARGE/2 - VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y); /* tick on input */
  SDL_RenderDrawLine(renderer, vol_x_pos, vol_y_pos + VOLUME_BAR_SPACING_Y_LARGE/2 + VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y*2 - 1, vol_x_pos, vol_y_pos + VOLUME_BAR_SPACING_Y_LARGE/2 + VOLUME_BAR_HEIGHT - VOLUME_BAR_SPACING_Y - 1); /* tick on input */
  render_text_absolute_c(renderer, font_10, "-48", C_Text_Gray, vol_x_pos, vol_y_pos); /* draw text */

  vol_x_pos = volume_bar_start_x + (1 - 54.0/(-1 * DB_LIMIT)) * volume_bar_width; /* -54 db */
  SDL_RenderDrawLine(renderer, vol_x_pos, vol_y_pos - VOLUME_BAR_SPACING_Y_LARGE/2 - VOLUME_BAR_HEIGHT - VOLUME_BAR_SPACING_Y*2, vol_x_pos, vol_y_pos - VOLUME_BAR_SPACING_Y_LARGE/2 - VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y); /* tick on input */
  SDL_RenderDrawLine(renderer, vol_x_pos, vol_y_pos + VOLUME_BAR_SPACING_Y_LARGE/2 + VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y*2 - 1, vol_x_pos, vol_y_pos + VOLUME_BAR_SPACING_Y_LARGE/2 + VOLUME_BAR_HEIGHT - VOLUME_BAR_SPACING_Y - 1); /* tick on input */
  render_text_absolute_c(renderer, font_10, "-54", C_Text_Gray, vol_x_pos, vol_y_pos); /* draw text */

  vol_x_pos = volume_bar_start_x + (1 - 60.0/(-1 * DB_LIMIT)) * volume_bar_width; /* -60 db */
  SDL_RenderDrawLine(renderer, vol_x_pos, vol_y_pos - VOLUME_BAR_SPACING_Y_LARGE/2 - VOLUME_BAR_HEIGHT - VOLUME_BAR_SPACING_Y*2, vol_x_pos, vol_y_pos - VOLUME_BAR_SPACING_Y_LARGE/2 - VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y); /* tick on input */
  SDL_RenderDrawLine(renderer, vol_x_pos, vol_y_pos + VOLUME_BAR_SPACING_Y_LARGE/2 + VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y*2 - 1, vol_x_pos, vol_y_pos + VOLUME_BAR_SPACING_Y_LARGE/2 + VOLUME_BAR_HEIGHT - VOLUME_BAR_SPACING_Y - 1); /* tick on input */
  render_text_absolute_c(renderer, font_10, "-60", C_Text_Gray, vol_x_pos, vol_y_pos); /* draw text */


  SDL_SetRenderDrawColor(renderer, C_Text_Gray.r, C_Text_Gray.g, C_Text_Gray.b, C_Text_Gray.a); /* set new color for volume slider */
  dstrect.x = VOLUME_BAR_SPACING_X * 2;
  dstrect.y = VOLUME_SLIDER_HEIGHT - 1;
  dstrect.w = display_w - VOLUME_BAR_SPACING_X*4;
  dstrect.h = 2;
  SDL_RenderFillRect(renderer, &dstrect); /* draw volume slider */

  int i; /* draw ticks for volume slider */
  for (i = 0; i <= 100; i += 5){
    vol_x_pos = dstrect.x + (i/100.0) * dstrect.w; /* volume 0 */
    SDL_RenderDrawLine(renderer, vol_x_pos, VOLUME_SLIDER_HEIGHT - VOLUME_BAR_SPACING_Y, vol_x_pos, VOLUME_SLIDER_HEIGHT + VOLUME_BAR_SPACING_Y); /* tick on input */
  }
  for (i = 0; i <= 100; i += 20){
    vol_x_pos = dstrect.x + (i/100.0) * dstrect.w; /* volume 0 */
    SDL_RenderDrawLine(renderer, vol_x_pos, VOLUME_SLIDER_HEIGHT - VOLUME_BAR_SPACING_Y*1.2, vol_x_pos, VOLUME_SLIDER_HEIGHT + VOLUME_BAR_SPACING_Y*1.2); /* tick on input */
  }
  for (i = 0; i <= 100; i += 50){
    vol_x_pos = dstrect.x + (i/100.0) * dstrect.w; /* volume 0 */
    SDL_RenderDrawLine(renderer, vol_x_pos, VOLUME_SLIDER_HEIGHT - VOLUME_BAR_SPACING_Y_LARGE/2, vol_x_pos, VOLUME_SLIDER_HEIGHT + VOLUME_BAR_SPACING_Y_LARGE/2); /* tick on input */
  }
  render_text_absolute_c(renderer, font_10, "-", C_Text_Gray, VOLUME_BAR_SPACING_X*1.2, VOLUME_SLIDER_HEIGHT); /* draw - and + */
  render_text_absolute_c(renderer, font_10, "+", C_Text_Gray, display_w - VOLUME_BAR_SPACING_X*1.2, VOLUME_SLIDER_HEIGHT); /* draw - and + */

  SDL_SetRenderDrawColor(renderer, C_White.r, C_White.g, C_White.b, C_White.a); /* set new color for volume slider maeking box */
  dstrect.x = VOLUME_BAR_SPACING_X * 2 + output_vol*(display_w - VOLUME_BAR_SPACING_X*4) - VOLUME_SLIDER_MARKER_WIDTH / 2;
  dstrect.y = VOLUME_SLIDER_HEIGHT - VOLUME_BAR_SPACING_Y_LARGE / 2;
  dstrect.w = VOLUME_SLIDER_MARKER_WIDTH;
  dstrect.h = VOLUME_BAR_SPACING_Y_LARGE;
  SDL_RenderFillRect(renderer, &dstrect); /* draw volume slider */
  SDL_SetRenderDrawColor(renderer, C_Text_Gray.r, C_Text_Gray.g, C_Text_Gray.b, C_Text_Gray.a); /* draw slider otline */
  SDL_RenderDrawRect(renderer, &dstrect); /* draw volume slider */


  SDL_SetRenderDrawColor(renderer, or, og, ob, oa); /* restore old color */
  return 0;
}

int handle_volume_bar_event(SDL_Renderer* renderer, SDL_Event* e, audio_system* system){

  int input_on;
  int output_on;
  float output_vol;

  if (SDL_LockMutex(system->audio_system_mutex) == 0) { /* handle mutex for system */
    input_on = system->input_on;
    output_on = system->output_on;
    output_vol = system->output_vol;
    SDL_UnlockMutex(system->audio_system_mutex);
  }
  else { /* error message for when mutex breaks */
    printf("Unable to lock mutex: %s\n", SDL_GetError());
  }

  int display_w, display_h;
  SDL_GetRendererOutputSize(renderer, &display_w, &display_h); /* get display w and h */
  static int volume_drag;/* dragging volume bar */
  if (e->type == SDL_MOUSEBUTTONDOWN /* if clicing on right column for audio io on-off switch */
      && e->button.x >= VOLUME_BAR_SPACING_X
      && e->button.x <= VOLUME_BAR_SPACING_X + VOLUME_BAR_HEIGHT * 2 + VOLUME_BAR_SPACING_Y){
    if (e->button.y >= VOLUME_BAR_SPACING_Y_LARGE + UI_TOP_HEIGHT /* input on-off */
        && e->button.y <= VOLUME_BAR_SPACING_Y_LARGE + UI_TOP_HEIGHT + VOLUME_BAR_HEIGHT * 2 + VOLUME_BAR_SPACING_Y){
      if (input_on){
        turn_off_audio_input(system);
      }
      else {
        turn_on_audio_input(system);
      }
    }
    else if(e->button.y >= VOLUME_BAR_SPACING_Y_LARGE + UI_TOP_HEIGHT + VOLUME_BAR_HEIGHT * 2 + VOLUME_BAR_SPACING_Y + VOLUME_BAR_SPACING_Y_LARGE/* output on-off */
            && e->button.y <= VOLUME_BAR_SPACING_Y_LARGE + UI_TOP_HEIGHT + VOLUME_BAR_HEIGHT * 2 + VOLUME_BAR_SPACING_Y + VOLUME_BAR_SPACING_Y_LARGE + VOLUME_BAR_HEIGHT * 2 + VOLUME_BAR_SPACING_Y){
      if (output_on){
        turn_off_audio_output(system);
      }
      else {
        turn_on_audio_output(system);
      }
    }
  }
  if (e->type == SDL_MOUSEBUTTONDOWN /* if clicking on volume bar */
      && e->button.y >= VOLUME_SLIDER_HEIGHT - VOLUME_BAR_HEIGHT/2
      && e->button.y <= VOLUME_SLIDER_HEIGHT + VOLUME_BAR_HEIGHT/2){
    if (e->button.x >= output_vol*(display_w - VOLUME_BAR_SPACING_X*4) + VOLUME_BAR_SPACING_X * 2.0 - VOLUME_SLIDER_MARKER_WIDTH/1.5/* if within the volume bar */
        && e->button.x <= output_vol*(display_w - VOLUME_BAR_SPACING_X*4) + VOLUME_BAR_SPACING_X * 2.0 + VOLUME_SLIDER_MARKER_WIDTH/1.5){
      volume_drag = 1;
    }

    if (SDL_LockMutex(system->audio_system_mutex) == 0) { /* handle mutex for system */
      system->output_vol = clamp_value((e->button.x - VOLUME_BAR_SPACING_X * 2.0)/(display_w - VOLUME_BAR_SPACING_X*4));
      SDL_UnlockMutex(system->audio_system_mutex);
    }
    else { /* error message for when mutex breaks */
      printf("Unable to lock mutex: %s\n", SDL_GetError());
    }

  }
  if (e->type == SDL_MOUSEBUTTONUP){ /* if unclicking */
    volume_drag = 0;
  }
  if (!(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LMASK || SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_MMASK || SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_RMASK)) {
    volume_drag = 0;
  }
  if (e->type == SDL_MOUSEMOTION && volume_drag){ /* if dragging the volume bar */
    if (e->button.y >= VOLUME_SLIDER_HEIGHT - VOLUME_BAR_HEIGHT /* if within the volume bar + a bit of margin */
        && e->button.y <= VOLUME_SLIDER_HEIGHT + VOLUME_BAR_HEIGHT) {

          if (SDL_LockMutex(system->audio_system_mutex) == 0) { /* handle mutex for system */
            system->output_vol = clamp_value((e->button.x - VOLUME_BAR_SPACING_X * 2.0)/(display_w - VOLUME_BAR_SPACING_X*4));
            SDL_UnlockMutex(system->audio_system_mutex);
          }
          else { /* error message for when mutex breaks */
            printf("Unable to lock mutex: %s\n", SDL_GetError());
          }

    }
    else {
      volume_drag = 0;
    }
  }
  return 0;
}

int audio_rendering_cleanup(){
  if (volume_bar_texture != NULL){
    SDL_DestroyTexture(volume_bar_texture);
  }
  if (input_icon_texture != NULL){
    SDL_DestroyTexture(input_icon_texture);
  }
  if (output_icon_texture != NULL){
    SDL_DestroyTexture(output_icon_texture);
  }
  return 0;
}
