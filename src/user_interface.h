#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include "SDL.h"

#include "states.h"
#include "fonts.h"
#include "colors.h"
#include "constants.h"

#include "text_rendering.h"
#include "rendering_helper.h"


int draw_top_ui(int* program_state, SDL_Renderer* renderer);

int handle_top_mouseclick(int* program_state,int* updatescreen, float x);

#endif /* USER_INTERFACE_H */
