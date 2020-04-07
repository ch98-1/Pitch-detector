#include "audio_rendering.h"

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


  if (system->input_on){ /* choose color depending on input status */
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


  if (system->output_on){ /* choose color depending on output status */
    SDL_SetRenderDrawColor(renderer, C_UIL_Gray.r, C_UIL_Gray.g, C_UIL_Gray.b, C_UIL_Gray.a);
  }
  else {
    SDL_SetRenderDrawColor(renderer, C_UI_Gray.r, C_UI_Gray.g, C_UI_Gray.b, C_UI_Gray.a);
  }

  dstrect.y = y + dstrect.w + VOLUME_BAR_SPACING_Y_LARGE; /* output volume icon */
  SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */
  SDL_RenderCopy(renderer, output_icon_texture, NULL, &dstrect);

  dstrect.x = volume_bar_start_x; /* left input */
  dstrect.y = y;
  dstrect.w = volume_bar_width;
  dstrect.h = VOLUME_BAR_HEIGHT;
  render_text_absolute_tl(renderer, font_10, "L", C_Text_Gray, volume_bar_start_x - VOLUME_BAR_SPACING_X, dstrect.y );
  SDL_RenderCopy(renderer, volume_bar_texture, NULL, &dstrect);
  dstrect.x = volume_bar_start_x + get_db_limited_normalized(system->in_l_vol) * volume_bar_width; /* left input volume bar */
  dstrect.w = volume_bar_width * (get_db_limited_normalized(system->in_l_vol_max) - get_db_limited_normalized(system->in_l_vol));
  SDL_SetRenderDrawColor(renderer, C_UIL_Gray.r, C_UIL_Gray.g, C_UIL_Gray.b, C_UIL_Gray.a); /* set new color */
  SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */
  dstrect.x = volume_bar_start_x + volume_bar_width * get_db_limited_normalized(system->in_l_vol_max) + 1; /* left input volume bar max */
  dstrect.w = volume_bar_width - volume_bar_width * get_db_limited_normalized(system->in_l_vol_max);
  SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */

  dstrect.x = volume_bar_start_x; /* right input */
  dstrect.y = y + VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y;
  dstrect.w = volume_bar_width;
  dstrect.h = VOLUME_BAR_HEIGHT;
  render_text_absolute_tl(renderer, font_10, "R", C_Text_Gray, volume_bar_start_x - VOLUME_BAR_SPACING_X, dstrect.y );
  SDL_RenderCopy(renderer, volume_bar_texture, NULL, &dstrect);
  dstrect.x = volume_bar_start_x + get_db_limited_normalized(system->in_r_vol) * volume_bar_width; /* left input volume bar */
  dstrect.w = volume_bar_width * (get_db_limited_normalized(system->in_r_vol_max) - get_db_limited_normalized(system->in_r_vol));
  SDL_SetRenderDrawColor(renderer, C_UIL_Gray.r, C_UIL_Gray.g, C_UIL_Gray.b, C_UIL_Gray.a); /* set new color */
  SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */
  dstrect.x = volume_bar_start_x + volume_bar_width * get_db_limited_normalized(system->in_r_vol_max) + 1; /* left input volume bar max */
  dstrect.w = volume_bar_width - volume_bar_width * get_db_limited_normalized(system->in_r_vol_max);
  SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */


  dstrect.x = volume_bar_start_x; /* left output */
  dstrect.y = y + VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y + VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y_LARGE;
  dstrect.w = volume_bar_width;
  dstrect.h = VOLUME_BAR_HEIGHT;
  render_text_absolute_tl(renderer, font_10, "L", C_Text_Gray, volume_bar_start_x - VOLUME_BAR_SPACING_X, dstrect.y );
  SDL_RenderCopy(renderer, volume_bar_texture, NULL, &dstrect);
  dstrect.x = volume_bar_start_x + get_db_limited_normalized(system->out_l_vol) * volume_bar_width; /* left input volume bar */
  dstrect.w = volume_bar_width * (get_db_limited_normalized(system->out_l_vol_max) - get_db_limited_normalized(system->out_l_vol));
  SDL_SetRenderDrawColor(renderer, C_UIL_Gray.r, C_UIL_Gray.g, C_UIL_Gray.b, C_UIL_Gray.a); /* set new color */
  SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */
  dstrect.x = volume_bar_start_x + volume_bar_width * get_db_limited_normalized(system->out_l_vol_max) + 1; /* left input volume bar max */
  dstrect.w = volume_bar_width - volume_bar_width * get_db_limited_normalized(system->out_l_vol_max);
  SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */

  dstrect.x = volume_bar_start_x; /* right output */
  dstrect.y = y + VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y + VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y_LARGE + VOLUME_BAR_HEIGHT + VOLUME_BAR_SPACING_Y;
  dstrect.w = volume_bar_width;
  dstrect.h = VOLUME_BAR_HEIGHT;
  render_text_absolute_tl(renderer, font_10, "R", C_Text_Gray, volume_bar_start_x - VOLUME_BAR_SPACING_X, dstrect.y );
  SDL_RenderCopy(renderer, volume_bar_texture, NULL, &dstrect);
  dstrect.x = volume_bar_start_x + get_db_limited_normalized(system->out_r_vol) * volume_bar_width; /* left input volume bar */
  dstrect.w = volume_bar_width * (get_db_limited_normalized(system->out_r_vol_max) - get_db_limited_normalized(system->out_r_vol));
  SDL_SetRenderDrawColor(renderer, C_UIL_Gray.r, C_UIL_Gray.g, C_UIL_Gray.b, C_UIL_Gray.a); /* set new color */
  SDL_RenderFillRect(renderer, &dstrect); /* draw background rectangle */
  dstrect.x = volume_bar_start_x + volume_bar_width * get_db_limited_normalized(system->out_r_vol_max) + 1; /* left input volume bar max */
  dstrect.w = volume_bar_width - volume_bar_width * get_db_limited_normalized(system->out_r_vol_max);
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

  SDL_SetRenderDrawColor(renderer, or, og, ob, oa); /* restore old color */
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
