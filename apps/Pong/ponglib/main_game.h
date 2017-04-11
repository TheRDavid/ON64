/**
 * @file main_game.h
 * @brief basic game functions
 */
#ifndef __MAIN_GAME
#define __MAIN_GAME

#include <libdragon.h>

ball pong_ball;
player player1, player2;
char gameOverText[35];
int exit_main_game_screen(struct SCREEN *screen);
int draw_main_game_screen(struct SCREEN *screen);
int calc_main_game_screen(struct SCREEN *screen);
int init_main_game_screen(struct SCREEN *screen, display_context_t display);
void ball_respawn(ball* ball);

#endif