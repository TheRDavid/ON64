#include <stdio.h>
#include <string.h>
#include <libdragon.h>
#include <stdlib.h>
#include <unistd.h>
#include <tools.h>
#include <grafix.h>
#include <math.h>
#include <time.h>
#include <game.h>
#include <pong_game.h>
#include <main_menu.h>
#include <main_game.h>


int main(void)
{
    display_context_t display = 0;
    tools_init("1.0", display, TRUE, TRUE, TRUE);

    SCREEN *menu_screen = malloc(sizeof(SCREEN));
    menu_screen->init = init_main_menu;
    menu_screen->init(menu_screen, display);
    run_screen(menu_screen);

while(TRUE){
    while( !(display = display_lock()) );
		tools_show(display, TRUE, 1);}
}

