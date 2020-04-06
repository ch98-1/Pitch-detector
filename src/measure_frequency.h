#ifndef MEASURE_FREQUENCY_H
#define MEASURE_FREQUENCY_H

#include <stdio.h>
#include <math.h>

#include "SDL.h"
#include "kiss_fft.h"

#include "states.h"


int measure_frequency_events(int* program_state, int* updatescreen, SDL_Event* e);

int measure_frequency_process(int* program_state, int* updatescreen);

int measure_frequency_display(int* program_state, SDL_Renderer* renderer);

#endif /* MEASURE_FREQUENCY_H */
