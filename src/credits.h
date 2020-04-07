#ifndef CREDITS_H
#define CREDITS_H

#include <stdio.h>

#include "SDL.h"
#include "SDL_image.h"

#include "states.h"
#include "constants.h"

#include "rendering_helper.h"
#include "audio_system.h"

SDL_Texture* credits_texture;

int credit_events(int* program_state, int* updatescreen, SDL_Event* e);

int credit_process(int* program_state, int* updatescreen);

int credit_display(int* program_state, SDL_Renderer* renderer, audio_system* system);

int credit_cleanup();

#endif /* CREDITS_H */
