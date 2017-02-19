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

int main(void)
{
	// INITIALIZE ALL THE THINGS
	static display_context_t disp = 0; 
	tools_init("2.3", disp);
	
	sprite_t* sprite = gfx_load_sprite("tracer.sprite");
	
	int ax = 0, ay = 0, bx = sprite->width, by = 0, cx = bx, cy = sprite->height, dx = 0, dy = cy;
	
	
	// GAME LOOP
	while(1) 
	{
		/* Grab a render buffer */
		while( !(disp = display_lock()) );
		graphics_fill_screen( disp, 0);

		tools_update();

		graphics_draw_sprite(disp, 0, 0, sprite);
		
		


        struct controller_data keys = get_keys_pressed();
        if( keys.c[0].A ) 
        {
            
        }
        else if( keys.c[0].B ) scroll = -1;

		tools_show(disp, 0, 0);
	}
}
