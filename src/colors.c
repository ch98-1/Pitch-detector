#include "colors.h"

/* common colors */
SDL_Color C_Black; /* Black */

SDL_Color C_BG_Gray; /* Background gray */

SDL_Color C_UIL_Gray; /* UI light gray */

SDL_Color C_UI_Gray; /* UI gray */

SDL_Color C_UID_Gray; /* UI dark gray */

SDL_Color C_Text_Gray; /* Text gray */

SDL_Color C_White; /* White */

int set_colors(){
  /* set common colors */

  C_Black.r = 0; /* Black */
  C_Black.g = 0;
  C_Black.b = 0;
  C_Black.a = 255;

  C_BG_Gray.r = 80; /* Background gray */
  C_BG_Gray.g = 80;
  C_BG_Gray.b = 80;
  C_BG_Gray.a = 80;

  C_UIL_Gray.r = 70; /* UI light gray */
  C_UIL_Gray.g = 70;
  C_UIL_Gray.b = 70;
  C_UIL_Gray.a = 255;

  C_UI_Gray.r = 50; /* UI gray */
  C_UI_Gray.g = 50;
  C_UI_Gray.b = 50;
  C_UI_Gray.a = 255;

  C_UID_Gray.r = 20; /* UI dark gray */
  C_UID_Gray.g = 20;
  C_UID_Gray.b = 20;
  C_UID_Gray.a = 255;

  C_Text_Gray.r = 180; /* Text gray */
  C_Text_Gray.g = 180;
  C_Text_Gray.b = 180;
  C_Text_Gray.a = 255;

  C_White.r = 255; /* White */
  C_White.g = 255;
  C_White.b = 255;
  C_White.a = 255;

  return 0;
}
