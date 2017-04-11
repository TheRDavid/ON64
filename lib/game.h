/**
 * @file game.h
 * @brief basic game functions
 */
#ifndef __GAME
#define __GAME

#include <libdragon.h>
#include <tools.h>

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

void run_screen(SCREEN *screen);
void run_new_screen(SCREEN *screen, SCREEN *previous);

#endif