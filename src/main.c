#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <string.h>


#include "kiss_fft.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"


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
  TTF_Font *font_12 = TTF_OpenFont( font_dir, 12 );


  /* common sdl colors (r, g, b, a) */
  SDL_Color C_Black = { 0, 0, 0, 255 }; /* Black */ 

  int displayfps = 1;
  SDL_Surface *fps_text_surface = NULL;
  SDL_Texture *fps_text_texture = NULL;

  Uint64 SDL_time_Hz = SDL_GetPerformanceFrequency();

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
      }

      updatescreen = 1;

      if (updatescreen){
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        int display_w, display_h;
        SDL_GetRendererOutputSize(renderer, &display_w, &display_h);


        /* fps display generation code */
        if (displayfps) {
          char fps_text[50];
          sprintf(fps_text, "%6.1f fps", fps);
          fps_text_surface = TTF_RenderUTF8_Blended( font_12, fps_text, C_Black );
          fps_text_texture = SDL_CreateTextureFromSurface( renderer, fps_text_surface );
          SDL_FreeSurface(fps_text_surface);

          int fps_text_texture_w, fps_text_texture_h;
          SDL_QueryTexture(fps_text_texture, NULL, NULL, &fps_text_texture_w, &fps_text_texture_h);/* get texture size*/

          SDL_Rect dstrect;/* set output rectangle */
          dstrect.x = display_w - fps_text_texture_w;
          dstrect.y = 0;
          dstrect.w = fps_text_texture_w;
          dstrect.h = fps_text_texture_h;

          SDL_RenderCopy(renderer, fps_text_texture, NULL, &dstrect);

          SDL_DestroyTexture(fps_text_texture);
        }


        SDL_RenderPresent(renderer);
        updatescreen = 0;
      }

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
