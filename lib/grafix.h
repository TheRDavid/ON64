/**
 * @file grafix.h
 * @brief 2D Graphics extention
 * @ingroup graphics
 */
#ifndef __GRAFIX_H
#define __GRAFIX_H

int bitDepth;
#define ZERO_X 13
#define ZERO_Y 0
#define MAX_X 305
#define MAX_Y 236
#define RES_X MAX_X - ZERO_X
#define RES_Y MAX_Y - ZERO_Y
#define UPPER_LAYER 0
#define LOWER_LAYER 1

uint16_t 	GFX_COLOR_WHITE, GFX_COLOR_BLACK, GFX_COLOR_LGRAY, GFX_COLOR_GRAY, GFX_COLOR_DGRAY,
			GFX_COLOR_RED, GFX_COLOR_LRED, GFX_COLOR_DRED,
			GFX_COLOR_LGREEN, GFX_COLOR_DGREEN, GFX_COLOR_GREEN, 
			GFX_COLOR_LBLUE, GFX_COLOR_DBLUE, GFX_COLOR_BLUE,
			GFX_COLOR_YELLOW, GFX_COLOR_BROWN, GFX_COLOR_ORANGE,
			GFX_COLOR_TRANS;
double preSin[361], preCos[361];			

typedef enum
{
    BILINEAR,
    NEAREST_NEIGHBOUR
} gfx_interpolationMode;

typedef struct
{
	uint8_t x;
	uint8_t y;
	uint8_t width;
	uint8_t height;
	uint16_t color;
	uint8_t fill;
} gfx_shape_rectangle;

typedef struct
{
	uint8_t x;
	uint8_t y;
	uint8_t radius;
	uint16_t color;
	uint8_t fill;
} gfx_shape_circle;

typedef struct
{
	uint8_t x0;
	uint8_t y0;
	uint8_t x1;
	uint8_t y1;
	uint8_t x2;
	uint8_t y2;
	uint16_t color;
	uint8_t fill;
} gfx_shape_triangle;

/**
 *  Sets the bit-depth
 */  
void gfx_init(int bpp);

/**
 * Flush hardware operations
 */ 
void gfx_finish();

/**
 * 	return a pointer to a sprite that was loaded from a file called name
 */ 
sprite_t* gfx_load_sprite(const char *const name);

/**
  * Returns a deep copy of the original  sprite
  */
sprite_t* gfx_copy_sprite(sprite_t* original);

/**
 * Scales by the given factor according to the given mode
 * 
 * sprite - the sprite to scale
 * mode - scaling mode (BILINEAR or nearest neighbour)
 * merged - is it a compressed sprite? (influences bilinear interpolation)
 * -> scaling merged images takes more time!
 * 
 * returns the new sprite (since it has a new array)
 */ 
sprite_t* gfx_sprite_scale(sprite_t* sprite, gfx_interpolationMode mode, float factor, int merged, int freeOriginal);

/**
 * Rotates by degree
 * 
 * returns the new sprite (since the array itself was transformed)
 */ 
sprite_t* gfx_sprite_rotate(sprite_t* sprite, gfx_interpolationMode mode, int deg, int freeOriginal);

/**
 *  Flips the given sprite vertically
 *  I currently don't bother checking whether or not the number of pixels is even,
 *  it SHOULD not matter, since the fractional digits should be discarded.
 * 
 *  sprite - the sprite to flip
 */ 
void gfx_sprite_vflip(sprite_t *sprite);

/**
 *  Flips the given sprite horizontally
 *  I currently don't bother checking whether or not the number of pixels is even,
 *  it SHOULD not matter, since the fractional digits should be discarded.
 * 
 *  sprite - the sprite to flip
 */ 
void gfx_sprite_hflip(sprite_t *sprite);

/**
 * Using Bresenham's circle algorithm
 */ 
void gfx_draw_circle(display_context_t display, uint8_t x, uint8_t y, uint8_t radius, uint16_t color, int8_t fill);

/**
 * If fill = 0: Draw Outline
 * Else use graphics_draw_box
 */ 
void gfx_draw_rectangle(display_context_t display, uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t color, int8_t fill);

/**
 * Just draw a damn triangle
 */ 
void gfx_draw_triangle(	display_context_t display, 
						uint8_t x0, uint8_t y0,
						uint8_t x1, uint8_t y1,
						uint8_t x2, uint8_t y2, 
						uint16_t color, int8_t fill);

/**
 * Draw part of a spritemap. Use either the upper or lower layer, allowing twice the number of tiles
 */ 
void gfx_draw_merged_sprite_stride(display_context_t display, sprite_t* sprite, uint8_t x, uint8_t y, uint8_t offset);

/**
 * Draw either the upper or lower layer of a merged sprite
 */ 
 void gfx_draw_merged_sprite(display_context_t display, sprite_t* sprite, uint8_t x, uint8_t y, uint8_t layer);

/**
 * Draw Textures faster.
 * CAREFUL! - Only up to 4KB !!!
 */ 
void gfx_draw_sprite_hardware(display_context_t display, sprite_t* sprite, int x, int y);
void gfx_draw_sprite_map_hardware(display_context_t display, sprite_t* sprite, int x, int y);

/**
 * Init precalculated sin and cos - values
 */ 
void gfx_init_sin_cos();

#endif



