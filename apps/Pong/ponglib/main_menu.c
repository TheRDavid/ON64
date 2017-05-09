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


char main_menu_items[main_menu_num_items][40] = { 	{"Start"},
                                                    {"Max Points:"},
                                                    {"Player 1 Color:"},
                                                    {"Player 2 Color:"} ,
                                                    {"Player 1 inverted:"},
                                                    {"Player 2 inverted:"}  }; 

int player_colors[4] = { 0x7BFF, 0xFA11, 0xFFFF, 0xFFC1 };
int joystick_back_to_zero[] = {TRUE, TRUE};

int draw_main_menu(struct SCREEN *screen)
{
    graphics_fill_screen( screen->display, GFX_COLOR_BLACK);
    int xPos;
    for(int i = 0; i < main_menu_num_items - 2; i++)
    {
        if(i == screen->params[0])
            graphics_set_color(GFX_COLOR_YELLOW, GFX_COLOR_BLACK);
        else
            graphics_set_color(GFX_COLOR_WHITE, GFX_COLOR_BLACK);
        xPos = (ZERO_X + RES_X) / 2 - strlen(main_menu_items[i]) / 2 * 10;
        graphics_draw_text(screen->display, xPos, 20 + i * 35, main_menu_items[i]);
    }
    graphics_set_color(GFX_COLOR_WHITE, GFX_COLOR_BLACK);

    char livesText[35];
    snprintf(livesText, 35, "< %d >", screen->params[1]);

    if(screen->params[0] == 1) graphics_set_color(GFX_COLOR_YELLOW, GFX_COLOR_BLACK);
    graphics_draw_text(screen->display, (ZERO_X + RES_X) / 2 + 50, 55, livesText); 
    gfx_draw_rectangle(screen->display, 5 + (ZERO_X + RES_X) / 2 + strlen(main_menu_items[2]) / 2 * 10 - 20, 85, 60,20, player_colors[screen->params[2]], TRUE);
    gfx_draw_rectangle(screen->display, 5 + (ZERO_X + RES_X) / 2 + strlen(main_menu_items[2]) / 2 * 10 - 20, 120, 60,20, player_colors[screen->params[3]], TRUE);
    
    graphics_set_color(GFX_COLOR_WHITE, GFX_COLOR_BLACK);
    if(screen->params[0] == 4) graphics_set_color(GFX_COLOR_YELLOW, GFX_COLOR_BLACK);

    graphics_draw_text(screen->display, xPos, 160, main_menu_items[4]); 
    if(screen->params[4] == 1) graphics_draw_text(screen->display, xPos + 160, 160, "kay");
    else graphics_draw_text(screen->display, xPos + 160, 160, "nah"); 
    graphics_set_color(GFX_COLOR_WHITE, GFX_COLOR_BLACK);
    if(screen->params[0] == 5) graphics_set_color(GFX_COLOR_YELLOW, GFX_COLOR_BLACK);

    graphics_draw_text(screen->display, xPos, 190, main_menu_items[5]); 
    if(screen->params[5] == 1) graphics_draw_text(screen->display, xPos + 160, 190, "kay"); 
    else graphics_draw_text(screen->display, xPos + 160, 190, "nah"); 

}

int calc_main_menu(struct SCREEN *screen)
{
    struct controller_data keys = get_keys_pressed();

    int dirY = keys.c[0].y > 20 ? 1 : keys.c[0].y < -20 ? -1 : 0;
    int dirX = keys.c[0].x > 20 ? 1 : keys.c[0].x < -20 ? -1 : 0;
    if(joystick_back_to_zero[1])
    {
        screen->params[0] -= dirY;
        if(screen->params[0] == main_menu_num_items) screen->params[0] = 0;
        if(screen->params[0] == -1) screen->params[0] = main_menu_num_items - 1;
        char msg[30];
        snprintf(msg, 30, "%dx%d", screen->params[0], keys.c[0].y);
        tools_print(msg);
    }
    if(joystick_back_to_zero[0])
    {
        switch(screen->params[0])
        {
            case 1: screen->params[1] = screen->params[1] + dirX >= 1 ? screen->params[1] + dirX : screen->params[1];
                    break;
            case 2: screen->params[2] = screen->params[2] + dirX == -1 ? 3 : screen->params[2] + dirX == 4 ? 0 : screen->params[2] + dirX;
                    break;
            case 3: screen->params[3] = screen->params[3] + dirX == -1 ? 3 : screen->params[3] + dirX == 4 ? 0 : screen->params[3] + dirX;
                    break;
            case 4: screen->params[4] = dirX == 0 ? screen->params[4] : screen->params[4] * -1;
                    break;
            case 5: screen->params[5] = dirX == 0 ? screen->params[5] : screen->params[5] * -1;
                    break;
            default: break;
        }
    }
    if(keys.c[0].A)
    {
        SCREEN *main_game_screen = malloc(sizeof(SCREEN));
        main_game_screen->init = init_main_game_screen;

        main_game_screen->params[1] = screen->params[1];      
        main_game_screen->params[2] = player_colors[screen->params[2]];    
        main_game_screen->params[3] = player_colors[screen->params[3]];   
        main_game_screen->params[4] = screen->params[4];    
        main_game_screen->params[5] = screen->params[5];   

        main_game_screen->init(main_game_screen, screen->display); 
        run_new_screen(main_game_screen, screen);
    }
    joystick_back_to_zero[0] = dirX == 0;
    joystick_back_to_zero[1] = dirY == 0;
}

int exit_main_menu(struct SCREEN *screen)
{
}

int init_main_menu(struct SCREEN *screen, display_context_t display)
{
    screen->draw = draw_main_menu;
    screen->calc = calc_main_menu;
    screen->exit = exit_main_menu;
    screen->display = display;
    screen->params[0] = 0;      // selection
    screen->params[1] = 3;      // max points
    screen->params[2] = 0;      // color player1
    screen->params[3] = 1;      // color player2
    screen->params[4] = 1;      // player1 inverted
    screen->params[5] = 1;      // player2 inverted
}
