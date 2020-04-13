#ifndef MEASURE_FREQUENCY_H
#define MEASURE_FREQUENCY_H

#include <stdio.h>
#include <math.h>

#include "SDL.h"
#include "kiss_fft.h"

#include "colors.h"
#include "fonts.h"
#include "states.h"
#include "constants.h"

#include "audio_system.h"
#include "text_rendering.h"
#include "rendering_helper.h"
#include "audio_rendering.h"
#include "convert_frequency.h"


int measure_frequency_events(int* program_state, int* updatescreen, SDL_Renderer* renderer, SDL_Event* e, audio_system* system);

int measure_frequency_process(int* program_state, int* updatescreen, audio_system* system);

int measure_frequency_display(int* program_state, SDL_Renderer* renderer, audio_system* system);

#endif /* MEASURE_FREQUENCY_H */
