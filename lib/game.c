#include <game.h>

/*
   Function: run_screen

   Launches a SCREEN

   Parameters:

      SCREEN *screen - Pointer to the screen-instance

   Details:

      - Loops given screen while active
      - Calls calc- and draw-function
      - Exits once screen is inactive
      - Updates Video-Display
    
*/
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