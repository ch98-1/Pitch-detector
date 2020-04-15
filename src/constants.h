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

#define FREQUENCY_DISPLAY_CONTENT_HEIGHT 250 /* height for content when frequency is displayed */
#define DRIVER_CONTENT_HEIGHT 140 /* height for content when in driver select screen */
#define DEVICE_CONTENT_HEIGHT 120 /* height for content when in device select screen */

#define SAMPLE_RATE 48000 /* sample rate for the audio processing */

#define WANTED_AUDIO_BUFFER_SIZE 2048 /* wanted audio buffer size. must be powers of 2 */

#define AUDIO_PROSESSING_LENGTH 16384   /* numer of samples in each channel to process */

#define PEAK_VALUE_LENGTH 3200 /* number of samples to find the peak from. 3200 = last 1/15 of a second */

#define PLAYBACK_BUFFER_LENGTH 2048 /* number of samples wanted in the playback buffer before loading more */


#define PLAY_NOTE_VOLUME 0.2 /* volume to play the tone at */


/* number of samples of peak values to store for calculating max values
will depend on the speed at whicth the audio loop occurs so test and change as needed */
#define MAX_VALUE_BUFFER_LENGTH 512

#define MIN_FRAMERATE 30.0 /* minimul framerate graphic tries to update at */

#define MIN_LOOP_DELAY 0.001 /* minimul loop delay in seconds */

#define DRIVER_TEXT_LENGTH 16 /* max number of characters for driver name */
#define DEVICE_TEXT_LENGTH 22 /* max number of characters for devicer name */

#define DRIVER_BUTTON_WIDTH 210 /* width, height, and spacing for driver and device select buttons */
#define DRIVER_BUTTON_HEIGHT 28
#define DRIVER_BUTTON_HEIGHT_SPACING 8
#define DEVICE_BUTTON_WIDTH 270
#define DEVICE_BUTTON_HEIGHT 28
#define DEVICE_BUTTON_HEIGHT_SPACING 4

#define TEST_BUTTON_WIDTH 120 /* width, height, and position for test button */
#define TEST_BUTTON_HEIGHT 28
#define TEST_BUTTON_HEIGHT_SPACING 10 /* spacing from the bottom */

#define CHANNEL_BUTTON_WIDTH 28 /* width and height of the channel switching button */
#define CHANNEL_BUTTON_HEIGHT 28
#define CHANNEL_BUTTON_TOP_SPACING 125 /* spacing from the left and top */
#define CHANNEL_BUTTON_LEFT_SPACING 10

#define PLAYBACK_BUTTON_WIDTH 100 /* width and height of the channel switching button */
#define PLAYBACK_BUTTON_HEIGHT 28
#define PLAYBACK_BUTTON_TOP_SPACING 125 /* spacing from the right and top */
#define PLAYBACK_BUTTON_RIGHT_SPACING 10

#endif /* CONSTANTS_H */
