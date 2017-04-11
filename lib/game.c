#include <game.h>

void run_screen(SCREEN *screen)
{
    screen->active = TRUE;
    while(screen->active)
    {
        screen->calc(screen);
        while( !(screen->display = display_lock()) );
		tools_update();
        screen->draw(screen);
		tools_show(screen->display, screen->debug, 0);
    }
    screen->exit(screen);
}

void run_new_screen(SCREEN *screen, SCREEN *previous)
{
    //free(previous);
    run_screen(screen);
}