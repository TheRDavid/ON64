#include <stdio.h>
#include <string.h>
#include <libdragon.h>
#include <stdlib.h>
#include <unistd.h>
#include <tools.h>
#include <grafix.h>
#include <math.h>
#include <time.h>

typedef struct player
{
    int score;
    int position;
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

int collisionY(int object_acceleration, int obstacle_acceleration)
{
    return object_acceleration + obstacle_acceleration;
}

int main(void)
{
    static display_context_t display = 0;
    tools_init("1.0", display, TRUE, TRUE, TRUE);
    int debug = FALSE;
    int player_width = 10, player_height = 80, player1_x = ZERO_X + RES_X / 8, player2_x = ZERO_X + RES_X / 8 * 7;
    srand ( time(NULL) );
    int accX = rand() % 9 - 4;
    int accY = rand() % 9 - 4;

    ball ball = {  .color = GFX_COLOR_LRED, .radius = 8, .x = (ZERO_X + RES_X) / 2, .y = (ZERO_Y + RES_Y) / 2, 
                .acceleration_x = accX, .acceleration_y = accY };

    player player1 = { .position = ZERO_Y + RES_Y / 3, .color = GFX_COLOR_LRED };
    player player2 = { .position = ZERO_Y + RES_Y / 3 * 2, .color = GFX_COLOR_LBLUE };

    while(TRUE)
    {
        while(!(display = display_lock()));
        graphics_fill_screen(display, 0);
        tools_update();
        struct controller_data keys = get_keys_pressed();

        player1.acceleration = keys.c[0].y / 10;
        player2.acceleration = keys.c[1].y / 10;

        char msg[35];
        snprintf(msg, 35, "%d x %d", player1.acceleration, player2.acceleration);
        tools_print(msg);

        if(player1.position + player1.acceleration + player_height < MAX_Y && player1.position + player1.acceleration > 0)
            player1.position += player1.acceleration;

        if(player2.position + player2.acceleration + player_height < MAX_Y && player2.position + player2.acceleration > 0)
            player2.position += player2.acceleration;

        if(ball.x - ball.radius <= ZERO_X || ball.x + ball.radius >= MAX_X) ball.acceleration_x *= -1;
        if(ball.y - ball.radius <= ZERO_Y || ball.y + ball.radius >= MAX_Y) ball.acceleration_y *= -1;

        ball.x += ball.acceleration_x;
        ball.y += ball.acceleration_y;
        
        if(ball.x - ball.radius < player1_x + player_width && ball.x + ball.radius > player1_x && ball.y + ball.radius > player1.position && ball.y - ball.radius < player1.position + player_height)
        {
            ball.acceleration_x *= -1;
            ball.acceleration_y += player1.acceleration;
        }

        if(ball.x - ball.radius < player2_x + player_width && ball.x + ball.radius > player2_x && ball.y + ball.radius > player2.position && ball.y - ball.radius < player2.position + player_height)
        {
            ball.acceleration_x *= -1;
            ball.acceleration_y += player2.acceleration;
        }

        gfx_draw_circle(display, ball.x, ball.y, ball.radius, ball.color, FALSE);

        graphics_draw_box(display, player1_x, player1.position, player_width, player_height, player1.color);
        graphics_draw_box(display, player2_x, player2.position, player_width, player_height, player2.color);

		tools_show(display, debug, 0);
    }
}