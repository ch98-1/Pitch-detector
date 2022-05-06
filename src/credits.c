#include "credits.h"

SDL_Texture* credits_texture;

int credit_events(int* program_state, int* updatescreen, SDL_Event* e){
  *updatescreen = 1;
  return  0;
}

int credit_process(int* program_state, int* updatescreen, audio_system* system){

  if (SDL_LockMutex(system->audio_system_mutex) == 0) { /* handle mutex for system */
    system->play_tone_end = 0; /* not playing any tone */
    system->input_monitor = 0; /* don't monitor input */
    system->measure_frequency_value = 0; /* don't measure frequency */
    SDL_UnlockMutex(system->audio_system_mutex);
  }
  else { /* error message for when mutex breaks */
    printf("Unable to lock mutex: %s\n", SDL_GetError());
  }

  return  0;
}

int credit_display(int* program_state, SDL_Renderer* renderer, audio_system* system){
  if (!credits_texture){ /* load credit texture if empty */
    char credits_image_dir[DIR_LENGTH];
    strcpy(credits_image_dir, system->data_path);
    strcat(credits_image_dir, "Credits.png");
    credits_texture = get_image_texture(renderer, credits_image_dir);
  }
  render_copy_relative_c(renderer, credits_texture, 0.5, 0.5);
  return 0;
}

int credit_cleanup(){ /* clean up and free stuff used by credits screen */
  if (credits_texture != NULL){
    SDL_DestroyTexture(credits_texture);
  }
  return 0;
}
