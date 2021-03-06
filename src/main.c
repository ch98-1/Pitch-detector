/* c standard libraries*/
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/* other libraries */
#include "kiss_fft.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

/* includes for defines */
#include "colors.h"
#include "fonts.h"
#include "states.h"
#include "constants.h"

/* includes for functions that does real work*/
#include "measure_frequency.h"
#include "select_audio_driver.h"
#include "select_audio_device.h"
#include "text_rendering.h"
#include "rendering_helper.h"
#include "user_interface.h"
#include "credits.h"
#include "audio_system.h"
#include "audio_rendering.h"
#include "convert_frequency.h"
#include "calculate_frequency.h"


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
    WINDOW_DEF_W,                               /* width, in pixels */
    WINDOW_DEF_H,                               /* height, in pixels */
    SDL_WINDOW_RESIZABLE|SDL_WINDOW_ALLOW_HIGHDPI
  );

  SDL_SetWindowMinimumSize(window, WINDOW_MIN_W, WINDOW_MIN_H);


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

  audio_system *system = init_audio();

  /* create thread for audio loop */
  SDL_Thread* audio_loop_thread = SDL_CreateThread(audio_loop, "audio_loop", (void*)system);

  /* create thread for frequency detection loop */
  SDL_Thread* frequency_loop_thread = SDL_CreateThread(frequency_loop, "frequency_loop", (void*)system);


  char font_dir[DIR_LENGTH];

  strcpy(font_dir, system->data_path);
  strcat(font_dir, "DejaVuSans.ttf");

  /* open fonts for text */
  Open_Fonts(font_dir);

  /* set common colors being used */
  set_colors();

  int displayfps = 1;

  Uint64 SDL_time_Hz = SDL_GetPerformanceFrequency();

  /* starting state */
  int program_state = SELECT_AUDIO_DEVICE;


  SDL_Event e;
  int quit = 0;
  int updatescreen = 0;
  Uint64 b = 0;
  double fps = 0;

  Uint64 curr = SDL_GetPerformanceCounter(); /* time for fps display */
  Uint64 last_t = SDL_GetPerformanceCounter(); /* time for minimum delay calculation */
  Uint64 last_update_t = SDL_GetPerformanceCounter(); /* time for minimul framerate calculation */

  if (SDL_LockMutex(system->system_quit_mutex) == 0) { /* handle mutex for quit */
    quit = system->quit; /* set quit to audio system's quit */
  SDL_UnlockMutex(system->system_quit_mutex);
  }
  else { /* error message for when mutex breaks */
    printf("Unable to lock mutex: %s\n", SDL_GetError());
  }


  /* main event loop */
  while (!quit){
    while (SDL_PollEvent(&e)){
      if (e.type == SDL_QUIT){
        if (SDL_LockMutex(system->system_quit_mutex) == 0) { /* handle mutex for quit */
          system->quit = 1; /*  quit system */
          SDL_UnlockMutex(system->system_quit_mutex);
        }
        else { /* error message for when mutex breaks */
          printf("Unable to lock mutex: %s\n", SDL_GetError());
        }
      }

      else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.y < UI_TOP_HEIGHT){
        int display_w, display_h;
        SDL_GetRendererOutputSize(renderer, &display_w, &display_h);
        handle_top_mouseclick(&program_state, &updatescreen, e.button.x/((float)display_w) );
      }
      else {
        switch (program_state) {
          case MEASURE_FREQUENCY: measure_frequency_events(&program_state, &updatescreen, renderer, &e, system);
              break;
          case SELECT_AUDIO_DRIVER: select_audio_driver_events(&program_state, &updatescreen, renderer, &e, system);
              break;
          case SELECT_AUDIO_DEVICE: select_audio_device_events(&program_state, &updatescreen, renderer, &e, system);
              break;
          case CREDITS: credit_events(&program_state, &updatescreen, &e);
              break;
          default:
              break;
        }
      }

    }


    switch (program_state) {
      case MEASURE_FREQUENCY: measure_frequency_process(&program_state, &updatescreen, system);
          break;
      case SELECT_AUDIO_DRIVER: select_audio_driver_process(&program_state, &updatescreen, system);
          break;
      case SELECT_AUDIO_DEVICE: select_audio_device_process(&program_state, &updatescreen, system);
          break;
      case CREDITS: credit_process(&program_state, &updatescreen, system);
          break;
      default:
          break;
    }

    /* attempt to keep minimum framerate */
    if ( (1.0 / MIN_FRAMERATE) > ((SDL_GetPerformanceCounter() - last_update_t)/(double)SDL_time_Hz)){ /* force screen update if its been too long */
      updatescreen = 1;
    }

    if (updatescreen){
      last_update_t = SDL_GetPerformanceCounter(); /* update last screen update time */

      SDL_SetRenderDrawColor(renderer, C_BG_Gray.r, C_BG_Gray.g, C_BG_Gray.b, C_BG_Gray.a);
      SDL_RenderClear(renderer);

      switch (program_state) {
        case MEASURE_FREQUENCY: measure_frequency_display(&program_state, renderer, system);
            break;
        case SELECT_AUDIO_DRIVER: select_audio_driver_display(&program_state, renderer, system);
            break;
        case SELECT_AUDIO_DEVICE: select_audio_device_display(&program_state, renderer, system);
            break;
        case CREDITS: credit_display(&program_state, renderer, system);
            break;
        default:
            break;
      }

      int display_w, display_h;
      SDL_GetRendererOutputSize(renderer, &display_w, &display_h);

      /* fps display generation code */
      if (displayfps) {
        char fps_text[64];
        sprintf(fps_text, "%6.1f fps", fps);
        render_text_relative_br(renderer, font_12, fps_text, C_Text_Gray, 1, 1);
      }

      draw_top_ui(&program_state, renderer); /* draw the top UI */

      SDL_RenderPresent(renderer);
      updatescreen = 0;
    }

    /* add delay if loop is going too fast */
    if ( MIN_LOOP_DELAY > ((SDL_GetPerformanceCounter() - last_t)/(double)SDL_time_Hz)) {
      SDL_Delay(1000*MIN_LOOP_DELAY);
    }
    last_t = SDL_GetPerformanceCounter();

    /* calculate fps every 30 loops */
    b++;
    if (b%30 == 0){
      fps = 30/((double)((SDL_GetPerformanceCounter() - curr)/(double)SDL_time_Hz));
      curr = SDL_GetPerformanceCounter();
    }

    if (SDL_LockMutex(system->system_quit_mutex) == 0) { /* handle mutex for quit */
      quit = system->quit; /* set quit to audio system's quit */
    SDL_UnlockMutex(system->system_quit_mutex);
    }
    else { /* error message for when mutex breaks */
      printf("Unable to lock mutex: %s\n", SDL_GetError());
    }

  }

  int audio_loop_thread_code; /* thread return values */
  int frequency_loop_thread_code;

  SDL_WaitThread(audio_loop_thread,  &audio_loop_thread_code);/* wait for audio loop to end */
  SDL_WaitThread(frequency_loop_thread,  &frequency_loop_thread_code);/* wait for frequency detection loop to end */



  clean_audio(system);

  /* clean up texture for credits */
  credit_cleanup();

  /* clean up textures for audio renderings */
  audio_rendering_cleanup();

  /* close fonts for text */
  Close_Fonts();

  SDL_DestroyRenderer(renderer); /* destroy main renderer */
  SDL_DestroyWindow(window); /* destroy main window */


  TTF_Quit(); /* quit ttf */
  SDL_Quit(); /* quit everything */
  exit(EXIT_SUCCESS);
}
