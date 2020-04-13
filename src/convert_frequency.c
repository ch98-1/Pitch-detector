#include "convert_frequency.h"

/* calculate the midi note of the frequency */
double f_to_midi(double frequency){
  if (frequency > 0.000001){ /* if in representable range */
    return 69.0 + 12 * log(frequency / 440.0 )/log(2); /* calculate the midi note number */
  }
  else {
    return -HUGE_VAL; /* if outside return something very low */
  }
}

/* calculate the midi note of the frequency rounded to the closest whole number */
long int f_to_midi_rounded(double frequency){
  return (long int) round(f_to_midi(frequency)); /* round the midi note that was calculated */
}

/* clamps the input from 21 to 128, the displayable range in english */
long int clamp_midi_english(long int midi_note){
  if (midi_note < 21){ /* if less than minimum */
    return 21;
  }
  else if (midi_note > 128){
    return 128; /* if more than maximum */
  }
  else{
    return midi_note; /* if just right */
  }
}

/* calculate the frequency of the midi note */
double midi_to_f(double midi_note){
  return pow(2, (midi_note - 69.0)/12 ) * 440.0; /* return calculated frequency */
}

/* get the string for the note's english name */
char* get_note_name_english(long int midi_note){
  long int clamped = clamp_midi_english(midi_note);
  char* text[108] = { /* big list of text */
    "A0",
    "A♯0/B♭0",
    "B",
    "C1",
    "C♯1/D♭1",
    "D1",
    "D♯1/E♭1",
    "E1",
    "F1",
    "F♯1/G♭1",
    "G1",
    "G♯1/A♭1",
    "A1",
    "A♯1/B♭1",
    "B1",
    "C2",
    "C♯2/D♭2",
    "D2",
    "D♯2/E♭2",
    "E2",
    "F2",
    "F♯2/G♭2",
    "G2",
    "G♯2/A♭2",
    "A2",
    "A♯2/B♭2",
    "B2",
    "C3",
    "C♯3/D♭3",
    "D3",
    "D♯3/E♭3",
    "E3",
    "F3",
    "F♯3/G♭3",
    "G3",
    "G♯3/A♭3",
    "A3",
    "A♯3/B♭3",
    "B3",
    "C4",
    "C♯4/D♭4",
    "D4",
    "D♯4/E♭4",
    "E4",
    "F4",
    "F♯4/G♭4",
    "G4",
    "G♯4/A♭4",
    "A4",
    "A♯4/B♭4",
    "B4",
    "C5",
    "C♯5/D♭5",
    "D5",
    "D♯5/E♭5",
    "E5",
    "F5",
    "F♯5/G♭5",
    "G5",
    "G♯5/A♭5",
    "A5",
    "A♯5/B♭5",
    "B5",
    "C6",
    "C♯6/D♭6",
    "D6",
    "D♯6/E♭6",
    "E6",
    "F6",
    "F♯6/G♭6",
    "G6",
    "G♯6/A♭6",
    "A6",
    "A♯6/B♭6",
    "B6",
    "C7",
    "C♯7/D♭7",
    "D7",
    "D♯7/E♭7",
    "E7",
    "F7",
    "F♯7/G♭7",
    "G7",
    "G♯7/A♭7",
    "A7",
    "A♯7/B♭7",
    "B7",
    "C8",
    "C♯8/D♭8",
    "D8",
    "D♯8/E♭8",
    "E8",
    "F8",
    "F♯8/G♭8",
    "G8",
    "G♯8/A♭8",
    "A8",
    "A♯8/B♭8",
    "B8",
    "C9",
    "C♯9/D♭9",
    "D9",
    "D♯9/E♭9",
    "E9",
    "F9",
    "F♯9/G♭9",
    "G9",
    "G♯9/A♭9"
  };
  return text[clamped-21]; /* return the wanted text */
}
