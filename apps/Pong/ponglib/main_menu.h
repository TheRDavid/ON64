/**
 * @file main_menu.h
 * @brief basic game functions
 */
#ifndef __MAIN_MENU
#define __MAIN_MENU

int init_main_menu(struct SCREEN *screen, display_context_t display);
int calc_main_menu(struct SCREEN *screen);
int draw_main_menu(struct SCREEN *screen);
int exit_main_menu(struct SCREEN *screen);

#endif