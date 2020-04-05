#ifndef SELECT_AUDIO_DRIVER_H
#define SELECT_AUDIO_DRIVER_H

#include <stdio.h>

#include "SDL.h"

#include "states.h"


int select_audio_driver_events(int* program_state, int* next_program_state, int* updatescreen, const SDL_Event* e);

int select_audio_driver_process(int* program_state, int* next_program_state, int* updatescreen);

int select_audio_driver_display(int* program_state, int* next_program_state, const SDL_Renderer* renderer);

#endif /* SELECT_AUDIO_DRIVER_H */
