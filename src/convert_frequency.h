#ifndef CONVERT_FREQUENCY_H
#define CONVERT_FREQUENCY_H

#include <stdio.h>
#include <math.h>

/* calculate the midi note of the frequency */
double f_to_midi(double frequency);

/* calculate the midi note of the frequency rounded to the closest whole number */
long int f_to_midi_rounded(double frequency);

/* clamps the input from 21 to 128, the displayable range in english */
long int clamp_midi_english(long int midi_note);

/* calculate the frequency of the midi note */
double midi_to_f(double frequency);

/* get the string for the note's english name */
char* get_note_name_english(long int midi_note);

#endif /* CONVERT_FREQUENCY_H */
