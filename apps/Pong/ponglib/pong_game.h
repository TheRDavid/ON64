/**
 * @file pong_game.h
 * @brief pong game functions
 */
#ifndef __PONG_GAME
#define __PONG_GAME
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <libdragon.h>
#include <stdlib.h>
#include <time.h>
#include <audio.h>
#include <mikmod.h>
#include <unistd.h>
#include <tools.h>
#include <grafix.h>
#include <math.h>
#include <effects.h>
#include <sound.h>
#include <game.h>

#define main_menu_num_items 4

typedef struct player
{
    int score;
    int x;
    int y;
    int width;
    int height;
    uint16_t color;
    int acceleration;
} player;

typedef struct ball
{
    int x;
    int y;
    int acceleration_x;
    int acceleration_y;
    uint16_t color;
    int radius;
} ball;

#endif