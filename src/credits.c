#include "credits.h"


int credit_events(int* program_state, int* updatescreen, SDL_Event* e){
  *updatescreen = 1;
  return  0;
}

int credit_process(int* program_state, int* updatescreen){
  return  0;
}

int credit_display(int* program_state, SDL_Renderer* renderer, const char *data_path){
  if (!credits_texture){ /* load credit texture if empty */
    char credits_image_dir[DIR_LENGTH];
    strcpy(credits_image_dir, data_path);
    strcat(credits_image_dir, "Credits.png");
    credits_texture = get_image_texture(renderer, credits_image_dir);
  }
  render_copy_relative_c(renderer, credits_texture, 0.5, 0.5);
  return 0;
}

int credit_cleanup(){ /* clean up and free stuff used by credits screen */
  if (credits_texture){
    SDL_DestroyTexture(credits_texture);
  }
  return 0;
}
