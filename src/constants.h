#ifndef CONSTANTS_H
#define CONSTANTS_H

/* constants to be used and various places */
#define DIR_LENGTH 4096  /* maximum string length for files and directories */

#define WINDOW_DEF_W 640  /* minimum width and height for the window */
#define WINDOW_DEF_H 480

#define WINDOW_MIN_W 600  /* minimum width and height for the window */
#define WINDOW_MIN_H 440

#define UI_TOP_HEIGHT 28 /* height of top of UI */

#define VOLUME_BAR_SPACING_X 10  /* spacing between volume bar stuff */
#define VOLUME_BAR_SPACING_Y 3
#define VOLUME_BAR_SPACING_Y_LARGE 10
#define VOLUME_BAR_HEIGHT 12  /* height of each volume bar */
#define VOLUME_SLIDER_MARKER_WIDTH 5  /* width of the volume bar slider marker */
#define VOLUME_SLIDER_HEIGHT (VOLUME_BAR_SPACING_Y_LARGE + UI_TOP_HEIGHT + VOLUME_BAR_HEIGHT * 2 + VOLUME_BAR_SPACING_Y + VOLUME_BAR_SPACING_Y_LARGE + VOLUME_BAR_HEIGHT * 2 + VOLUME_BAR_SPACING_Y + VOLUME_BAR_SPACING_Y + VOLUME_BAR_HEIGHT/2) /* height where volume bar slider is at */

#define DB_LIMIT -64 /* min db that can be displayed */

#define FREQUENCY_HEIGHT 150 /* display position for frequency displays */
#define FREQUENCY_HEIGHT_2 220
#define FREQUENCY_X 0.5
#define MIDI_NUM_X 0.28
#define NOTE_X 0.78

#define CONTENT_HEIGHT 250 /* height for content */

#define WANTED_AUDIO_BUFFER_SIZE 4096 /* wanted audio buffer size. must be powers of 2 */


#endif /* CONSTANTS_H */
