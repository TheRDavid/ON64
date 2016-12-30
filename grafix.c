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
#include <graphics.h>
#include <inttypes.h>
#include <math.h>
#include <display.h>
	
#define MASK_R1 49152
#define MASK_R2 12288
#define MASK_G1 3584
#define MASK_G2 448
#define MASK_B1 48
#define MASK_B2 12
#define MASK_A1 2
#define MASK_A2 1

void grafix_init(int bpp)
{
	bitDepth = bpp;
	GFX_COLOR_BLACK = 1;
	GFX_COLOR_BLUE = 0x3F;
	GFX_COLOR_BROWN = 0xC2CB;
	GFX_COLOR_DBLUE = 0x1F;
	GFX_COLOR_DGRAY = 0x39CF;
	GFX_COLOR_DGREEN = 0x3C1;
	GFX_COLOR_DRED = 0x7801;
	GFX_COLOR_GRAY = 0x7BDF;
	GFX_COLOR_GREEN = 0x7C1;
	GFX_COLOR_LBLUE = 0x7BFF;
	GFX_COLOR_LGRAY = 0xBDEF;
	GFX_COLOR_LGREEN = 0x47D1;
	GFX_COLOR_LRED = 0xFA11;
	GFX_COLOR_ORANGE = 0xFDCF;
	GFX_COLOR_RED = 0xF801;
	GFX_COLOR_TRANS = 0x0;
	GFX_COLOR_WHITE = 0xFFFF;
	GFX_COLOR_YELLOW = 0xFFC1;
	initSinCos();
}

int hardwareUsed = 0;
int textureSlot = 0;
void gfx_finish()
{
	if(hardwareUsed) rdp_detach_display();
	hardwareUsed = 0;
}

sprite_t* gfx_load_Sprite(const char *const name)
{
	int fp = dfs_open(name);
	sprite_t *newSprite = malloc( dfs_size( fp ) );
	dfs_read( newSprite, 1, dfs_size( fp ), fp );
	newSprite->bitdepth = bitDepth;
	dfs_close( fp );
	return newSprite;
}

sprite_t* gfx_sprite_Scale(sprite_t* sprite, gfx_scaleMode mode, float factor, int freeOriginal)
{	
	uint16_t newWidth = (uint16_t) round(sprite->width * factor);
	uint16_t newHeight = (uint16_t) round(sprite->height * factor);
	
	sprite_t* newSprite = malloc  (sizeof(uint16_t) 
								* (int) (newWidth)
								* (int) (newHeight)
								+ sizeof(sprite_t));
	newSprite->width = newWidth; 
	newSprite->height = newHeight;
	newSprite->bitdepth = sprite->bitdepth;
	newSprite->hslices = sprite->hslices;
	newSprite->vslices = sprite->vslices;
	uint16_t *newData = (uint16_t *) newSprite->data;
	uint16_t *oldData = (uint16_t *) sprite->data;
	switch(mode)
	{
		case NEAREST_NEIGHBOUR:
			tools_print("nnscaling");
			for(int newRow = 0; newRow < newHeight; newRow++)
			{
				int indexByRow = newRow * newWidth;
				int oldRow = round(newRow / factor);
				int oldIndexByRow = oldRow * sprite->width;
				for(int newCol = 0; newCol < newWidth; newCol++)
				{
					int newIndex = indexByRow + newCol;
					int oldCol = round((double) newCol / factor);
					int oldIndex = oldIndexByRow + oldCol;
					
					// Check for Border (top and bottom)
					oldIndex = oldIndex < 0 ? oldIndex + sprite->width : oldIndex;
					oldIndex = oldIndex > sprite->width * sprite->height - 1 ? oldIndex - sprite->width : oldIndex;
					
					newData[newIndex] = oldData[oldIndex];
				}
			}
			break;
		case BILINEAR:
			tools_print("biscaling");
			for(int newRow = 0; newRow < newHeight; newRow++)
			{
				int indexByRow = newRow * newWidth;
				double oldRow = (newRow) / factor;
				int upperRow = floor(oldRow);
				int lowerRow = ceil(oldRow);
				double lowerShare = fmod(oldRow, 1);
				double upperShare = (1 - lowerShare);
				
				for(int newCol = 0; newCol < newWidth; newCol++)
				{
					int newIndex = indexByRow + newCol;
					double oldCol = (newCol) / factor;
					int leftCol = floor(oldCol);
					int rightCol = ceil(oldCol);
					
					double rightShare = fmod(oldCol, 1);
					double leftShare = (1 - rightShare);
					
					int ulIndex = upperRow * sprite->width + leftCol;
					int urIndex = upperRow * sprite->width + rightCol;
					int llIndex = lowerRow * sprite->width + leftCol;
					int lrIndex = lowerRow * sprite->width + rightCol;
					
					// Check for Border (left and right)
					if(ulIndex / sprite->width != urIndex / sprite->width)
					{
						if(newIndex > newWidth / 2) // right border
						{
							urIndex = ulIndex;
							lrIndex = llIndex;
						} else  // left border
						{
							ulIndex = urIndex; 
							llIndex = lrIndex;
						}
					}
					// Check for Border (top and bottom)
					ulIndex = ulIndex >= 0 ? ulIndex : llIndex;
					urIndex = urIndex >= 0 ? urIndex : lrIndex;
					llIndex = llIndex < sprite->width * sprite->height ? llIndex : ulIndex;
					lrIndex = lrIndex < sprite->width * sprite->height ? lrIndex : urIndex;


					// MIXDEMCOLORS
					color_t color0;
					color0.a = (oldData[ulIndex]) << 7;
					color0.b = (oldData[ulIndex]) << 2;
					color0.g = (oldData[ulIndex]) >> 3;
					color0.r = (oldData[ulIndex]) >> 8;
					
					color_t color1;
					color1.a = (oldData[urIndex]) << 7;
					color1.b = (oldData[urIndex]) << 2;
					color1.g = (oldData[urIndex]) >> 3;
					color1.r = (oldData[urIndex]) >> 8;
					
					color_t color2;
					color2.a = (oldData[llIndex]) << 7;
					color2.b = (oldData[llIndex]) << 2;
					color2.g = (oldData[llIndex]) >> 3;
					color2.r = (oldData[llIndex]) >> 8;
					
					color_t color3;
					color3.a = (oldData[lrIndex]) << 7;
					color3.b = (oldData[lrIndex]) << 2;
					color3.g = (oldData[lrIndex]) >> 3;
					color3.r = (oldData[lrIndex]) >> 8;
					
					uint8_t a4 = (oldData[ulIndex + 1]) << 7;
					uint8_t a5 = (oldData[llIndex + 1]) << 7;
					uint8_t a6 = (oldData[ulIndex + sprite->width]) << 7;
					uint8_t a7 = (oldData[urIndex + sprite->width]) << 7;
					
					color_t colorUp;
					colorUp.r = (double) color0.r * leftShare + (double) color1.r * rightShare;
					colorUp.g = (double) color0.g * leftShare + (double) color1.g * rightShare;
					colorUp.b = (double) color0.b * leftShare + (double) color1.b * rightShare;
					
					color_t colorLow;
					colorLow.r = (double) color2.r * leftShare + (double) color3.r * rightShare;
					colorLow.g = (double) color2.g * leftShare + (double) color3.g * rightShare;
					colorLow.b = (double) color2.b * leftShare + (double) color3.b * rightShare;
					
					color_t colorNew;
					colorNew.r = (double) colorLow.r * lowerShare + (double) colorUp.r * upperShare;
					colorNew.g = (double) colorLow.g * lowerShare + (double) colorUp.g * upperShare;
					colorNew.b = (double) colorLow.b * lowerShare + (double) colorUp.b * upperShare;
					
					int numTrans = 0;
					if(color0.a == 0) numTrans++;
					if(color1.a == 0) numTrans++;
					if(color2.a == 0) numTrans++;
					if(color3.a == 0) numTrans++;
					if(a4 == 0) numTrans+=2;
					if(a5 == 0) numTrans+=2;
					if(a6 == 0) numTrans+=2;
					if(a7 == 0) numTrans+=2;
					
				/*	char msg[30];
					sprintf(msg, "numTrans: %d", numTrans);
					tools_print(msg);   */
					
					colorNew.a = 255;
					if(numTrans > 7) colorNew.a = 0;
					
					newData[newIndex] = graphics_make_color(colorNew.r, colorNew.g, colorNew.b, colorNew.a);			
				}
			}
		break;
		
		default:
			break;
	}
	if(freeOriginal) free(sprite);
	return newSprite;
}

sprite_t* gfx_sprite_Rotate(sprite_t* sprite, int deg, int freeOriginal)
{
	while(deg < 0) deg += 360;
	deg%=360;
	double sDeg = preSin[deg], cDeg = preCos[deg]; // gonna need those
			
	uint16_t *oldData = (uint16_t *)sprite->data;
	
	sprite_t* newSprite = malloc(sizeof(sprite_t) + sizeof(uint16_t) 
							* (int) (sprite->width)
							* (int) (sprite->height));
	
	newSprite->bitdepth = sprite->bitdepth;
	newSprite->width = sprite->width;
	newSprite->height = sprite->height;
	newSprite->hslices = sprite->hslices;
	newSprite->vslices = sprite->vslices;
	
	int widthDivBy2 = sprite->width / 2;
	int heightDivBy2 = sprite->height / 2;
	
	
	uint16_t *newData = (uint16_t *)newSprite->data;
	
	for(int row = 0; row < sprite->height; row++) // for every row in the old picture ...
	{
		for(int col = 0; col < sprite->width; col++) // ... and every col in each row ...
		{
			int xt = col - widthDivBy2, yt = row - heightDivBy2;
			
			int xn = (int) round(xt * cDeg - yt * sDeg) + widthDivBy2; // calculate the new COORDINATES
			int yn = (int) round(xt * sDeg + yt * cDeg) + heightDivBy2;
			
			int spriteIndex = yn * sprite->width + xn;
			if(spriteIndex >= 0 && spriteIndex < sprite->width * sprite->height)
			{
				int newIdx = row * sprite->width + col;
				
				// check if distance to center is bigger than radius
				int dx = abs(widthDivBy2 - xn);
				int dy = abs(heightDivBy2 - yn);
				int distance = sqrt(dx * dx + dy * dy);
				
				if(!(distance > widthDivBy2 && distance > heightDivBy2))
					newData[newIdx] = oldData[spriteIndex];
			} else newData[row * sprite->width + col] = 0;
			
		}
	}
	
	if(freeOriginal)
		free(sprite);
	return newSprite;
}

void gfx_sprite_VFlip(sprite_t *sprite)
{
	if(bitDepth == 2)
	{
		uint16_t *data = (uint16_t *)sprite->data;
		const int widthMax = sprite->width/2;
		for(int i0 = 0; i0 < sprite->height; i0++)
		{
			const int indexByRow = i0*sprite->width;
			for(int i1 = 0; i1 < widthMax; i1++)
			{
				int index = indexByRow + i1;
				uint16_t temp = data[index];
				int otherIndex = indexByRow + sprite->width - i1-1;
				data[index] = data[otherIndex];
				data[otherIndex] = temp;
			}
		}
	} else if(bitDepth == 4)
	{
		uint16_t *data = (uint16_t *)sprite->data;
		const int widthMax = sprite->width/2;
		for(int i0 = 0; i0 < sprite->height; i0++)
		{
			const int indexByRow = i0*sprite->width;
			for(int i1 = 0; i1 < widthMax; i1++)
			{
				int index = indexByRow + i1;
				uint16_t temp = data[index];
				int otherIndex = indexByRow + sprite->width - i1-1;
				data[index] = data[otherIndex];
				data[otherIndex] = temp;
			}
		}
	}
}

void gfx_sprite_HFlip(sprite_t *sprite)
{
	if(bitDepth == 2)
	{
		uint16_t *data = (uint16_t *)sprite->data;
		const int heightMax = sprite->height/2;
		for(int i0 = 0; i0 < heightMax; i0++)
		{
			const int indexByRow = i0*sprite->width;
			for(int i1 = 0; i1 < sprite->width; i1++)
			{
				int index = indexByRow + i1;
				uint16_t temp = data[index];
				int otherIndex = sprite->height * sprite->width - indexByRow + i1 -(sprite->width-1) - 1;
				char msg[120];
				sprintf(msg, "%d<->%d", index, otherIndex);
				//tools_print(msg);
				data[index] = data[otherIndex];
				data[otherIndex] = temp;
			}
		}
	} else if(bitDepth == 4)
	{
		uint16_t *data = (uint16_t *)sprite->data;
		const int heightMax = sprite->height/2;
		for(int i0 = 0; i0 < heightMax; i0++)
		{
			const int indexByRow = i0*sprite->width;
			for(int i1 = 0; i1 < sprite->width; i1++)
			{
				int index = indexByRow + i1;
				uint16_t temp = data[index];
				int otherIndex = sprite->height * sprite->width - indexByRow + i1 -(sprite->width-1) - 1;
				char msg[120];
				sprintf(msg, "%d<->%d", index, otherIndex);
				//tools_print(msg);
				data[index] = data[otherIndex];
				data[otherIndex] = temp;
			}
		}
	}
}

void gfx_draw_sprite_Hardware(sprite_t* sprite, int x, int y, display_context_t display)
{
	if(1)
	{
		rdp_sync(SYNC_PIPE);
		hardwareUsed = 1;
		rdp_set_default_clipping();
		rdp_enable_texture_copy();
		rdp_attach_display(display);
	}
	rdp_sync(SYNC_PIPE);
	rdp_load_texture(0,0, MIRROR_DISABLED, sprite);
	rdp_draw_sprite(0, x, y);
}

void gfx_drawCircle(display_context_t display, uint8_t x0, uint8_t y0, uint8_t radius, uint16_t color, int8_t fill)
{
	int x = radius;
    int y = 0;
    int err = 0;

	if(fill)
	{
		while (x >= y)
		{
			graphics_draw_line_trans(display, x0 - x, y0 + y, x0 + x, y0 + y, color);
			graphics_draw_line_trans(display, x0 - y, y0 + x, x0 + y, y0 + x, color);
			graphics_draw_line_trans(display, x0 - x, y0 - y, x0 + x, y0 - y, color);
			graphics_draw_line_trans(display, x0 - y, y0 - x, x0 + y, y0 - x, color);

			y += 1;
			err += 1 + 2*y;
			if (2*(err-x) + 1 > 0)
			{
				x -= 1;
				err += 1 - 2*x;
			}
		}
	} else
	{
		while (x >= y)
		{
			graphics_draw_pixel_trans(display, x0 + x, y0 + y, color);
			graphics_draw_pixel_trans(display, x0 + y, y0 + x, color);
			graphics_draw_pixel_trans(display, x0 - y, y0 + x, color);
			graphics_draw_pixel_trans(display, x0 - x, y0 + y, color);
			graphics_draw_pixel_trans(display, x0 - x, y0 - y, color);
			graphics_draw_pixel_trans(display, x0 - y, y0 - x, color);
			graphics_draw_pixel_trans(display, x0 + y, y0 - x, color);
			graphics_draw_pixel_trans(display, x0 + x, y0 - y, color);

			y += 1;
			err += 1 + 2*y;
			if (2*(err-x) + 1 > 0)
			{
				x -= 1;
				err += 1 - 2*x;
			}
		}
	}
}

void gfx_drawRectangle(display_context_t display, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t color, int8_t fill)
{
	if(fill)
	{
		graphics_draw_box(display, x, y, width, height, color);
	}
	else
	{
		graphics_draw_line_trans(display, x, y, x + width, y, color);
		graphics_draw_line_trans(display, x, y, x, y + height, color);
		graphics_draw_line_trans(display, x + width, y, x + width, y + height, color);
		graphics_draw_line_trans(display, x , y + height, x + width, y + height, color);
	}
}

void gfx_drawTriangle(	display_context_t disp, uint8_t x_0, uint8_t y_0,
						uint8_t x_1, uint8_t y_1,
						uint8_t x_2, uint8_t y_2, 
						uint16_t color, int8_t fill)
{
	if(fill)
	{	
		// top to bottom
		double ax, ay, bx, by, cx, cy;	
		if(y_0 <= y_1 && y_0 <= y_2)
		{
			ax = x_0;
			ay = y_0;
			if(y_1 <= y_2)
			{
				bx = x_1;
				by = y_1;
				cx = x_2;
				cy = y_2;
			} else
			{
				bx = x_2;
				by = y_2;
				cx = x_1;
				cy = y_1;
			}
		} else if(y_1 <= y_2)
		{
			ax = x_1;
			ay = y_1;
			if(y_0 <= y_2)
			{
				bx = x_0;
				by = y_0;
				cx = x_2;
				cy = y_2;
			} else
			{
				bx = x_2;
				by = y_2;
				cx = x_0;
				cy = y_0;
			}
		} else
		{
			ax = x_2;
			ay = y_2;
			if(y_0 <= y_1)
			{
				bx = x_0;
				by = y_0;
				cx = x_1;
				cy = y_1;
			} else
			{
				bx = x_1;
				by = y_1;
				cx = x_0;
				cy = y_0;
			}
		}
		double dx1, dx2, dx3;
		if(by - ay > 0) dx1 = (bx - ax) / (by - ay); else dx1 = 0;
		if(cy - ay > 0) dx2 = (cx - ax) / (cy - ay); else dx2 = 0;
		if(cy - by > 0) dx3 = (cx - bx) / (cy - by); else dx3 = 0;
		
		double sx = ax, sy = ay, ex = ax, ey = ay;
		if(dx1 > dx2)
		{
			for(; sy <= by; sy++, ey++, sx += dx2, ex += dx1)
				graphics_draw_line_trans(disp, sx, sy, ex, sy, color);
			ex = bx; ey = by;
			for(; sy <= cy; sy++, ey++, sx += dx2, ex += dx3)
				graphics_draw_line_trans(disp, sx, sy, ex, sy, color);
		} else 
		{
			for(; sy <= by; sy++, ey++, sx += dx1, ex += dx2)
				graphics_draw_line_trans(disp, sx, sy, ex, sy, color);
			sx = bx; sy = by;
			for(; sy <= cy; sy++, ey++, sx += dx3, ex += dx2)
				graphics_draw_line_trans(disp, sx, sy, ex, sy, color);
		}
	} else
	{
		graphics_draw_line_trans(disp, x_0, y_0, x_1, y_1, color);
		graphics_draw_line_trans(disp, x_0, y_0, x_2, y_2, color);
		graphics_draw_line_trans(disp, x_2, y_2, x_1, y_1, color);
	}
}

void gfx_drawMergedSprite_stride(display_context_t display, sprite_t* sprite, uint8_t x, uint8_t y, uint8_t offset)
{
	int twidth = sprite->width / sprite->hslices;
	int theight = sprite->height / sprite->vslices;
	int numTiles = sprite->vslices * sprite->hslices;
	
	uint16_t *sp_data = (uint16_t *)sprite->data;
	
	if(offset < numTiles) // UPPER
	{
		// position inside sprite
		int tx = (offset % sprite->hslices) * twidth;
		int ty = (offset / sprite->hslices) * theight;

		for( int yp = ty; yp < ty + theight; yp++ ) // loop over tile
		{
			const register int idxByRow = yp * sprite->width;

			for( int xp = tx; xp < tx + twidth; xp++ )
			{
				uint16_t compressedPixel = (sp_data[xp + idxByRow]);
				
				uint16_t uncompressedPixel = (compressedPixel & MASK_R1) 
											| ((compressedPixel & MASK_G1) >> 1)
											| (compressedPixel & MASK_B1)
											| ((compressedPixel & MASK_A1) >> 1);
											
				graphics_draw_pixel(display, x + xp - tx, y + yp - ty, uncompressedPixel);
			}
		}
		tools_print("UP");
	} else
	{
		offset -= numTiles;
		// position inside sprite
		int tx = (offset % sprite->hslices) * twidth;
		int ty = (offset / sprite->hslices) * theight;

		for( int yp = ty; yp < ty + theight; yp++ ) // loop over tile
		{
			const register int idxByRow = yp * sprite->width;

			for( int xp = tx; xp < tx + twidth; xp++ )
			{
				uint16_t compressedPixel = (sp_data[xp + idxByRow]);
				
				uint16_t uncompressedPixel = ((compressedPixel & MASK_R2)  << 2)
											| ((compressedPixel & MASK_G2) << 2)
											| ((compressedPixel & MASK_B2) << 2)
											| ((compressedPixel & MASK_A2));
											
				graphics_draw_pixel(display, x + xp - tx, y + yp - ty, uncompressedPixel);
			}
		}
		tools_print("UP");
	}
}

void gfx_drawMergedSprite(display_context_t display, sprite_t* sprite, uint8_t x, uint8_t y, uint8_t layer)
{
	char msg[32];
	uint16_t *data = (uint16_t *)sprite->data;
	if(layer == UPPER_LAYER)
	{
		tools_print("UPPER LAYER");
		for(int y0 = 0; y0 < sprite->height; y0++)
		{
			const register int idxByRow = y0 * sprite->width, yCoord = y0 + y;
			for(int x0 = 0; x0 < sprite->width; x0++)
			{
				uint16_t compressedPixel = data[idxByRow + x0];
				
				uint16_t uncompressedPixel = 	  (compressedPixel & MASK_R1)
												| ((compressedPixel & MASK_G1) >> 1)
												| (compressedPixel & MASK_B1)
												| ((compressedPixel & MASK_A1) >> 1);
				if(y0 == 100 && x0 == 100)
				{
					sprintf(msg, "compressed: %u",  (unsigned int)compressedPixel);
					tools_print(msg);
					sprintf(msg, "uncompressed: %u",  (unsigned int)uncompressedPixel);
					tools_print(msg);
				}
				graphics_draw_pixel_trans(display, x + x0, yCoord, uncompressedPixel);
			}
		}
	} else
	{
		tools_print("LOWER LAYER");
		for(int y0 = 0; y0 < sprite->height; y0++)
		{
			const register int idxByRow = y0 * sprite->width, yCoord = y0 + y;
			for(int x0 = 0; x0 < sprite->width; x0++)
			{
				uint16_t compressedPixel = data[idxByRow + x0];
				uint16_t uncompressedPixel = 	  ((compressedPixel & MASK_R2) << 2)
												| ((compressedPixel & MASK_G2) << 2)
												| ((compressedPixel & MASK_B2) << 2)
												| (compressedPixel & MASK_A2);
				if(y0 == 100 && x0 == 100)
				{
					sprintf(msg, "compressed: %u",  (unsigned int)compressedPixel);
					tools_print(msg);
					sprintf(msg, "uncompressed: %u",  (unsigned int)uncompressedPixel);
					tools_print(msg);
				}
				graphics_draw_pixel_trans(display, x + x0, yCoord, uncompressedPixel);
			}
		}
	}
}

void initSinCos()
{
	preSin[0] = 0.0;
	preCos[0] = 1.0;
	preSin[1] = 0.017452413751550336;
	preCos[1] = 0.9998476950287202;
	preSin[2] = 0.034899511324350284;
	preCos[2] = 0.9993908265084894;
	preSin[3] = 0.05233598001910283;
	preCos[3] = 0.9986295335085179;
	preSin[4] = 0.06975650293436964;
	preCos[4] = 0.9975640482186431;
	preSin[5] = 0.08715577732981969;
	preCos[5] = 0.9961946950661978;
	preSin[6] = 0.10452851062437554;
	preCos[6] = 0.9945218903908801;
	preSin[7] = 0.12186939607988134;
	preCos[7] = 0.9925461451736766;
	preSin[8] = 0.13917315891357085;
	preCos[8] = 0.9902680605967346;
	preSin[9] = 0.15643452822836626;
	preCos[9] = 0.9876883305871182;
	preSin[10] = 0.17364824604067433;
	preCos[10] = 0.9848077409560698;
	preSin[11] = 0.19080906888202018;
	preCos[11] = 0.9816271691596441;
	preSin[12] = 0.2079117839717943;
	preCos[12] = 0.9781475809332997;
	preSin[13] = 0.22495115245112815;
	preCos[13] = 0.9743700421353837;
	preSin[14] = 0.24192199858746039;
	preCos[14] = 0.97029570059825;
	preSin[15] = 0.2588191385000116;
	preCos[15] = 0.9659258012632812;
	preSin[16] = 0.27563746832892594;
	preCos[16] = 0.9612616636760359;
	preSin[17] = 0.29237183611900064;
	preCos[17] = 0.9563047157911563;
	preSin[18] = 0.30901711606411697;
	preCos[18] = 0.951056476755937;
	preSin[19] = 0.32556829468272475;
	preCos[19] = 0.94551852731577;
	preSin[20] = 0.34202027380860056;
	preCos[20] = 0.9396925732939949;
	preSin[21] = 0.3583680981815095;
	preCos[21] = 0.9335803694410931;
	preSin[22] = 0.37460673227329;
	preCos[22] = 0.927183798464753;
	preSin[23] = 0.3907312850822473;
	preCos[23] = 0.9205047869826509;
	preSin[24] = 0.4067368170790748;
	preCos[24] = 0.9135453801713319;
	preSin[25] = 0.42261842580223163;
	preCos[25] = 0.906307710533483;
	preSin[26] = 0.4383713277844177;
	preCos[26] = 0.8987939580218186;
	preSin[27] = 0.45399067074806065;
	preCos[27] = 0.8910064370551574;
	preSin[28] = 0.46947175021909743;
	preCos[28] = 0.8829474931988976;
	preSin[29] = 0.48480982371360665;
	preCos[29] = 0.8746195943556156;
	preSin[30] = 0.5000001674757917;
	preCos[30] = 0.8660253070922236;
	preSin[31] = 0.5150382581195195;
	preCos[31] = 0.8571671906187329;
	preSin[32] = 0.529919462754628;
	preCos[32] = 0.8480479721064408;
	preSin[33] = 0.5446392484114553;
	preCos[33] = 0.8386704293641246;
	preSin[34] = 0.5591931312902388;
	preCos[34] = 0.8290374188888084;
	preSin[35] = 0.5735766293016701;
	preCos[35] = 0.8191519091834765;
	preSin[36] = 0.5877854593224391;
	preCos[36] = 0.8090168439588321;
	preSin[37] = 0.6018152437807011;
	preCos[37] = 0.7986353437916301;
	preSin[38] = 0.615661709058233;
	preCos[38] = 0.788010570994765;
	preSin[39] = 0.6293205910562423;
	preCos[39] = 0.7771457994949351;
	preSin[40] = 0.6427878224278412;
	preCos[40] = 0.7660442646077799;
	preSin[41] = 0.6560592539452348;
	preCos[41] = 0.7547093846725519;
	preSin[42] = 0.6691308429923912;
	preCos[42] = 0.743144612411536;
	preSin[43] = 0.6819986078273724;
	preCos[43] = 0.7313534705746092;
	preSin[44] = 0.6946585859192356;
	preCos[44] = 0.7193395922710553;
	preSin[45] = 0.707107007375064;
	preCos[45] = 0.7071065549979587;
	preSin[46] = 0.719340036684183;
	preCos[46] = 0.6946581257159512;
	preSin[47] = 0.7313539475386698;
	preCos[47] = 0.6819980963460269;
	preSin[48] = 0.7431450803763617;
	preCos[48] = 0.6691303232647665;
	preSin[49] = 0.7547098043914472;
	preCos[49] = 0.6560587711138565;
	preSin[50] = 0.7660446758361704;
	preCos[50] = 0.6427873323448873;
	preSin[51] = 0.7771462021075567;
	preCos[51] = 0.6293200938709972;
	preSin[52] = 0.7880110015652736;
	preCos[52] = 0.6156611579530533;
	preSin[53] = 0.7986357288074858;
	preCos[53] = 0.6018147328473572;
	preSin[54] = 0.8090172199990211;
	preCos[54] = 0.587784941747452;
	preSin[55] = 0.819152276133453;
	preCos[55] = 0.5735761052426985;
	preSin[56] = 0.8290378099673023;
	preCos[56] = 0.5591925514924347;
	preSin[57] = 0.8386708102641763;
	preCos[57] = 0.5446386618766885;
	preSin[58] = 0.8480483427120243;
	preCos[58] = 0.5299188696615633;
	preSin[59] = 0.8571675508169581;
	preCos[59] = 0.5150376586488192;
	preSin[60] = 0.8660255971687716;
	preCos[60] = 0.49999966504836046;
	preSin[61] = 0.8746199045163212;
	preCos[61] = 0.4848092641687669;
	preSin[62] = 0.8829477935469487;
	preCos[62] = 0.4694711853464223;
	preSin[63] = 0.8910067274990648;
	preCos[63] = 0.45399010071961615;
	preSin[64] = 0.8987942515375935;
	preCos[64] = 0.43837072598769294;
	preSin[65] = 0.9063079935017407;
	preCos[65] = 0.4226178189746011;
	preSin[66] = 0.9135456646275786;
	preCos[66] = 0.40673617817961033;
	preSin[67] = 0.9205050602452274;
	preCos[67] = 0.3907306413156387;
	preSin[68] = 0.9271840604504205;
	preCos[68] = 0.3746060838356353;
	preSin[69] = 0.9335805880295175;
	preCos[69] = 0.35836752873894745;
	preSin[70] = 0.9396927819109724;
	preCos[70] = 0.3420197006378691;
	preSin[71] = 0.9455187356004447;
	preCos[71] = 0.3255676897797695;
	preSin[72] = 0.9510566744518644;
	preCos[72] = 0.3090165076182186;
	preSin[73] = 0.9563049115514753;
	preCos[73] = 0.29237119581539645;
	preSin[74] = 0.9612618482316879;
	preCos[74] = 0.27563682470635065;
	preSin[75] = 0.9659259745580485;
	preCos[75] = 0.25881849175451943;
	preSin[76] = 0.9702958697891823;
	preCos[76] = 0.24192131999899127;
	preSin[77] = 0.9743702028095649;
	preCos[77] = 0.22495045649397413;
	preSin[78] = 0.978147707750057;
	preCos[78] = 0.2079111873452438;
	preSin[79] = 0.9816272855445054;
	preCos[79] = 0.1908084701330788;
	preSin[80] = 0.9848078494611434;
	preCos[80] = 0.17364763067694836;
	preSin[81] = 0.9876884306671316;
	preCos[81] = 0.1564338963469835;
	preSin[82] = 0.9902681517074833;
	preCos[82] = 0.13917251062564379;
	preSin[83] = 0.9925462267723464;
	preCos[83] = 0.12186873151049811;
	preSin[84] = 0.9945219619363697;
	preCos[84] = 0.10452782991258386;
	preSin[85] = 0.9961947553700791;
	preCos[85] = 0.08715508805083266;
	preSin[86] = 0.9975640975232014;
	preCos[86] = 0.06975579784305162;
	preSin[87] = 0.9986295650410228;
	preCos[87] = 0.052335378339871376;
	preSin[88] = 0.9993908479254614;
	preCos[88] = 0.034898898017376126;
	preSin[89] = 0.9998477059662888;
	preCos[89] = 0.017451787127677667;
	preSin[90] = 0.9999999999997954;
	preCos[90] = -6.397578377557251E-7;
	preSin[91] = 0.9998476836356351;
	preCos[91] = -0.017453066448476287;
	preSin[92] = 0.9993908032709563;
	preCos[92] = -0.03490017675360344;
	preSin[93] = 0.9986294980762684;
	preCos[93] = -0.05233665610201104;
	preSin[94] = 0.9975639999530497;
	preCos[94] = -0.06975719316079085;
	preSin[95] = 0.9961946334631071;
	preCos[95] = -0.08715648145322182;
	preSin[96] = 0.9945218157297115;
	preCos[96] = -0.10452922097517807;
	preSin[97] = 0.9925460708385816;
	preCos[97] = -0.12187000148885417;
	preSin[98] = 0.9902679736332637;
	preCos[98] = -0.13917377768915293;
	preSin[99] = 0.9876882305067005;
	preCos[99] = -0.156435160109685;
	preSin[100] = 0.9848076272754718;
	preCos[100] = -0.17364889075388698;
	preSin[101] = 0.9816270414012733;
	preCos[101] = -0.190809726140422;
	preSin[102] = 0.978147441723633;
	preCos[102] = -0.20791243890039865;
	preSin[103] = 0.9743698881647924;
	preCos[103] = -0.22495181936968195;
	preSin[104] = 0.9702955314068432;
	preCos[104] = -0.24192267717581117;
	preSin[105] = 0.9659256125412178;
	preCos[105] = -0.25881984281904125;
	preSin[106] = 0.961261495549263;
	preCos[106] = -0.2756380546557281;
	preSin[107] = 0.9563045374571644;
	preCos[107] = -0.29237241942228204;
	preSin[108] = 0.9510562790596203;
	preCos[108] = -0.3090177245098889;
	preSin[109] = 0.9455183190307083;
	preCos[109] = -0.3255688995855467;
	preSin[110] = 0.939692344290635;
	preCos[110] = -0.34202090298923377;
	preSin[111] = 0.9335801294918826;
	preCos[111] = -0.35836872326965086;
	preSin[112] = 0.9271835364786319;
	preCos[112] = -0.37460738071076144;
	preSin[113] = 0.9205045137196243;
	preCos[113] = -0.3907319288486647;
	preSin[114] = 0.9135450957146383;
	preCos[114] = -0.40673745597834027;
	preSin[115] = 0.9063074527548747;
	preCos[115] = -0.4226189786095397;
	preSin[116] = 0.898793638376636;
	preCos[116] = -0.43837198315322223;
	preSin[117] = 0.8910061466108851;
	preCos[117] = -0.4539912407763193;
	preSin[118] = 0.8829471368850177;
	preCos[118] = -0.4694724203470847;
	preSin[119] = 0.8746192841945521;
	preCos[119] = -0.484810383258248;
	preSin[120] = 0.8660250170153843;
	preCos[120] = -0.5000006699030547;
	preSin[121] = 0.8571668304200885;
	preCos[121] = -0.515038857589968;
	preSin[122] = 0.8480476646718297;
	preCos[122] = -0.5299199547522776;
	preSin[123] = 0.8386700484636627;
	preCos[123] = -0.5446398349459556;
	preSin[124] = 0.8290370944709884;
	preCos[124] = -0.5591936122588504;
	preSin[125] = 0.8191514738574339;
	preCos[125] = -0.5735772510108762;
	preSin[126] = 0.809016467918312;
	preCos[126] = -0.5877859768971856;
	preSin[127] = 0.7986348870334135;
	preCos[127] = -0.6018158499185003;
	preSin[128] = 0.7880101771201999;
	preCos[128] = -0.615662213194046;
	preSin[129] = 0.777145434392454;
	preCos[129] = -0.6293210419197534;
	preSin[130] = 0.7660438150659052;
	preCos[130] = -0.6427883581703026;
	preSin[131] = 0.7547090040575539;
	preCos[131] = -0.6560596917921837;
	preSin[132] = 0.7431441444463467;
	preCos[132] = -0.6691313627196885;
	preSin[133] = 0.7313530749108161;
	preCos[133] = -0.6819990321243088;
	preSin[134] = 0.7193390650478168;
	preCos[134] = -0.6946591318741394;
	preSin[135] = 0.7071061026205638;
	preCos[135] = -0.70710745975188;
	preSin[136] = 0.69465757976031;
	preCos[136] = -0.7193405639066579;
	preSin[137] = 0.6819976284564672;
	preCos[137] = -0.7313543838521476;
	preSin[138] = 0.6691299364215831;
	preCos[138] = -0.7431454286910792;
	preSin[139] = 0.6560582432979737;
	preCos[139] = -0.7547102632141535;
	preSin[140] = 0.6427868879215144;
	preCos[140] = -0.7660450487511649;
	preSin[141] = 0.629319550363946;
	preCos[141] = -0.7771466422302298;
	preSin[142] = 0.615660700785851;
	preCos[142] = -0.7880113587429276;
	preSin[143] = 0.601814126708919;
	preCos[143] = -0.7986361855648547;
	preSin[144] = 0.5877844241722243;
	preCos[144] = -0.8090175960388789;
	preSin[145] = 0.5735754835328835;
	preCos[145] = -0.819152711458626;
	preSin[146] = 0.5591920211088836;
	preCos[146] = -0.8290381677149502;
	preSin[147] = 0.544638225307742;
	preCos[147] = -0.8386710937749274;
	preSin[148] = 0.5299183271158808;
	preCos[148] = -0.8480486817316011;
	preSin[149] = 0.5150372124514253;
	preCos[149] = -0.8571678189189474;
	preSin[150] = 0.4999990077632568;
	preCos[150] = -0.8660259766518316;
	preSin[151] = 0.4848087046237287;
	preCos[151] = -0.8746202146766687;
	preSin[152] = 0.46947051521793665;
	preCos[152] = -0.8829481498598913;
	preSin[153] = 0.4539895306909858;
	preCos[153] = -0.8910070179426077;
	preSin[154] = 0.43837004383227557;
	preCos[154] = -0.8987945842463053;
	preSin[155] = 0.4226172391568724;
	preCos[155] = -0.9063082638746173;
	preSin[156] = 0.4067357026346858;
	preCos[156] = -0.9135458763533819;
	preSin[157] = 0.3907300524152318;
	preCos[157] = -0.9205053102180293;
	preSin[158] = 0.37460560119126796;
	preCos[158] = -0.927184255450948;
	preSin[159] = 0.3583668201818673;
	preCos[159] = -0.9335808600184224;
	preSin[160] = 0.3420190994619768;
	preCos[160] = -0.9396930007205643;
	preSin[161] = 0.32556697216203717;
	preCos[161] = -0.9455189826953467;
	preSin[162] = 0.30901589917219374;
	preCos[162] = -0.9510568721474024;
	preSin[163] = 0.2923704700113207;
	preCos[163] = -0.9563051334513265;
	preSin[164] = 0.2756362097314927;
	preCos[164] = -0.9612620245723101;
	preSin[165] = 0.25881798894312064;
	preCos[165] = -0.9659261092855078;
	preSin[166] = 0.24192069924455412;
	preCos[166] = -0.9702960245600443;
	preSin[167] = 0.22494994928694917;
	preCos[167] = -0.9743703199070665;
	preSin[168] = 0.20791044496322808;
	preCos[168] = -0.9781478655475319;
	preSin[169] = 0.19080784212929008;
	preCos[169] = -0.9816274076155188;
	preSin[170] = 0.1736468832401139;
	preCos[170] = -0.9848079812537032;
	preSin[171] = 0.1564332644655367;
	preCos[171] = -0.9876885307467408;
	preSin[172] = 0.13917175904462933;
	preCos[172] = -0.9902682573345586;
	preSin[173] = 0.12186809652124526;
	preCos[173] = -0.9925463047386195;
	preSin[174] = 0.10452731221560625;
	preCos[174] = -0.994522016348045;
	preSin[175] = 0.08715445072741215;
	preCos[175] = -0.9961948111280259;
	preSin[176] = 0.06975527856249947;
	preCos[176] = -0.9975641338343456;
	preSin[177] = 0.052334620412844084;
	preCos[177] = -0.998629604761567;
	preSin[178] = 0.034898258649241044;
	preCos[178] = -0.9993908702521005;
	preSin[179] = 0.017451028276131497;
	preCos[179] = -0.9998477192113335;
	preSin[180] = -1.2795156755111882E-6;
	preCos[180] = -0.9999999999991814;
	preSin[181] = -0.017453825299995455;
	preCos[181] = -0.9998476703890434;
	preSin[182] = -0.034900816121695676;
	preCos[182] = -0.9993907809430902;
	preSin[183] = -0.052337175937139366;
	preCos[183] = -0.9986294708323628;
	preSin[184] = -0.0697578313601642;
	preCos[184] = -0.9975639553251344;
	preSin[185] = -0.08715700002088018;
	preCos[185] = -0.9961945880937922;
	preSin[186] = -0.1045298572282831;
	preCos[186] = -0.9945217488561197;
	preSin[187] = -0.12187063647795739;
	preCos[187] = -0.9925459928710899;
	preSin[188] = -0.13917452926995208;
	preCos[188] = -0.9902678680046563;
	preSin[189] = -0.1564357919909397;
	preCos[189] = -0.9876881304258787;
	preSin[190] = -0.1736496381904528;
	preCos[190] = -0.9848074954813884;
	preSin[191] = -0.19081035414397643;
	preCos[191] = -0.9816269193290547;
	preSin[192] = -0.20791318128209269;
	preCos[192] = -0.9781472839246448;
	preSin[193] = -0.22495244273040876;
	preCos[193] = -0.9743697442499034;
	preSin[194] = -0.2419231822617086;
	preCos[194] = -0.9702954054742133;
	preSin[195] = -0.25882046077746956;
	preCos[195] = -0.9659254469589971;
	preSin[196] = -0.27563866963024763;
	preCos[196] = -0.9612613192074605;
	preSin[197] = -0.29237314522590546;
	preCos[197] = -0.9563043155558337;
	preSin[198] = -0.3090183329555343;
	preCos[198] = -0.9510560813629144;
	preSin[199] = -0.32556961720277533;
	preCos[199] = -0.9455180719343435;
	preSin[200] = -0.3420215041647061;
	preCos[200] = -0.9396921254798892;
	preSin[201] = -0.35836943182617664;
	preCos[201] = -0.9335798575015334;
	preSin[202] = -0.37460797388361927;
	preCos[202] = -0.9271832968204343;
	preSin[203] = -0.39073240801590003;
	preCos[203] = -0.9205043103245613;
	preSin[204] = -0.4067380404258922;
	preCos[204] = -0.913544835500976;
	preSin[205] = -0.42261955842674953;
	preCos[205] = -0.9063071823808853;
	preSin[206] = -0.43837255816340714;
	preCos[206] = -0.89879335792454;
	preSin[207] = -0.4539918108043922;
	preCos[207] = -0.8910058561662482;
	preSin[208] = -0.4694729852193398;
	preCos[208] = -0.8829468365361764;
	preSin[209] = -0.4848109428026909;
	preCos[209] = -0.8746189740331306;
	preSin[210] = -0.5000014304256082;
	preCos[210] = -0.8660245779262535;
	preSin[211] = -0.5150394059690606;
	preCos[211] = -0.8571665009197671;
	preSin[212] = -0.5299204972973094;
	preCos[212] = -0.8480473256512117;
	preSin[213] = -0.5446403714915808;
	preCos[213] = -0.8386697000258879;
	preSin[214] = -0.559194142641715;
	preCos[214] = -0.8290367367223224;
	preSin[215] = -0.5735777750693345;
	preCos[215] = -0.8191511069067244;
	preSin[216] = -0.5877864944716915;
	preCos[216] = -0.8090160918774608;
	preSin[217] = -0.6018163608513057;
	preCos[217] = -0.7986345020168432;
	preSin[218] = -0.6156627173296071;
	preCos[218] = -0.7880097832453125;
	preSin[219] = -0.6293217243903036;
	preCos[219] = -0.7771448817372567;
	preSin[220] = -0.6427888482527058;
	preCos[220] = -0.7660434038368583;
	preSin[221] = -0.65606017462305;
	preCos[221] = -0.7547085843380696;
	preSin[222] = -0.6691318381518426;
	preCos[222] = -0.7431437163641608;
	preSin[223] = -0.6819995000130311;
	preCos[223] = -0.7313526385964404;
	preSin[224] = -0.6946595920768021;
	preCos[224] = -0.7193386206340453;
	preSin[225] = -0.7071079121284066;
	preCos[225] = -0.7071056502428796;
	preSin[226] = -0.7193410083191419;
	preCos[226] = -0.694657119556404;
	preSin[227] = -0.7313548201653262;
	preCos[227] = -0.6819971605666284;
	preSin[228] = -0.7431460163049227;
	preCos[228] = -0.6691292838085354;
	preSin[229] = -0.7547106829324022;
	preCos[229] = -0.6560577604660333;
	preSin[230] = -0.7660454599789576;
	preCos[230] = -0.6427863978380588;
	preSin[231] = -0.777146894800809;
	preCos[231] = -0.6293192384644382;
	preSin[232] = -0.7880118994013319;
	preCos[232] = -0.6156600087726222;
	preSin[233] = -0.7986365705799958;
	preCos[233] = -0.6018136157750367;
	preSin[234] = -0.8090179720784056;
	preCos[234] = -0.5877839065967561;
	preSin[235] = -0.8191529416569193;
	preCos[235] = -0.5735751547746953;
	preSin[236] = -0.829038658783876;
	preCos[236] = -0.5591912930669004;
	preSin[237] = -0.8386715720630493;
	preCos[237] = -0.544637488806356;
	preSin[238] = -0.8480490207508307;
	preCos[238] = -0.5299177845699814;
	preSin[239] = -0.8571681484178654;
	preCos[239] = -0.5150366640714895;
	preSin[240] = -0.8660261773209929;
	preCos[240] = -0.4999986601929931;
	preSin[241] = -0.8746206404239026;
	preCos[241] = -0.48480793655269555;
	preSin[242] = -0.8829484502071524;
	preCos[242] = -0.46946995034484135;
	preSin[243] = -0.8910073083857859;
	preCos[243] = -0.4539889606621697;
	preSin[244] = -0.8987947601813413;
	preCos[244] = -0.43836968311068814;
	preSin[245] = -0.9063086350067606;
	preCos[245] = -0.42261644325816566;
	preSin[246] = -0.9135462335387315;
	preCos[246] = -0.4067349003800846;
	preSin[247] = -0.9205055601904544;
	preCos[247] = -0.3907294635146649;
	preSin[248] = -0.9271844951076276;
	preCos[248] = -0.37460500801779684;
	preSin[249] = -0.9335810038450212;
	preCos[249] = -0.3583664454991881;
	preSin[250] = -0.9396933010733092;
	preCos[250] = -0.3420182742456126;
	preSin[251] = -0.9455191909791754;
	preCos[251] = -0.3255663672587906;
	preSin[252] = -0.9510570698425513;
	preCos[252] = -0.3090152907260424;
	preSin[253] = -0.9563052507909972;
	preCos[253] = -0.29237008620850385;
	preSin[254] = -0.9612622666291585;
	preCos[254] = -0.275635365573746;
	preSin[255] = -0.9659263365729894;
	preCos[255] = -0.2588171406894912;
	preSin[256] = -0.9702961793305092;
	preCos[256] = -0.24192007849001795;
	preSin[257] = -0.9743704638203602;
	preCos[257] = -0.2249493259258541;
	preSin[258] = -0.978147948990077;
	preCos[258] = -0.20791005239407212;
	preSin[259] = -0.9816275751780874;
	preCos[259] = -0.190806980087177;
	preSin[260] = -0.9848080923454562;
	preCos[260] = -0.17364625320145366;
	preSin[261] = -0.9876886308259456;
	preCos[261] = -0.15643263258402587;
	preSin[262] = -0.9902683131895694;
	preCos[262] = -0.13917136161108987;
	preSin[263] = -0.9925464117599251;
	preCos[263] = -0.1218672248904408;
	preSin[264] = -0.9945221081410821;
	preCos[264] = -0.10452643884978514;
	preSin[265] = -0.9961948668855649;
	preCos[265] = -0.08715381340395596;
	preSin[266] = -0.9975641784606276;
	preCos[266] = -0.0697546403630119;
	preSin[267] = -0.9986296257654244;
	preCos[267] = -0.052334219623574624;
	preSin[268] = -0.9993909008985428;
	preCos[268] = -0.03489738100773411;
	preSin[269] = -0.9998477303755611;
	preCos[269] = -0.017450388615713;
	preSin[270] = -0.9999999999981581;
	preCos[270] = 1.9192735132661275E-6;
	preSin[271] = -0.9998476633840576;
	preCos[271] = 0.01745422657811685;
	preSin[272] = -0.9993907502936311;
	preCos[272] = 0.03490169376309726;
	preSin[273] = -0.998629424870704;
	preCos[273] = 0.052338052909969644;
	preSin[274] = -0.9975639106968109;
	preCos[274] = 0.06975846955950901;
	preSin[275] = -0.9961945323342144;
	preCos[275] = 0.087157637344158;
	preSin[276] = -0.9945217069041041;
	preCos[276] = 0.1045302563688961;
	preSin[277] = -0.9925458858467883;
	preCos[277] = 0.12187150810839398;
	preSin[278] = -0.9902677789664577;
	preCos[278] = 0.13917516280155362;
	preSin[279] = -0.9876880303446525;
	preCos[279] = 0.15643642387213041;
	preSin[280] = -0.984807425788892;
	preCos[280] = 0.17365003343234894;
	preSin[281] = -0.981626751763523;
	preCos[281] = 0.19081121618551355;
	preSin[282] = -0.9781471509103573;
	preCos[282] = 0.2079138070594415;
	preSin[283] = -0.9743696003346156;
	preCos[283] = 0.22495306609104349;
	preSin[284] = -0.9702952507017628;
	preCos[284] = 0.2419238030157497;
	preSin[285] = -0.9659253430841075;
	preCos[285] = 0.2588208484412515;
	preSin[286] = -0.9612610771477106;
	preCos[286] = 0.2756395137871623;
	preSin[287] = -0.9563041285076268;
	preCos[287] = 0.2923737570290268;
	preSin[288] = -0.9510558836658193;
	preCos[288] = 0.3090189414010533;
	preSin[289] = -0.9455179412703986;
	preCos[289] = 0.3255699966762711;
	preSin[290] = -0.9396918251243079;
	preCos[290] = 0.3420223293800379;
	preSin[291] = -0.9335796282316895;
	preCos[291] = 0.3583700290911343;
	preSin[292] = -0.9271830571618572;
	preCos[292] = 0.3746085670563238;
	preSin[293] = -0.9205040603502525;
	preCos[293] = 0.39073299691566726;
	preSin[294] = -0.9135446722609588;
	preCos[294] = 0.40673840706726644;
	preSin[295] = -0.9063068112460063;
	preCos[295] = 0.42262035432418055;
	preSin[296] = -0.898793077472076;
	preCos[296] = 0.4383731331734127;
	preSin[297] = -0.8910055657212466;
	preCos[297] = 0.4539923808322793;
	preSin[298] = -0.8829466481181656;
	preCos[298] = 0.4694733395805308;
	preSin[299] = -0.8746185482832568;
	preCos[299] = 0.48481171087226066;
	preSin[300] = -0.8660242580462423;
	preCos[300] = 0.5000019844715173;
	preSin[301] = -0.857166171419095;
	preCos[301] = 0.5150399543479425;
	preSin[302] = -0.8480469866302466;
	preCos[302] = 0.5299210398421244;
	preSin[303] = -0.8386694814402574;
	preCos[303] = 0.5446407080826127;
	preSin[304] = -0.8290362456508942;
	preCos[304] = 0.5591948706820102;
	preSin[305] = -0.8191507399556797;
	preCos[305] = 0.5735782991275581;
	preSin[306] = -0.8090157158362785;
	preCos[306] = 0.587787012045957;
	preSin[307] = -0.7986342604842467;
	preCos[307] = 0.6018166813746362;
	preSin[308] = -0.7880092425845295;
	preCos[308] = 0.6156634093409776;
	preSin[309] = -0.7771444791235921;
	preCos[309] = 0.629322221574704;
	preSin[310] = -0.7660429926074979;
	preCos[310] = 0.6427893383348459;
	preSin[311] = -0.7547081646182764;
	preCos[311] = 0.6560606574536478;
	preSin[312] = -0.7431434478152251;
	preCos[312] = 0.669132136404537;
	preSin[313] = -0.7313520396802812;
	preCos[313] = 0.6820001422694077;
	preSin[314] = -0.7193381762199795;
	preCos[314] = 0.6946600522791805;
	preSin[315] = -0.707105197864906;
	preCos[315] = 0.7071083645046438;
	preSin[316] = -0.694656830856561;
	preCos[316] = 0.7193412871122573;
	preSin[317] = -0.6819965183078099;
	preCos[317] = 0.7313554190788668;
	preSin[318] = -0.66912880837491;
	preCos[318] = 0.7431464443854745;
	preSin[319] = -0.6560572776338244;
	preCos[319] = 0.754711102650342;
	preSin[320] = -0.6427859077543402;
	preCos[320] = 0.7660458712064369;
	preSin[321] = -0.6293187412784923;
	preCos[321] = 0.7771472974125653;
	preSin[322] = -0.6156595046357073;
	preCos[322] = 0.7880122932744866;
	preSin[323] = -0.6018131048409079;
	preCos[323] = 0.7986369555948098;
	preSin[324] = -0.5877833890210473;
	preCos[324] = 0.8090183481176012;
	preSin[325] = -0.5735746307150632;
	preCos[325] = 0.8191533086059526;
	preSin[326] = -0.5591907626828062;
	preCos[326] = 0.8290390165307189;
	preSin[327] = -0.544636952259533;
	preCos[327] = 0.8386719204989799;
	preSin[328] = -0.5299172420238651;
	preCos[328] = 0.8480493597697131;
	preSin[329] = -0.5150361156913429;
	preCos[329] = 0.8571684779164326;
	preSin[330] = -0.4999981061458561;
	preCos[330] = 0.8660264971988774;
	preSin[331] = -0.48480737700718657;
	preCos[331] = 0.8746209505834008;
	preSin[332] = -0.4694693854715539;
	preCos[332] = 0.882948750554052;
	preSin[333] = -0.45398839063316776;
	preCos[333] = 0.8910075988285994;
	preSin[334] = -0.43836910809960605;
	preCos[334] = 0.898795040631598;
	preSin[335] = -0.4226158634400265;
	preCos[335] = 0.9063089053787571;
	preSin[336] = -0.40673431593163833;
	preCos[336] = 0.913546493750385;
	preSin[337] = -0.3907288746139381;
	preCos[337] = 0.9205058101625027;
	preSin[338] = -0.3746044148441724;
	preCos[338] = 0.9271847347639278;
	preSin[339] = -0.3583658482333504;
	preCos[339] = 0.9335812331125724;
	preSin[340] = -0.34201767306938813;
	preCos[340] = 0.9396935198819885;
	preSin[341] = -0.3255657623554108;
	preCos[341] = 0.9455193992626171;
	preSin[342] = -0.30901468227976453;
	preCos[342] = 0.9510572675373109;
	preSin[343] = -0.29236947440466454;
	preCos[343] = 0.9563054378368556;
	preSin[344] = -0.2756347505986204;
	preCos[344] = 0.9612624429688472;
	preSin[345] = -0.2588165227304937;
	preCos[345] = 0.9659265021530861;
	preSin[346] = -0.2419194577353828;
	preCos[346] = 0.9702963341005769;
	preSin[347] = -0.22494870256466695;
	preCos[347] = 0.9743706077332551;
	preSin[348] = -0.20790942661621273;
	preCos[348] = 0.9781480820019623;
	preSin[349] = -0.19080635208320298;
	preCos[349] = 0.9816276972481476;
	preSin[350] = -0.17364562316272236;
	preCos[350] = 0.9848082034368062;
	preSin[351] = -0.15643200070245103;
	preCos[351] = 0.9876887309047463;
	preSin[352] = -0.13917072807914654;
	preCos[352] = 0.9902684022253362;
	preSin[353] = -0.1218665899010696;
	preCos[353] = 0.9925464897252343;
	preSin[354] = -0.10452580259645025;
	preCos[354] = 0.9945221750124871;
	preSin[355] = -0.0871531760804641;
	preCos[355] = 0.9961949226426963;
	preSin[356] = -0.0697540021634958;
	preCos[356] = 0.9975642230865014;
	preSin[357] = -0.05233358074243381;
	preCos[357] = 0.9986296592464472;
	preSin[358] = -0.03489674163956514;
	preCos[358] = 0.9993909232242113;
	preSin[359] = -0.017449748955287357;
	preCos[359] = 0.9998477415393793;
	preSin[360] = 2.5590313510202817E-6;
	preCos[360] = 0.9999999999967257;
}
