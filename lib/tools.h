/**
 * @file tools.h
 * @brief Stuff
 * @ingroup misc
 */
#ifndef __TOOLS_H
#define __TOOLS_H
#define TRUE 1
#define FALSE 0

int consoleIndex;

/**
 * INITIALIZE ALL THE THINGS for real now
 * 
 * ver - version
 */
void tools_init(char *ver, display_context_t d, int showFPS);

/**
 * refresh start time, play audio (if need be) and check input 
 */
void tools_update();

/**
 *  Draw the screen and if debugging - show performance
 * 
 *  display - display to draw on
 *  debug mode on or off
 */ 
void tools_show(display_context_t display, int debug, int consoleScroll);

/**
 * Print a debug message on the screen
 * 
 */ 
void tools_print(char *msg);

void fpsUpdater();

#endif
