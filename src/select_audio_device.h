#ifndef SELECT_AUDIO_DEVICE_H
#define SELECT_AUDIO_DEVICE_H

#include <stdio.h>
#include <string.h>

#include "SDL.h"

#include "states.h"
#include "constants.h"

#include "audio_system.h"
#include "audio_rendering.h"



int select_audio_device_events(int* program_state, int* updatescreen, SDL_Renderer* renderer, SDL_Event* e, audio_system* system);

int select_audio_device_process(int* program_state, int* updatescreen, audio_system* system);

int select_audio_device_display(int* program_state, SDL_Renderer* renderer, audio_system* system);

#endif /* SELECT_AUDIO_DEVICE_H */
