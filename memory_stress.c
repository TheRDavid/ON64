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
	
	sprite_t* playSprite = gfx_load_sprite("193.sprite");
	playSprite->hslices = 4;
	playSprite->vslices = 4;
	int mapNum = 193;
	
	int offset = 0, maxOffset = 31, /*loading = FALSE,*/ scroll = 0;
	
	
	// GAME LOOP
	while(1) 
	{
		/* Grab a render buffer */
		while( !(disp = display_lock()) );
		graphics_fill_screen( disp, 0);

		tools_update();
		gfx_draw_merged_sprite_stride(disp, playSprite, 0, 0, offset);
		
		if(offset < maxOffset)
			offset++;
		else
		{
			free(playSprite);
			offset = 0;
			mapNum++;
			char fName[30];
			sprintf(fName, "%d.sprite", mapNum);
			playSprite = gfx_load_sprite(fName);
			playSprite->hslices = 4;
			playSprite->vslices = 4;
		}


        struct controller_data keys = get_keys_pressed();
		scroll = 0;
        if( keys.c[0].A ) scroll = 1;
        else if( keys.c[0].B ) scroll = -1;

		tools_show(disp, 1, scroll);
	}
}
