#include "rendering_helper.h"


/* create texture from image file */
SDL_Texture* get_image_texture(SDL_Renderer* renderer, const char *file){
  SDL_Surface* surface = IMG_Load(file); /* create surface */
  SDL_Texture* texture = SDL_CreateTextureFromSurface( renderer, surface ); /* convert to texture */
  SDL_FreeSurface(surface); /* free surface */
  return texture;
}



/* render copy with coordinates on display */

/* render texture with absolute coordinate on top left */
int render_copy_absolute_tl(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y){
  int w, h;
  SDL_QueryTexture(texture, NULL, NULL, &w, &h);/* get texture size*/

  SDL_Rect dstrect;/* set output rectangle */
  dstrect.x = x;
  dstrect.y = y;
  dstrect.w = w;
  dstrect.h = h;

  SDL_RenderCopy(renderer, texture, NULL, &dstrect);
  return 0;
}

/* render texture with absolute coordinate on top right */
int render_copy_absolute_tr(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y){
  int w, h;
  SDL_QueryTexture(texture, NULL, NULL, &w, &h);/* get texture size*/

  SDL_Rect dstrect;/* set output rectangle */
  dstrect.x = x - w;
  dstrect.y = y;
  dstrect.w = w;
  dstrect.h = h;

  SDL_RenderCopy(renderer, texture, NULL, &dstrect);
  return 0;
}

/* render texture with absolute coordinate on bottom left */
int render_copy_absolute_bl(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y){
  int w, h;
  SDL_QueryTexture(texture, NULL, NULL, &w, &h);/* get texture size*/

  SDL_Rect dstrect;/* set output rectangle */
  dstrect.x = x;
  dstrect.y = y - h;
  dstrect.w = w;
  dstrect.h = h;

  SDL_RenderCopy(renderer, texture, NULL, &dstrect);
  return 0;
}

/* render texture with absolute coordinate on bottom right */
int render_copy_absolute_br(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y){
  int w, h;
  SDL_QueryTexture(texture, NULL, NULL, &w, &h);/* get texture size*/

  SDL_Rect dstrect;/* set output rectangle */
  dstrect.x = x - w;
  dstrect.y = y - h;
  dstrect.w = w;
  dstrect.h = h;

  SDL_RenderCopy(renderer, texture, NULL, &dstrect);
  return 0;
}

/* render texture with absolute coordinate on center */
int render_copy_absolute_c(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y){
  int w, h;
  SDL_QueryTexture(texture, NULL, NULL, &w, &h);/* get texture size*/

  SDL_Rect dstrect;/* set output rectangle */
  dstrect.x = x - w/2;
  dstrect.y = y - h/2;
  dstrect.w = w;
  dstrect.h = h;

  SDL_RenderCopy(renderer, texture, NULL, &dstrect);
  return 0;
}


/* render copy with coordinates in float from 0 to 1 */

/* render texture with relative coordinate on top left */
int render_copy_relative_tl(SDL_Renderer* renderer, SDL_Texture* texture, float x, float y){
  int display_w, display_h;
  SDL_GetRendererOutputSize(renderer, &display_w, &display_h);
  int ax = x*display_w; /* calculate absolute coordinates */
  int ay = y*display_h;

  /* call absolute render copy */
  render_copy_absolute_tl(renderer,texture, ax, ay);
  return 0;
}

/* render texture with relative coordinate on top right */
int render_copy_relative_tr(SDL_Renderer* renderer, SDL_Texture* texture, float x, float y){
  int display_w, display_h;
  SDL_GetRendererOutputSize(renderer, &display_w, &display_h);
  int ax = x*display_w; /* calculate absolute coordinates */
  int ay = y*display_h;

  /* call absolute render copy */
  render_copy_absolute_tr(renderer,texture, ax, ay);
  return 0;
}

/* render texture with relative coordinate on bottom left */
int render_copy_relative_bl(SDL_Renderer* renderer, SDL_Texture* texture, float x, float y){
  int display_w, display_h;
  SDL_GetRendererOutputSize(renderer, &display_w, &display_h);
  int ax = x*display_w; /* calculate absolute coordinates */
  int ay = y*display_h;

  /* call absolute render copy */
  render_copy_absolute_bl(renderer,texture, ax, ay);
  return 0;
}

/* render texture with relative coordinate on bottom right */
int render_copy_relative_br(SDL_Renderer* renderer, SDL_Texture* texture, float x, float y){
  int display_w, display_h;
  SDL_GetRendererOutputSize(renderer, &display_w, &display_h);
  int ax = x*display_w; /* calculate absolute coordinates */
  int ay = y*display_h;

  /* call absolute render copy */
  render_copy_absolute_br(renderer,texture, ax, ay);
  return 0;
}

/* render texture with relative coordinate on center */
int render_copy_relative_c(SDL_Renderer* renderer, SDL_Texture* texture, float x, float y){
  int display_w, display_h;
  SDL_GetRendererOutputSize(renderer, &display_w, &display_h);
  int ax = x*display_w; /* calculate absolute coordinates */
  int ay = y*display_h;

  /* call absolute render copy */
  render_copy_absolute_c(renderer, texture, ax, ay);
  return 0;
}




/* render copy with filled background color */
/* render copy with coordinates on display */

/* render texture with absolute coordinate on top left */
int render_copy_absolute_fb_tl(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a){
  int w, h;
  SDL_QueryTexture(texture, NULL, NULL, &w, &h);/* get texture size*/

  SDL_Rect dstrect;/* set output rectangle */
  dstrect.x = x;
  dstrect.y = y;
  dstrect.w = w;
  dstrect.h = h;

  SDL_Rect dstrectb;/* set background rectangle */
  dstrectb.x = dstrect.x - 1;
  dstrectb.y = dstrect.y - 1;
  dstrectb.w = w + 2;
  dstrectb.h = h + 2;

  Uint8 or, og, ob, oa; /* old render draw colors */
  SDL_GetRenderDrawColor(renderer, &or, &og, &ob, &oa); /* fill old color */
  SDL_SetRenderDrawColor(renderer, r, g, b, a); /* set new color */
  SDL_RenderFillRect(renderer, &dstrectb); /* draw background rectangle */
  SDL_SetRenderDrawColor(renderer, or, og, ob, oa); /* restore old color */


  SDL_RenderCopy(renderer, texture, NULL, &dstrect);
  return 0;
}

/* render texture with absolute coordinate on top right */
int render_copy_absolute_fb_tr(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a){
  int w, h;
  SDL_QueryTexture(texture, NULL, NULL, &w, &h);/* get texture size*/

  SDL_Rect dstrect;/* set output rectangle */
  dstrect.x = x - w;
  dstrect.y = y;
  dstrect.w = w;
  dstrect.h = h;

  SDL_Rect dstrectb;/* set background rectangle */
  dstrectb.x = dstrect.x - 1;
  dstrectb.y = dstrect.y - 1;
  dstrectb.w = w + 2;
  dstrectb.h = h + 2;

  Uint8 or, og, ob, oa; /* old render draw colors */
  SDL_GetRenderDrawColor(renderer, &or, &og, &ob, &oa); /* fill old color */
  SDL_SetRenderDrawColor(renderer, r, g, b, a); /* set new color */
  SDL_RenderFillRect(renderer, &dstrectb); /* draw background rectangle */
  SDL_SetRenderDrawColor(renderer, or, og, ob, oa); /* restore old color */

  SDL_RenderCopy(renderer, texture, NULL, &dstrect);
  return 0;
}

/* render texture with absolute coordinate on bottom left */
int render_copy_absolute_fb_bl(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a){
  int w, h;
  SDL_QueryTexture(texture, NULL, NULL, &w, &h);/* get texture size*/

  SDL_Rect dstrect;/* set output rectangle */
  dstrect.x = x;
  dstrect.y = y - h;
  dstrect.w = w;
  dstrect.h = h;

  SDL_Rect dstrectb;/* set background rectangle */
  dstrectb.x = dstrect.x - 1;
  dstrectb.y = dstrect.y - 1;
  dstrectb.w = w + 2;
  dstrectb.h = h + 2;

  Uint8 or, og, ob, oa; /* old render draw colors */
  SDL_GetRenderDrawColor(renderer, &or, &og, &ob, &oa); /* fill old color */
  SDL_SetRenderDrawColor(renderer, r, g, b, a); /* set new color */
  SDL_RenderFillRect(renderer, &dstrectb); /* draw background rectangle */
  SDL_SetRenderDrawColor(renderer, or, og, ob, oa); /* restore old color */

  SDL_RenderCopy(renderer, texture, NULL, &dstrect);
  return 0;
}


/* render texture with absolute coordinate on bottom right */
int render_copy_absolute_fb_br(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a){
  int w, h;
  SDL_QueryTexture(texture, NULL, NULL, &w, &h);/* get texture size*/

  SDL_Rect dstrect;/* set output rectangle */
  dstrect.x = x - w;
  dstrect.y = y - h;
  dstrect.w = w;
  dstrect.h = h;

  SDL_Rect dstrectb;/* set background rectangle */
  dstrectb.x = dstrect.x - 1;
  dstrectb.y = dstrect.y - 1;
  dstrectb.w = w + 2;
  dstrectb.h = h + 2;

  Uint8 or, og, ob, oa; /* old render draw colors */
  SDL_GetRenderDrawColor(renderer, &or, &og, &ob, &oa); /* fill old color */
  SDL_SetRenderDrawColor(renderer, r, g, b, a); /* set new color */
  SDL_RenderFillRect(renderer, &dstrectb); /* draw background rectangle */
  SDL_SetRenderDrawColor(renderer, or, og, ob, oa); /* restore old color */

  SDL_RenderCopy(renderer, texture, NULL, &dstrect);
  return 0;
}

/* render texture with absolute coordinate on center */
int render_copy_absolute_fb_c(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a){
  int w, h;
  SDL_QueryTexture(texture, NULL, NULL, &w, &h);/* get texture size*/

  SDL_Rect dstrect;/* set output rectangle */
  dstrect.x = x - w/2;
  dstrect.y = y - h/2;
  dstrect.w = w;
  dstrect.h = h;

  SDL_Rect dstrectb;/* set background rectangle */
  dstrectb.x = dstrect.x - 1;
  dstrectb.y = dstrect.y - 1;
  dstrectb.w = w + 2;
  dstrectb.h = h + 2;

  Uint8 or, og, ob, oa; /* old render draw colors */
  SDL_GetRenderDrawColor(renderer, &or, &og, &ob, &oa); /* fill old color */
  SDL_SetRenderDrawColor(renderer, r, g, b, a); /* set new color */
  SDL_RenderFillRect(renderer, &dstrectb); /* draw background rectangle */
  SDL_SetRenderDrawColor(renderer, or, og, ob, oa); /* restore old color */

  SDL_RenderCopy(renderer, texture, NULL, &dstrect);
  return 0;
}


/* render copy with coordinates in float from 0 to 1 */

/* render texture with relative coordinate on top left */
int render_copy_relative_fb_tl(SDL_Renderer* renderer, SDL_Texture* texture, float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a){
  int display_w, display_h;
  SDL_GetRendererOutputSize(renderer, &display_w, &display_h);
  int ax = x*display_w; /* calculate absolute coordinates */
  int ay = y*display_h;

  /* call absolute render copy */
  render_copy_absolute_fb_tl(renderer, texture, ax, ay, r, g, b, a);
  return 0;
}

/* render texture with relative coordinate on top right */
int render_copy_relative_fb_tr(SDL_Renderer* renderer, SDL_Texture* texture, float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a){
  int display_w, display_h;
  SDL_GetRendererOutputSize(renderer, &display_w, &display_h);
  int ax = x*display_w; /* calculate absolute coordinates */
  int ay = y*display_h;

  /* call absolute render copy */
  render_copy_absolute_fb_tr(renderer, texture, ax, ay, r, g, b, a);
  return 0;
}

/* render texture with relative coordinate on bottom left */
int render_copy_relative_fb_bl(SDL_Renderer* renderer, SDL_Texture* texture, float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a){
  int display_w, display_h;
  SDL_GetRendererOutputSize(renderer, &display_w, &display_h);
  int ax = x*display_w; /* calculate absolute coordinates */
  int ay = y*display_h;

  /* call absolute render copy */
  render_copy_absolute_fb_bl(renderer, texture, ax, ay, r, g, b, a);
  return 0;
}

/* render texture with relative coordinate on bottom right */
int render_copy_relative_fb_br(SDL_Renderer* renderer, SDL_Texture* texture, float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a){
  int display_w, display_h;
  SDL_GetRendererOutputSize(renderer, &display_w, &display_h);
  int ax = x*display_w; /* calculate absolute coordinates */
  int ay = y*display_h;

  /* call absolute render copy */
  render_copy_absolute_fb_br(renderer, texture, ax, ay, r, g, b, a);
  return 0;
}

/* render texture with relative coordinate on center */
int render_copy_relative_fb_c(SDL_Renderer* renderer, SDL_Texture* texture, float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a){
  int display_w, display_h;
  SDL_GetRendererOutputSize(renderer, &display_w, &display_h);
  int ax = x*display_w; /* calculate absolute coordinates */
  int ay = y*display_h;

  /* call absolute render copy */
  render_copy_absolute_fb_c(renderer, texture, ax, ay, r, g, b, a);
  return 0;
}



/* render copy with outlined background */
/* render copy with coordinates on display */

/* render texture with absolute coordinate on top left */
int render_copy_absolute_ob_tl(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a){
  int w, h;
  SDL_QueryTexture(texture, NULL, NULL, &w, &h);/* get texture size*/

  SDL_Rect dstrect;/* set output rectangle */
  dstrect.x = x;
  dstrect.y = y;
  dstrect.w = w;
  dstrect.h = h;

  SDL_Rect dstrectb;/* set background rectangle */
  dstrectb.x = dstrect.x - 1;
  dstrectb.y = dstrect.y - 1;
  dstrectb.w = w + 2;
  dstrectb.h = h + 2;

  Uint8 or, og, ob, oa; /* old render draw colors */
  SDL_GetRenderDrawColor(renderer, &or, &og, &ob, &oa); /* fill old color */
  SDL_SetRenderDrawColor(renderer, r, g, b, a); /* set new color */
  SDL_RenderFillRect(renderer, &dstrectb); /* draw background rectangle */
  SDL_SetRenderDrawColor(renderer, or, og, ob, oa); /* restore old color */


  SDL_RenderCopy(renderer, texture, NULL, &dstrect);
  return 0;
}

/* render texture with absolute coordinate on top right */
int render_copy_absolute_ob_tr(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a){
  int w, h;
  SDL_QueryTexture(texture, NULL, NULL, &w, &h);/* get texture size*/

  SDL_Rect dstrect;/* set output rectangle */
  dstrect.x = x - w;
  dstrect.y = y;
  dstrect.w = w;
  dstrect.h = h;

  SDL_Rect dstrectb;/* set background rectangle */
  dstrectb.x = dstrect.x - 1;
  dstrectb.y = dstrect.y - 1;
  dstrectb.w = w + 2;
  dstrectb.h = h + 2;

  Uint8 or, og, ob, oa; /* old render draw colors */
  SDL_GetRenderDrawColor(renderer, &or, &og, &ob, &oa); /* fill old color */
  SDL_SetRenderDrawColor(renderer, r, g, b, a); /* set new color */
  SDL_RenderDrawRect(renderer, &dstrectb); /* draw background rectangle */
  SDL_SetRenderDrawColor(renderer, or, og, ob, oa); /* restore old color */

  SDL_RenderCopy(renderer, texture, NULL, &dstrect);
  return 0;
}

/* render texture with absolute coordinate on bottom left */
int render_copy_absolute_ob_bl(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a){
  int w, h;
  SDL_QueryTexture(texture, NULL, NULL, &w, &h);/* get texture size*/

  SDL_Rect dstrect;/* set output rectangle */
  dstrect.x = x;
  dstrect.y = y - h;
  dstrect.w = w;
  dstrect.h = h;

  SDL_Rect dstrectb;/* set background rectangle */
  dstrectb.x = dstrect.x - 1;
  dstrectb.y = dstrect.y - 1;
  dstrectb.w = w + 2;
  dstrectb.h = h + 2;

  Uint8 or, og, ob, oa; /* old render draw colors */
  SDL_GetRenderDrawColor(renderer, &or, &og, &ob, &oa); /* fill old color */
  SDL_SetRenderDrawColor(renderer, r, g, b, a); /* set new color */
  SDL_RenderDrawRect(renderer, &dstrectb); /* draw background rectangle */
  SDL_SetRenderDrawColor(renderer, or, og, ob, oa); /* restore old color */

  SDL_RenderCopy(renderer, texture, NULL, &dstrect);
  return 0;
}


/* render texture with absolute coordinate on bottom right */
int render_copy_absolute_ob_br(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a){
  int w, h;
  SDL_QueryTexture(texture, NULL, NULL, &w, &h);/* get texture size*/

  SDL_Rect dstrect;/* set output rectangle */
  dstrect.x = x - w;
  dstrect.y = y - h;
  dstrect.w = w;
  dstrect.h = h;

  SDL_Rect dstrectb;/* set background rectangle */
  dstrectb.x = dstrect.x - 1;
  dstrectb.y = dstrect.y - 1;
  dstrectb.w = w + 2;
  dstrectb.h = h + 2;

  Uint8 or, og, ob, oa; /* old render draw colors */
  SDL_GetRenderDrawColor(renderer, &or, &og, &ob, &oa); /* fill old color */
  SDL_SetRenderDrawColor(renderer, r, g, b, a); /* set new color */
  SDL_RenderDrawRect(renderer, &dstrectb); /* draw background rectangle */
  SDL_SetRenderDrawColor(renderer, or, og, ob, oa); /* restore old color */

  SDL_RenderCopy(renderer, texture, NULL, &dstrect);
  return 0;
}

/* render texture with absolute coordinate on center */
int render_copy_absolute_ob_c(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a){
  int w, h;
  SDL_QueryTexture(texture, NULL, NULL, &w, &h);/* get texture size*/

  SDL_Rect dstrect;/* set output rectangle */
  dstrect.x = x - w/2;
  dstrect.y = y - h/2;
  dstrect.w = w;
  dstrect.h = h;

  SDL_Rect dstrectb;/* set background rectangle */
  dstrectb.x = dstrect.x - 1;
  dstrectb.y = dstrect.y - 1;
  dstrectb.w = w + 2;
  dstrectb.h = h + 2;

  Uint8 or, og, ob, oa; /* old render draw colors */
  SDL_GetRenderDrawColor(renderer, &or, &og, &ob, &oa); /* fill old color */
  SDL_SetRenderDrawColor(renderer, r, g, b, a); /* set new color */
  SDL_RenderDrawRect(renderer, &dstrectb); /* draw background rectangle */
  SDL_SetRenderDrawColor(renderer, or, og, ob, oa); /* restore old color */

  SDL_RenderCopy(renderer, texture, NULL, &dstrect);
  return 0;
}


/* render copy with coordinates in float from 0 to 1 */

/* render texture with relative coordinate on top left */
int render_copy_relative_ob_tl(SDL_Renderer* renderer, SDL_Texture* texture, float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a){
  int display_w, display_h;
  SDL_GetRendererOutputSize(renderer, &display_w, &display_h);
  int ax = x*display_w; /* calculate absolute coordinates */
  int ay = y*display_h;

  /* call absolute render copy */
  render_copy_absolute_ob_tl(renderer, texture, ax, ay, r, g, b, a);
  return 0;
}

/* render texture with relative coordinate on top right */
int render_copy_relative_ob_tr(SDL_Renderer* renderer, SDL_Texture* texture, float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a){
  int display_w, display_h;
  SDL_GetRendererOutputSize(renderer, &display_w, &display_h);
  int ax = x*display_w; /* calculate absolute coordinates */
  int ay = y*display_h;

  /* call absolute render copy */
  render_copy_absolute_ob_tr(renderer, texture, ax, ay, r, g, b, a);
  return 0;
}

/* render texture with relative coordinate on bottom left */
int render_copy_relative_ob_bl(SDL_Renderer* renderer, SDL_Texture* texture, float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a){
  int display_w, display_h;
  SDL_GetRendererOutputSize(renderer, &display_w, &display_h);
  int ax = x*display_w; /* calculate absolute coordinates */
  int ay = y*display_h;

  /* call absolute render copy */
  render_copy_absolute_ob_bl(renderer, texture, ax, ay, r, g, b, a);
  return 0;
}

/* render texture with relative coordinate on bottom right */
int render_copy_relative_ob_br(SDL_Renderer* renderer, SDL_Texture* texture, float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a){
  int display_w, display_h;
  SDL_GetRendererOutputSize(renderer, &display_w, &display_h);
  int ax = x*display_w; /* calculate absolute coordinates */
  int ay = y*display_h;

  /* call absolute render copy */
  render_copy_absolute_ob_br(renderer, texture, ax, ay, r, g, b, a);
  return 0;
}

/* render texture with relative coordinate on center */
int render_copy_relative_ob_c(SDL_Renderer* renderer, SDL_Texture* texture, float x, float y, Uint8 r, Uint8 g, Uint8 b, Uint8 a){
  int display_w, display_h;
  SDL_GetRendererOutputSize(renderer, &display_w, &display_h);
  int ax = x*display_w; /* calculate absolute coordinates */
  int ay = y*display_h;

  /* call absolute render copy */
  render_copy_absolute_ob_c(renderer, texture, ax, ay, r, g, b, a);
  return 0;
}
