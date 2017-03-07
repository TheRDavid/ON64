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

void fx_sprite_fade(sprite_t* sprite, int offset, int merged)
{
	int idxMax = sprite->width * sprite->height;
	uint16_t *data = (uint16_t *)sprite->data;
	if(merged)
		for(int idx = 0; idx < idxMax; idx++)
		{
			int colorUb = (data[idx] >> 2) & 0b11;
			int colorUg = (data[idx] >> 7) & 0b111;
			int colorUr = (data[idx] >> 12) & 0b11;
			int colorLg = (data[idx] >> 4) & 0b111;
			int colorLr = (data[idx] >> 10) & 0b11;
			int colorLb = (data[idx] & 0b11);
			if(colorUb + offset >= 0 && colorUb + offset < 4) // max of 2 bits
					colorUb = colorUb + offset;

				if(colorUg + offset >= 0 && colorUg + offset < 8) // max of 3 bits
					colorUg = colorUg + offset;

				if(colorUr + offset >= 0 && colorUr + offset < 4) // max of 2 bits
					colorUr = colorUr + offset;

				if(colorLb + offset >= 0 && colorLb + offset < 4) // max of 2 bits
					colorLb = colorLb + offset;

				if(colorLg + offset >= 0 && colorLg + offset < 8) // max of 3 bits
					colorLg = colorLg + offset;

				if(colorLr + offset >= 0 && colorLr + offset < 4) // max of 2 bits
					colorLr = colorLr + offset;
			
			data[idx] =  	/*upper alpha*/	 (data[idx] & 0b1000000000000000)
						 	/*lower alpha*/ |(data[idx] & 0b0100000000000000)
							/*upper red*/ 	| colorUr << 12
							/*upper green*/ | colorUg << 7
							/*upper blue*/ 	| colorUb << 2
							/*upper red*/ 	| colorLr << 10
							/*upper green*/ | colorLg << 4
							/*upper blue*/ 	| colorLb;
		}
	else
	{
		offset *= 8;
		for(int idx = 0; idx < idxMax; idx++)
		{
			uint8_t a = (data[idx]) << 7;
			uint8_t b = (data[idx]) << 2;
			uint8_t g = (data[idx]) >> 3;
			uint8_t r = (data[idx]) >> 8;
			
			char msg[39];
			if(idx == 2000)
			{
				snprintf(msg, 39, "before: %u,%u,%u,%u",  
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
				snprintf(msg, 39, "after: %u,%u,%u,%u",  (unsigned int)r, (unsigned int)g, (unsigned int)b, (unsigned int)a);
				tools_print(msg);
			}
			
			data[idx] = graphics_make_color(r,g,b,a);
			
		}
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

uint16_t fx_sprite_4_point_transform(sprite_t* source, sprite_t* dest,
										int ax, int ay,
										int bx, int by,
										int cx, int cy,
										int dx, int dy)
{
	uint16_t deb = 56;
	uint16_t *oldData = (uint16_t *) source->data;
	
	int width = source->width;
	int height = source->height;
	uint16_t *newData = (uint16_t *) dest->data;
	int l = width * height - 1;
	for(int i = 0; i < l; i++) newData[i] = GFX_COLOR_TRANS;
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
			if(x_new == 50 && y_new == 50)
				deb = oldData[y * width + x];
		}
	}
	return deb;
}
