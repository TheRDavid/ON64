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
	//char *soundFiles[5] =  {"rom://music.mod", "rom://pacman_intro.wav", "rom://pacman_x.wav", "rom://pacman_x_s.wav"};
	//int SOUND_MUSIC = 0;//, SOUND_EFFECT = 1; //, SOUND_EDD_DOING = 2;
	tools_init("2.4.3", disp, TRUE, TRUE, TRUE);
	//sound_init(soundFiles, 4, 4);								

	int debug = 1;
		
	char baseName[42] = {"harryPotterMovie_anim_merged_320x131_9x2/"}; // length + 1 for terminator
	char spriteName[64];
    sprite_t* p0 = gfx_load_sprite("harryPotterMovie_anim_merged_320x131_9x2/0.sprite");
    sprite_t* p1 = gfx_load_sprite("harryPotterMovie_anim_merged_320x131_9x2/0.sprite");
    p0->hslices = 9;
    p0->vslices = 2;
    p1->hslices = 9;
    p1->vslices = 2;
    sprite_t **play_sprites[2] = {&p0,&p1};
    int load = 1;
    //tools_push_to_sprite_queue("harryPotterMovie_anim_merged_320x131_9x2/1.sprite", 9, 2, play_sprites[load]);

	int frame = 1, maxFrame = 33, offset = 0, maxOffset = 35;
	// GAME LOOP
    while(1) 
	{
		/* Grab a render buffer */
		while( !(disp = display_lock()) );
		graphics_fill_screen(disp, 0);
		
		tools_update();
		struct controller_data keys = get_keys_down();

        if(offset == maxOffset)
        {
            offset = 0;
            if(frame == maxFrame)
            {
                frame = 0;
            } else frame++;
            snprintf(spriteName, 64, "%s%d.sprite", baseName, frame);
            tools_push_to_sprite_queue(spriteName, 9, 2, play_sprites[1 - load]);
            load = 1 - load;

        } else offset++;
		char degMessage[64];
		snprintf(degMessage, 64, "of: %d, fr: %d, ap: %d", offset, frame, sprite_loading_queue->append_index);
		graphics_draw_text( disp, ZERO_X, 160 + ZERO_Y, degMessage);

       // if(load == 1) gfx_draw_merged_sprite_stride(disp, p0, 0, 0, offset);
       // else gfx_draw_merged_sprite_stride(disp, p1, 0, 0, offset);

			
		if(keys.c[0].B) debug = 1 - debug;
		keys = get_keys_down();
		int consoleScroll = 0;
		if(keys.c[0].L)	consoleScroll = -1;
		if(keys.c[0].R) consoleScroll = 1;
		
		tools_show(disp, debug, consoleScroll);
	}
}
