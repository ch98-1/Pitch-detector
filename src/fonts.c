#include "fonts.h"

TTF_Font *font_10;
TTF_Font *font_12;
TTF_Font *font_14;
TTF_Font *font_18;
TTF_Font *font_24;
TTF_Font *font_30;
TTF_Font *font_36;
TTF_Font *font_48;
TTF_Font *font_60;

/* load fonts to be used */
int Open_Fonts(const char *font_dir){
  font_10 = TTF_OpenFont( font_dir, 10 );
  if(!font_10) {
    printf("TTF_OpenFont: %s\n", TTF_GetError());
  }
  font_12 = TTF_OpenFont( font_dir, 12 );
  if(!font_12) {
    printf("TTF_OpenFont: %s\n", TTF_GetError());
  }
  font_14 = TTF_OpenFont( font_dir, 14 );
  if(!font_14) {
    printf("TTF_OpenFont: %s\n", TTF_GetError());
  }
  font_18 = TTF_OpenFont( font_dir, 18 );
  if(!font_18) {
    printf("TTF_OpenFont: %s\n", TTF_GetError());
  }
  font_24 = TTF_OpenFont( font_dir, 24 );
  if(!font_24) {
    printf("TTF_OpenFont: %s\n", TTF_GetError());
  }
  font_30 = TTF_OpenFont( font_dir, 30 );
  if(!font_30) {
    printf("TTF_OpenFont: %s\n", TTF_GetError());
  }
  font_36 = TTF_OpenFont( font_dir, 36 );
  if(!font_36) {
    printf("TTF_OpenFont: %s\n", TTF_GetError());
  }
  font_48 = TTF_OpenFont( font_dir, 48 );
  if(!font_48) {
    printf("TTF_OpenFont: %s\n", TTF_GetError());
  }
  font_60 = TTF_OpenFont( font_dir, 60 );
  if(!font_60) {
    printf("TTF_OpenFont: %s\n", TTF_GetError());
  }

  return 0;
}

/* close all fonts opened */
int Close_Fonts(){
  TTF_CloseFont(font_10);
  TTF_CloseFont(font_12);
  TTF_CloseFont(font_14);
  TTF_CloseFont(font_18);
  TTF_CloseFont(font_24);
  TTF_CloseFont(font_30);
  TTF_CloseFont(font_36);
  TTF_CloseFont(font_48);
  TTF_CloseFont(font_60);
  return 0;
}
