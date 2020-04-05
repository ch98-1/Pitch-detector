#ifndef RENDERING_HELPER_H
#define RENDERING_HELPER_H

#include <stdio.h>

#include "SDL.h"


/* render copy with coordinates on display */

/* render texture with absolute coordinate on top left */
int render_copy_absolute_tl(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y);

/* render texture with absolute coordinate on top right */
int render_copy_absolute_tr(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y);

/* render texture with absolute coordinate on bottom left */
int render_copy_absolute_bl(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y);

/* render texture with absolute coordinate on bottom right */
int render_copy_absolute_br(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y);

/* render texture with absolute coordinate on center */
int render_copy_absolute_c(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y);


/* render copy with coordinates in float from 0 to 1 */

/* render texture with relative coordinate on top left */
int render_copy_relative_tl(SDL_Renderer* renderer, SDL_Texture* texture, float x, float y);

/* render texture with relative coordinate on top right */
int render_copy_relative_tr(SDL_Renderer* renderer, SDL_Texture* texture, float x, float y);

/* render texture with relative coordinate on bottom left */
int render_copy_relative_bl(SDL_Renderer* renderer, SDL_Texture* texture, float x, float y);

/* render texture with relative coordinate on bottom right */
int render_copy_relative_br(SDL_Renderer* renderer, SDL_Texture* texture, float x, float y);

/* render texture with relative coordinate on center */
int render_copy_relative_c(SDL_Renderer* renderer, SDL_Texture* texture, float x, float y);




/* render copy with filled background color */
/* render copy with coordinates on display */

/* render texture with absolute coordinate on top left */
int render_copy_absolute_fb_tl(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* render texture with absolute coordinate on top right */
int render_copy_absolute_fb_tr(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* render texture with absolute coordinate on bottom left */
int render_copy_absolute_fb_bl(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* render texture with absolute coordinate on bottom right */
int render_copy_absolute_fb_br(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* render texture with absolute coordinate on center */
int render_copy_absolute_fb_c(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);


/* render copy with coordinates in float from 0 to 1 */

/* render texture with relative coordinate on top left */
int render_copy_relative_fb_tl(SDL_Renderer* renderer, SDL_Texture* texture, float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* render texture with relative coordinate on top right */
int render_copy_relative_fb_tr(SDL_Renderer* renderer, SDL_Texture* texture, float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* render texture with relative coordinate on bottom left */
int render_copy_relative_fb_bl(SDL_Renderer* renderer, SDL_Texture* texture, float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* render texture with relative coordinate on bottom right */
int render_copy_relative_fb_br(SDL_Renderer* renderer, SDL_Texture* texture, float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* render texture with relative coordinate on center */
int render_copy_relative_fb_c(SDL_Renderer* renderer, SDL_Texture* texture, float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);



/* render copy with outlined background */
/* render copy with coordinates on display */

/* render texture with absolute coordinate on top left */
int render_copy_absolute_ob_tl(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* render texture with absolute coordinate on top right */
int render_copy_absolute_ob_tr(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* render texture with absolute coordinate on bottom left */
int render_copy_absolute_ob_bl(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* render texture with absolute coordinate on bottom right */
int render_copy_absolute_ob_br(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* render texture with absolute coordinate on center */
int render_copy_absolute_ob_c(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);


/* render copy with coordinates in float from 0 to 1 */

/* render texture with relative coordinate on top left */
int render_copy_relative_ob_tl(SDL_Renderer* renderer, SDL_Texture* texture, float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* render texture with relative coordinate on top right */
int render_copy_relative_ob_tr(SDL_Renderer* renderer, SDL_Texture* texture, float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* render texture with relative coordinate on bottom left */
int render_copy_relative_ob_bl(SDL_Renderer* renderer, SDL_Texture* texture, float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* render texture with relative coordinate on bottom right */
int render_copy_relative_ob_br(SDL_Renderer* renderer, SDL_Texture* texture, float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

/* render texture with relative coordinate on center */
int render_copy_relative_ob_c(SDL_Renderer* renderer, SDL_Texture* texture, float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

#endif /* RENDERING_HELPER_H */
