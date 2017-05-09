#include <stdio.h>
#include <string.h>
#include <libdragon.h>
#include <stdlib.h>
#include <unistd.h>
#include <tools.h>
#include <grafix.h>
#include <math.h>
#include <time.h>
#include <pong_game.h>
#include <main_menu.h>
#include <main_game.h>

int ball_wait = 50;

void ball_respawn(ball* ball)
{
    int accX = 0;
    while(accX == 0) accX = (rand() % 9 - 4) / 2 + 1;
    int accY = (rand() % 9 - 4);
    ball->x = (ZERO_X + MAX_X) / 2;
    ball->y = (ZERO_Y + MAX_Y) / 2;
    ball->acceleration_x = accX;
    ball->acceleration_y = accY;
    ball_wait = 50;
}


int exit_main_game_screen(struct SCREEN *screen)
{
    free(screen);
}

int draw_main_game_screen(struct SCREEN *screen)
{
    graphics_fill_screen( screen->display, 0 );
    gfx_draw_rectangle(screen->display, ZERO_X, ZERO_Y, RES_X, RES_Y, GFX_COLOR_WHITE, FALSE);
    if(screen->params[6])
    {
        graphics_draw_text(screen->display, 100, 110, gameOverText);
        return 0;
    }
    gfx_draw_circle(screen->display, pong_ball.x, pong_ball.y, pong_ball.radius, pong_ball.color, FALSE);

    graphics_draw_box(screen->display, player1.x, player1.y, player1.width, player1.height, player1.color);
    graphics_draw_box(screen->display, player2.x, player2.y, player2.width, player2.height, player2.color);

    char scoreText[35];
    snprintf(scoreText, 35, "%d : %d", player1.score, player2.score);
    graphics_draw_text(screen->display, 150, 20, scoreText);
}

int ball_overlaps(int player)
{
    int player_x_max, player_x_min, player_x_centre;
    int player_y_max, player_y_min, player_y_centre;

    if(player == 1)
    {
        player_x_max = player1.x + player1.width;
        player_x_centre = player1.x + player1.width / 2;
        player_x_min = player1.x;
        player_y_max = player1.y + player1.height;
        player_y_centre = player1.y + player1.height / 2;
        player_y_min = player1.y;
    } else 
    {
        player_x_max = player2.x + player2.width;
        player_x_centre = player2.x + player2.width / 2;
        player_x_min = player2.x;
        player_y_max = player2.y + player2.height;
        player_y_centre = player2.y + player2.height / 2;
        player_y_min = player2.y;
    }

    int ball_x_max = pong_ball.x + pong_ball.radius;
    int ball_x_min = pong_ball.x - pong_ball.radius;
    int ball_y_max = pong_ball.y + pong_ball.radius;
    int ball_y_min = pong_ball.y - pong_ball.radius;

    int x_overlap = FALSE, y_overlap = FALSE;

    if (    ( ball_x_max < player_x_max && ball_x_max > player_x_min )
        ||  ( ball_x_min < player_x_max && ball_x_min > player_x_min )
        ||  ( ball_x_min < player_x_centre && ball_x_max > player_x_centre ))  x_overlap = TRUE;

    if (    ( ball_y_max < player_y_max && ball_y_max > player_y_min )
        ||  ( ball_y_min < player_y_max && ball_y_min > player_y_min )
        ||  ( ball_y_min < player_y_centre && ball_y_max > player_y_centre )) y_overlap = TRUE;
            
    return x_overlap && y_overlap;

}

int calc_main_game_screen(struct SCREEN *screen)
{
    struct controller_data keys = get_keys_pressed();

    if(screen->params[6])
    {
        if(keys.c[0].start || keys.c[1].start)
        {
            screen->active = FALSE;
        }
        return 0;
    }

    player1.acceleration = keys.c[0].y / 10 * screen->params[4];
    player2.acceleration = keys.c[1].y / 10 * screen->params[5];

    if(player1.y + player1.acceleration + player1.height < MAX_Y && player1.y + player1.acceleration > ZERO_Y)
        player1.y += player1.acceleration;

    if(player2.y + player2.acceleration + player2.height < MAX_Y && player2.y + player2.acceleration > ZERO_Y)
        player2.y += player2.acceleration;

    if(--ball_wait > 0) return;

    if(pong_ball.x - pong_ball.radius <= ZERO_X || pong_ball.x + pong_ball.radius >= MAX_X) pong_ball.acceleration_x *= -1;
    if(pong_ball.y - pong_ball.radius <= ZERO_Y || pong_ball.y + pong_ball.radius >= MAX_Y) pong_ball.acceleration_y *= -1;

    pong_ball.x += pong_ball.acceleration_x;
    pong_ball.y += pong_ball.acceleration_y;

    if(pong_ball.x < player1.x - 10)
    {
        player2.score++;
        ball_respawn(&pong_ball);
    } else if(pong_ball.x > player2.x + 10)
    {
        player1.score++;
        ball_respawn(&pong_ball);
    }
    
    if(ball_overlaps(1))
    {
        pong_ball.acceleration_x *= -1;
        pong_ball.acceleration_y += player1.acceleration;
        pong_ball.x += pong_ball.acceleration_x;

        pong_ball.acceleration_x += pong_ball.acceleration_x > 0 ? 1 : -1;
    }

    if(ball_overlaps(2))
    {
        pong_ball.acceleration_x *= -1;
        pong_ball.acceleration_y += player2.acceleration;
        pong_ball.x += pong_ball.acceleration_x;
        pong_ball.acceleration_x++;

        pong_ball.acceleration_x += pong_ball.acceleration_x > 0 ? 1 : -1;
    }

    pong_ball.acceleration_x = pong_ball.acceleration_x > 10 ? 10 : pong_ball.acceleration_x;
    pong_ball.acceleration_y = pong_ball.acceleration_y > 10 ? 10 : pong_ball.acceleration_y;

    if(player1.score >= screen->params[1])
    {
        snprintf(gameOverText, 35, "PLAYER 1 WINS");
        screen->params[6] = TRUE;
    } else if(player2.score >= screen->params[1])
    {
        snprintf(gameOverText, 35, "PLAYER 2 WINS");
        screen->params[6] = TRUE;
    }
       
    if(keys.c[0].start || keys.c[1].start)
    {
        screen->active = FALSE;
    }

}

int init_main_game_screen(struct SCREEN *screen, display_context_t display)
{
    screen->draw = draw_main_game_screen;
    screen->calc = calc_main_game_screen;
    screen->exit = exit_main_game_screen;
    screen->display = display;
    pong_ball = (struct ball) { .color = GFX_COLOR_LRED, .radius = 8 };
    ball_respawn(&pong_ball);
    player1 = (struct player) { .width = 10, .height = 80, .x = ZERO_X + RES_X / 8, .y = ZERO_Y + RES_Y / 3, .color = screen->params[2] };
    player2 = (struct player) { .width = 10, .height = 80, .x = ZERO_X + RES_X / 8 * 7, .y = ZERO_Y + RES_Y / 4 * 2, .color = screen->params[3] };
    screen->params[6] = FALSE; // game over
}