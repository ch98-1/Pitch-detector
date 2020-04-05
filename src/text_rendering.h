#ifndef TEXT_RENDERING_H
#define TEXT_RENDERING_H

#include <stdio.h>
#include <math.h>
#include <string.h>

#include "SDL.h"
#include "SDL_ttf.h"

#include "rendering_helper.h"

/* create texture for text */
SDL_Texture* get_text_texture(SDL_Renderer* renderer, TTF_Font *font, const char *text, SDL_Color color);


/* render with coordinates on display */

/* render utf8 text with absolute coordinate on top left */
int render_text_absolute_tl(SDL_Renderer* renderer, TTF_Font *font, const char *text, SDL_Color color, int x, int y);

/* render utf8 text with absolute coordinate on top right */
int render_text_absolute_tr(SDL_Renderer* renderer, TTF_Font *font, const char *text, SDL_Color color, int x, int y);

/* render utf8 text with absolute coordinate on bottom left */
int render_text_absolute_bl(SDL_Renderer* renderer, TTF_Font *font, const char *text, SDL_Color color, int x, int y);

/* render utf8 text with absolute coordinate on bottom right */
int render_text_absolute_br(SDL_Renderer* renderer, TTF_Font *font, const char *text, SDL_Color color, int x, int y);

/* render utf8 text with absolute coordinate on center */
int render_text_absolute_c(SDL_Renderer* renderer, TTF_Font *font, const char *text, SDL_Color color, int x, int y);


/* render with coordinates in float from 0 to 1 */

/* render utf8 text with relative coordinate on top left */
int render_text_relative_tl(SDL_Renderer* renderer, TTF_Font *font, const char *text, SDL_Color color, float x, float y);

/* render utf8 text with relative coordinate on top right */
int render_text_relative_tr(SDL_Renderer* renderer, TTF_Font *font, const char *text, SDL_Color color, float x, float y);

/* render utf8 text with relative coordinate on bottom left */
int render_text_relative_bl(SDL_Renderer* renderer, TTF_Font *font, const char *text, SDL_Color color, float x, float y);

/* render utf8 text with relative coordinate on bottom right */
int render_text_relative_br(SDL_Renderer* renderer, TTF_Font *font, const char *text, SDL_Color color, float x, float y);

/* render utf8 text with relative coordinate on center */
int render_text_relative_c(SDL_Renderer* renderer, TTF_Font *font, const char *text, SDL_Color color, float x, float y);




/* render with filled background color */
/* render with coordinates on display */

/* render utf8 text with absolute coordinate on top left */
int render_text_absolute_fb_tl(SDL_Renderer* renderer, TTF_Font *font, const char *text, SDL_Color color, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* render utf8 text with absolute coordinate on top right */
int render_text_absolute_fb_tr(SDL_Renderer* renderer, TTF_Font *font, const char *text, SDL_Color color, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* render utf8 text with absolute coordinate on bottom left */
int render_text_absolute_fb_bl(SDL_Renderer* renderer, TTF_Font *font, const char *text, SDL_Color color, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* render utf8 text with absolute coordinate on bottom right */
int render_text_absolute_fb_br(SDL_Renderer* renderer, TTF_Font *font, const char *text, SDL_Color color, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* render utf8 text with absolute coordinate on center */
int render_text_absolute_fb_c(SDL_Renderer* renderer, TTF_Font *font, const char *text, SDL_Color color, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);


/* render with coordinates in float from 0 to 1 */

/* render utf8 text with relative coordinate on top left */
int render_text_relative_fb_tl(SDL_Renderer* renderer, TTF_Font *font, const char *text, SDL_Color color, float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* render utf8 text with relative coordinate on top right */
int render_text_relative_fb_tr(SDL_Renderer* renderer, TTF_Font *font, const char *text, SDL_Color color, float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* render utf8 text with relative coordinate on bottom left */
int render_text_relative_fb_bl(SDL_Renderer* renderer, TTF_Font *font, const char *text, SDL_Color color, float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* render utf8 text with relative coordinate on bottom right */
int render_text_relative_fb_br(SDL_Renderer* renderer, TTF_Font *font, const char *text, SDL_Color color, float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* render utf8 text with relative coordinate on center */
int render_text_relative_fb_c(SDL_Renderer* renderer, TTF_Font *font, const char *text, SDL_Color color, float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);



/* render with outlined background */
/* render with coordinates on display */

/* render utf8 text with absolute coordinate on top left */
int render_text_absolute_ob_tl(SDL_Renderer* renderer, TTF_Font *font, const char *text, SDL_Color color, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* render utf8 text with absolute coordinate on top right */
int render_text_absolute_ob_tr(SDL_Renderer* renderer, TTF_Font *font, const char *text, SDL_Color color, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* render utf8 text with absolute coordinate on bottom left */
int render_text_absolute_ob_bl(SDL_Renderer* renderer, TTF_Font *font, const char *text, SDL_Color color, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* render utf8 text with absolute coordinate on bottom right */
int render_text_absolute_ob_br(SDL_Renderer* renderer, TTF_Font *font, const char *text, SDL_Color color, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* render utf8 text with absolute coordinate on center */
int render_text_absolute_ob_c(SDL_Renderer* renderer, TTF_Font *font, const char *text, SDL_Color color, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);


/* render with coordinates in float from 0 to 1 */

/* render utf8 text with relative coordinate on top left */
int render_text_relative_ob_tl(SDL_Renderer* renderer, TTF_Font *font, const char *text, SDL_Color color, float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* render utf8 text with relative coordinate on top right */
int render_text_relative_ob_tr(SDL_Renderer* renderer, TTF_Font *font, const char *text, SDL_Color color, float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* render utf8 text with relative coordinate on bottom left */
int render_text_relative_ob_bl(SDL_Renderer* renderer, TTF_Font *font, const char *text, SDL_Color color, float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* render utf8 text with relative coordinate on bottom right */
int render_text_relative_ob_br(SDL_Renderer* renderer, TTF_Font *font, const char *text, SDL_Color color, float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* render utf8 text with relative coordinate on center */
int render_text_relative_ob_c(SDL_Renderer* renderer, TTF_Font *font, const char *text, SDL_Color color, float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

#endif /* TEXT_RENDERING_H */
