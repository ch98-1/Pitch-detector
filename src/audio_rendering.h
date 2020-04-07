#ifndef AUDIO_RENDERING_H
#define AUDIO_RENDERING_H

#include <stdio.h>
#include <math.h>

#include "SDL.h"
#include "SDL_image.h"

#include "states.h"
#include "colors.h"
#include "constants.h"
#include "fonts.h"

#include "audio_system.h"
#include "rendering_helper.h"
#include "text_rendering.h"

SDL_Texture* volume_bar_texture;
SDL_Texture* input_icon_texture;
SDL_Texture* output_icon_texture;

float get_db_limited(float volume);/* get db with lower limit */

float get_db_limited_normalized(float volume);/* get db normalised from 0 to 1 */


/* render a full width volume bar with top edge at height of y
volumes should be normalised with max at 1
Will strech to full width of the window with some margins*/
int render_volume_bar(SDL_Renderer* renderer, audio_system* system, int y);

int audio_rendering_cleanup();

#endif /* AUDIO_RENDERING_H */
