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
	
	//sprite_t* bunnySprite = gfx_load_Sprite("bunny_01.sprite");
	//sprite_t* tmpSprite = gfx_load_Sprite("tmp.sprite");
	sprite_t* playSprite = gfx_load_Sprite("sAnim0.sprite");
	playSprite->hslices = 12;
	playSprite->vslices = 1;
	
	int offset = 0, maxOffset = 23, slice = 0, scroll = 0;
	
	//uint16_t textpix = gruesomeDeathSprite->data[gruesomeDeathSprite->width * 50 + 50];
	
	// GAME LOOP
	while(1) 
	{
		/* Grab a render buffer */
		while( !(disp = display_lock()) );
		graphics_fill_screen( disp, 0 );

		tools_update();
		//graphics_draw_sprite_trans(disp, 0, 0, playSprite);
		gfx_drawMergedSprite_stride(disp, playSprite, 0, 0, offset);
		
		if(offset < maxOffset)
		{
			offset++;
		} else
		{
			offset = 0;
			if(slice == 0)
			{
				slice = 1;
				free(playSprite);
				playSprite = gfx_load_Sprite("sAnim1.sprite");
				playSprite->hslices = 12;
				playSprite->vslices = 1;
			} else if(slice == 1)
			{
				slice = 2;
				free(playSprite);
				playSprite = gfx_load_Sprite("sAnim2.sprite");
				playSprite->hslices = 12;
				playSprite->vslices = 1;
			} else
			{
				slice = 0;
				free(playSprite);
				playSprite = gfx_load_Sprite("sAnim0.sprite");
				playSprite->hslices = 12;
				playSprite->vslices = 1;
			}
		}


        struct controller_data keys = get_keys_down();
		scroll = 0;
        if( keys.c[0].A )
        {
            /* Lazy switching */
            scroll = 1;
        } else if( keys.c[0].B )
        {
            /* Lazy switching */
            scroll = -1;
        }

		tools_show(disp, 1, scroll);
	}
}
