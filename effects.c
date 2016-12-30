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

void fx_sprite_Fade(sprite_t* sprite, int offset)
{
	offset *= 8;
	int idxMax = sprite->width * sprite->height;
	uint16_t *data = (uint16_t *)sprite->data;
	for(int idx = 0; idx < idxMax; idx++)
	{
		
		uint8_t a = (data[idx]) << 7;
		uint8_t b = (data[idx]) << 2;
		uint8_t g = (data[idx]) >> 3;
		uint8_t r = (data[idx]) >> 8;
		
		char msg[39];
		if(idx == 2000)
		{
			sprintf(msg, "before: %u,%u,%u,%u",  (unsigned int)r, (unsigned int)g, (unsigned int)b, (unsigned int)a);
			tools_print(msg);
		}
		
		if(offset + r > 0 && offset + r < 256)
		r += offset ;
		if(offset + g > 0 && offset + g < 256)
		g += offset;
		if(offset + b > 0 && offset + b < 256)
		b += offset;
		
		if(idx == 2000)
		{
			sprintf(msg, "after: %u,%u,%u,%u",  (unsigned int)r, (unsigned int)g, (unsigned int)b, (unsigned int)a);
			tools_print(msg);
		}
		
		data[idx] = graphics_make_color(r,g,b,a);
		
	}
}
/**
 * Irrelevant for now - only works for 32 bit!
 * 16-bit colors only leave 1 bit for the alpha channel
 */
void fx_sprite_Fade_Alpha(sprite_t* sprite, int alphaOffset)
{
	int idxMax = sprite->width * sprite->height;
	uint16_t *data = (uint16_t *)sprite->data;
	for(int idx = 0; idx < idxMax; idx++)
	{
		int32_t a = (data[idx] & 0xFF);
		int32_t b = ((data[idx] >> 4) & 0xFF);
		int32_t g = ((data[idx] >> 8) & 0xFF);
		int32_t r = ((data[idx] >> 16) & 0xFF);
		
		a += alphaOffset;
		if(a < 0) a = 0;
		if(a > 15) a = 15;
		
		data[idx] = graphics_make_color(r,g,b,a);
		
	}
}
