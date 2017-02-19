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

void fx_sprite_fade(sprite_t* sprite, int offset)
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
			sprintf(msg, "before: %u,%u,%u,%u",  
			(unsigned int) r, 
			(unsigned int) g, 
			(unsigned int) b, 
			(unsigned int) a);
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
 * 16-bit colors only leave 1 bit for the alpha channel, not much to fade there...
 */
void fx_sprite_fade_alpha(sprite_t* sprite, int alphaOffset)
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

sprite_t* fx_sprite_4_point_transform(sprite_t* sprite,
										int ax, int ay,
										int bx, int by,
										int cx, int cy,
										int dx, int dy,
										int freeOriginal)
{
	uint16_t *oldData = (uint16_t *) sprite->data;
	
	sprite_t* newSprite = malloc(sizeof(sprite_t) + sizeof(uint16_t) 
							* (int) (sprite->width)
							* (int) (sprite->height));
	int width = sprite->width;
	int height = sprite->height;
	newSprite->bitdepth = sprite->bitdepth;
	newSprite->width = sprite->width;
	newSprite->height = sprite->height;
	newSprite->hslices = sprite->hslices;
	newSprite->vslices = sprite->vslices;
	uint16_t *newData = (uint16_t *)newSprite->data;
	memset(newData, 0, sizeof(newData));
	for(int x = 0; x < width; x++)
	{
		float x_share = (float) x / width;
		float x_share_comp = 1 - x_share;

		float y_start = ay * x_share_comp + by * x_share;
		float y_end = dy * x_share_comp + cy * x_share;
		
		for(int y = 0; y < height; y++)
		{
			float y_share = (float) y / height;
			float y_share_comp = 1 - y_share;

			float x_start = ax * y_share_comp + dx * y_share;
			float x_end = bx * y_share_comp + cx * y_share;

			int x_new = (int) (x_start + x_share * (x_end - x_start));
			int y_new = (int) (y_start + y_share * (y_end - y_start));

			if(x_new >= 0 && x_new < width && y_new >= 0 && y_new < height)
				newData[y_new * width + x_new] = oldData[y * width + x];
		}
	}
	if(freeOriginal) free(sprite);
	return newSprite;
}
