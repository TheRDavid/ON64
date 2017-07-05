/**
 * @file game.h
 * @brief basic game functions
 */
#ifndef __GAME
#define __GAME

#include <libdragon.h>
#include <tools.h>

/*
   Structure: SCREEN
   
   Used to structure the game into views, each with their own logic and drawing commands.

   Contains:

        - init-function pointer (setup variables 'n stuff')
        - calc-function pointer (implement logic here)
        - draw-function pointer (call drawing and image-processing functions here)
        - exit-function pointer (free memory)
        - int-array (store data here)
        - active-flag (stop the screen by setting to 0)
        - debug-flag (activate console by setting to 1)
        - display-reference

*/
typedef struct SCREEN
{
    int (*init)(SCREEN);
    int (*calc)(SCREEN);
    int (*draw)(SCREEN);
    int (*exit)(SCREEN);
    int params[128];
    int active;
    int debug;
    display_context_t display;
} SCREEN;

/*
   Function: run_screen

   Launches a SCREEN

   Parameters:

      SCREEN *screen - Pointer to the screen-instance
*/
void run_screen(SCREEN *screen);

#endif