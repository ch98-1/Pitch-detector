#ifndef FONTS_H
#define FONTS_H

#include "SDL.h"
#include "SDL_ttf.h"

/* load fonts to be used */
int Open_Fonts(const char *font_dir);

TTF_Font *font_10;
TTF_Font *font_12;
TTF_Font *font_14;
TTF_Font *font_18;
TTF_Font *font_24;
TTF_Font *font_30;
TTF_Font *font_36;
TTF_Font *font_48;
TTF_Font *font_60;

#endif /* FONTS_H */
