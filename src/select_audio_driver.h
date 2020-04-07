#ifndef SELECT_AUDIO_DRIVER_H
#define SELECT_AUDIO_DRIVER_H

#include <stdio.h>

#include "SDL.h"

#include "states.h"
#include "constants.h"

#include "audio_system.h"


int select_audio_driver_events(int* program_state, int* updatescreen, SDL_Renderer* renderer, SDL_Event* e, audio_system* system);

int select_audio_driver_process(int* program_state, int* updatescreen, audio_system* system);

int select_audio_driver_display(int* program_state, SDL_Renderer* renderer, audio_system* system);

#endif /* SELECT_AUDIO_DRIVER_H */
