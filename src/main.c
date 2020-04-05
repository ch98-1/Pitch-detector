/* c standard libraries*/
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

/* other libraries */
#include "kiss_fft.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

/* includes for defines */
#include "colors.h"
#include "fonts.h"
#include "states.h"

/* includes for functions that does real work*/
#include "select_audio_driver.h"
#include "measure_frequency.h"
#include "text_rendering.h"
#include "rendering_helper.h"


int main(int argc, char* argv[])
{
  if (SDL_Init(SDL_INIT_TIMER|SDL_INIT_AUDIO|SDL_INIT_VIDEO|SDL_INIT_EVENTS)) { /* initialise sdl */
    printf("Unable to initialize SDL: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  SDL_Window *window = NULL; /* main window pointer */
  SDL_Renderer *renderer = NULL; /* main renderer pointer */

  window = SDL_CreateWindow( /* create main window */
    "Pitch Detector",                  /* window title */
    SDL_WINDOWPOS_UNDEFINED,           /* initial x position */
    SDL_WINDOWPOS_UNDEFINED,           /* initial y position */
    640,                               /* width, in pixels */
    480,                               /* height, in pixels */
    SDL_WINDOW_RESIZABLE|SDL_WINDOW_ALLOW_HIGHDPI
  );

  if (window == NULL) { /* check for success */
      printf("Could not create window: %s\n", SDL_GetError());
      exit(EXIT_FAILURE);
  }

  renderer = SDL_CreateRenderer(window, -1, 0);
  if (renderer == NULL) { /* check for success */
      printf("Could not create renderer: %s\n", SDL_GetError());
      exit(EXIT_FAILURE);
  }

  if(TTF_Init()==-1) {
    printf("TTF_Init: %s\n", TTF_GetError());
    exit(EXIT_FAILURE);
  }


  char font_dir[2048];

  /* get resources base path */
  char *data_path = SDL_GetBasePath();
  if (!data_path) {
      data_path = SDL_strdup("./");
  }

  strcpy(font_dir, data_path);
  strcat(font_dir, "DejaVuSans.ttf");

  /* open fonts for text */
  Open_Fonts(font_dir);

  int displayfps = 1;
  SDL_Surface *fps_text_surface = NULL;
  SDL_Texture *fps_text_texture = NULL;

  Uint64 SDL_time_Hz = SDL_GetPerformanceFrequency();

  /* starting state */
  int program_state = select_audio_driver;
  int next_program_state = program_state;


  SDL_Event e;
  int quit = 0;
  int updatescreen = 0;
  Uint64 b = 0;
  double fps = 0;
  Uint64 curr = SDL_GetPerformanceCounter();

  /* main event loop */
  while (!quit){
    while (SDL_PollEvent(&e)){
      if (e.type == SDL_QUIT){
          quit = 1;
      }

      /*if (e.type == SDL_KEYDOWN){
          printf("fps=%lf\n", fps);
          updatescreen = 1;
      }
      if (e.type == SDL_MOUSEBUTTONDOWN){
          quit = 1;
      }*/

      else {
        switch (program_state) {
            case select_audio_driver: select_audio_driver_events(&program_state, &next_program_state, &updatescreen, &e);
                break;
            case measure_frequency: measure_frequency_events(&program_state, &next_program_state, &updatescreen, &e);
                break;
            default:
                break;
        }
      }

    }

    switch (program_state) {
        case select_audio_driver: select_audio_driver_process(&program_state, &next_program_state, &updatescreen);
            break;
        case measure_frequency: measure_frequency_process(&program_state, &next_program_state, &updatescreen);
            break;
        default:
            break;
    }



    if (updatescreen){
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      SDL_RenderClear(renderer);


      switch (program_state) {
          case select_audio_driver: select_audio_driver_display(&program_state, &next_program_state, renderer);
              break;
          case measure_frequency: measure_frequency_display(&program_state, &next_program_state, renderer);
              break;
          default:
              break;
      }

      int display_w, display_h;
      SDL_GetRendererOutputSize(renderer, &display_w, &display_h);

      /* fps display generation code */
      if (displayfps) {
        char fps_text[50];
        sprintf(fps_text, "%6.1f fps", fps);
        render_text_relative_tr(renderer, font_12, fps_text, C_Black, 1, 0);
      }


      SDL_RenderPresent(renderer);
      updatescreen = 0;
    }

    program_state = next_program_state; /* update to next program state */

    /* calculate fps every 30 frames */
    b++;
    if (b%30 == 0){
      fps = 30/((double)((SDL_GetPerformanceCounter() - curr)/(double)SDL_time_Hz));
      curr = SDL_GetPerformanceCounter();
    }
  }

  /* close fonts for text */
  TTF_CloseFont(font_12);

  SDL_free(data_path);

  SDL_DestroyRenderer(renderer); /* destroy main renderer */
  SDL_DestroyWindow(window); /* destroy main window */


  TTF_Quit(); /* quit ttf */
  SDL_Quit(); /* quit everything */
  exit(EXIT_SUCCESS);
}
