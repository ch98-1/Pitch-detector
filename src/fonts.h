#ifndef FONTS_H
#define FONTS_H

#include "SDL.h"
#include "SDL_ttf.h"

/* load fonts to be used */
int Open_Fonts(const char *font_dir);

/* close all fonts opened */
int Close_Fonts();

extern TTF_Font *font_10;
extern TTF_Font *font_12;
extern TTF_Font *font_14;
extern TTF_Font *font_18;
extern TTF_Font *font_24;
extern TTF_Font *font_30;
extern TTF_Font *font_36;
extern TTF_Font *font_48;
extern TTF_Font *font_60;

#endif /* FONTS_H */
