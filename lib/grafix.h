/**
 * @file grafix.h
 * @brief 2D Graphics extention
 * @ingroup graphics
 */
#ifndef __GRAFIX_H
#define __GRAFIX_H

int bitDepth;
#define ZERO_X 9
#define ZERO_Y 0
#define MAX_X 308
#define MAX_Y 236
#define RES_X 299
#define RES_Y 236
#define UPPER_LAYER 0
#define LOWER_LAYER 1

uint16_t 	GFX_COLOR_WHITE, GFX_COLOR_BLACK, GFX_COLOR_LGRAY, GFX_COLOR_GRAY, GFX_COLOR_DGRAY,
			GFX_COLOR_RED, GFX_COLOR_LRED, GFX_COLOR_DRED,
			GFX_COLOR_LGREEN, GFX_COLOR_DGREEN, GFX_COLOR_GREEN, 
			GFX_COLOR_LBLUE, GFX_COLOR_DBLUE, GFX_COLOR_BLUE,
			GFX_COLOR_YELLOW, GFX_COLOR_BROWN, GFX_COLOR_ORANGE,
			GFX_COLOR_TRANS;
double preSin[361], preCos[361];			

/*
   Enum: gfx_interpolationMode

   BILINEAR - Use interpolation to add information
   NEAREST_NEIGHBOUR - Don't interpolate, just multiply pixels
*/
typedef enum
{
    BILINEAR,
    NEAREST_NEIGHBOUR
} gfx_interpolationMode;

/*
   Structure: gfx_shape_rectangle
   
	Stores all data necessary to draw a beautiful if 'highly' customizable rectangle

   Contains:

        - uint16_t x, y (coordinates)
        - uint16_t width, height (dimension)
		- uint16_t color
		- uint16_t fill (flag)

*/
typedef struct
{
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
	uint16_t color;
	uint16_t fill;
} gfx_shape_rectangle;

/*
   Structure: gfx_shape_circle
   
	Stores all data necessary to draw a beautiful if 'highly' customizable circle

   Contains:

        - uint16_t x, y (coordinates)
        - uint16_t width, height (dimension)
		- uint16_t color
		- uint16_t fill (flag)

*/
typedef struct
{
	uint16_t x;
	uint16_t y;
	uint16_t radius;
	uint16_t color;
	uint16_t fill;
} gfx_shape_circle;

/*
   Structure: gfx_shape_triangle
   
	Stores all data necessary to draw a beautiful if 'highly' customizable triangle

   Contains:

        - uint16_t x0, y0 first corner-coordinates
        - uint16_t x1, y1 second corner-coordinates
        - uint16_t x2, y2 third corner-coordinates
		- uint16_t color
		- uint16_t fill (flag)

*/
typedef struct
{
	uint16_t x0;
	uint16_t y0;
	uint16_t x1;
	uint16_t y1;
	uint16_t x2;
	uint16_t y2;
	uint16_t color;
	uint16_t fill;
} gfx_shape_triangle;

/*
   Function: gfx_init

   Initializes all the predefined colors and sets the global bit-depth (though 16 is strongly recommended)

   Parameters:

      int bpp - how many bits per pixel -> 16 probably ;) 
*/
void gfx_init(int bpp);

/*
   Function: gfx_finish

   Flush hardware-operations
*/
void gfx_finish();

 /*
   Function: gfx_load_sprite

   Load a sprite-file from the cartridge into RAM

   Parameters:

   char* name - name of the image-file

   Returns:

   A pointer to a sprite-instance that contains the image data of the file
*/
sprite_t* gfx_load_sprite(char* name);

 /*
   Function: gfx_load_sprite_into_buffer

   Load a sprite-file from the cartridge into a buffer

   Parameters:

   char* name - name of the image-file
   sprite_t **buffer - buffer to load the sprite into

   Returns:

   Flag whether or not the function succeeded
*/
int gfx_load_sprite_into_buffer(char *name, sprite_t **buffer);

 /*
   Function: gfx_copy_sprite

   Creates a deep copy of the original sprite

   Parameters:

   sprite_t* original - the sprite to copy from

   Returns:

   A copy of the original
*/
sprite_t* gfx_copy_sprite(sprite_t* original);

 /*
   Function: gfx_sprite_scale

   Scales by the given factor according to the given mode

   Parameters:

   sprite_t* sprite - the sprite to scale
   gfx_interpolationMode mode - scaling mode (BILINEAR or nearest neighbour)
   float factor - factor by which to scale the image
   int merged - is it a compressed sprite? (influences bilinear interpolation)
   int freeOriginal - flag for whether or not free the memory of the original sprite

   Returns:

   the new sprite (since it has a new array)
*/
sprite_t* gfx_sprite_scale(sprite_t* sprite, gfx_interpolationMode mode, float factor, int merged, int freeOriginal);

 /*
   Function: gfx_sprite_scale_2

   Scales by the given factor according to the given mode

   Parameters:

   sprite_t* source - the sprite containing the original data
   sprite_t* dest - buffer for the scaled data
   gfx_interpolationMode mode - scaling mode (BILINEAR or nearest neighbour)
   float factor - factor by which to scale the image
   int merged - is it a compressed sprite? (influences bilinear interpolation)
   int freeOriginal - flag for whether or not free the memory of the original sprite

*/
void gfx_sprite_scale_2(sprite_t* source, sprite_t* dest, gfx_interpolationMode mode, float factor, int merged, int freeOriginal);

 /*
   Function: gfx_sprite_rotate

   Rotates by the given number of degrees according to the given mode

   Parameters:

   sprite_t* sprite - the sprite to scale
   gfx_interpolationMode mode - interpolation mode (BILINEAR or nearest neighbour)
   int deg - number of degrees to rotate by
   int freeOriginal - flag for whether or not free the memory of the original sprite

   Returns:

   the new sprite (since it has a new array)
*/ 
sprite_t* gfx_sprite_rotate(sprite_t* sprite, gfx_interpolationMode mode, int deg, int freeOriginal);

 /*
   Function: gfx_sprite_rotate_2

   Scales by the given factor according to the given mode

   Parameters:

   sprite_t* source - the sprite containing the original data
   sprite_t* dest - buffer for the scaled data
   gfx_interpolationMode mode - scaling mode (BILINEAR or nearest neighbour)
   int deg - number of degrees to rotate by
   int freeOriginal - flag for whether or not free the memory of the original sprite

*/
void gfx_sprite_rotate_2(sprite_t* source, sprite_t* dest, gfx_interpolationMode mode, int deg, int freeOriginal);

 /*
   Function: gfx_sprite_vflip

   Flips the given sprite vertically

   Parameters:

   sprite_t* sprite - the sprite that is supposed to be flipped

*/
void gfx_sprite_vflip(sprite_t *sprite);

 /*
   Function: gfx_sprite_hflip

   Flips the given sprite horizontally

   Parameters:

   sprite_t* sprite - the sprite that is supposed to be flipped

*/
void gfx_sprite_hflip(sprite_t *sprite);

 /*
   Function: gfx_draw_circle

   Draws a circle

   Parameters:

   - display_context_t display - display reference
   - uint16_t x, y - coordinates
   - uint16_t radius
   - uint16_t color
   - int8_t fill - flag whether to fill the geometry or only draw

   TODO:

   - version with a cicle-struct as parameter

*/
void gfx_draw_circle(display_context_t display, uint16_t x, uint16_t y, uint16_t radius, uint16_t color, int8_t fill);

 /*
   Function: gfx_draw_rectangle

   Draws a rectangle

   Parameters:

   - display_context_t display - display reference
   - uint16_t x, y - coordinates
   - uint16_t width, height - dimension
   - uint16_t color
   - int8_t fill - flag whether to fill the geometry or only draw

   TODO:

   - version with a rectangle-struct as parameter

*/
void gfx_draw_rectangle(display_context_t display, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color, int8_t fill);

 /*
   Function: gfx_draw_triangle

   Draws a triangle

   Parameters:

   - display_context_t display - display reference
   - uint16_t x0, y0 - coordinates of first corner
   - uint16_t x1, y1 - coordinates of second corner
   - uint16_t x2, y2 - coordinates of third corner
   - uint16_t color
   - int8_t fill - flag whether to fill the geometry or only draw

   TODO:

   - version with a triangle-struct as parameter

*/
void gfx_draw_triangle(	display_context_t display, 
						uint16_t x0, uint16_t y0,
						uint16_t x1, uint16_t y1,
						uint16_t x2, uint16_t y2, 
						uint16_t color, int8_t fill);

 /*
   Function: gfx_draw_merged_sprite_stride

   Draws certain frame of a merged sprite-animation

   Parameters:

   - display_context_t display - display reference
   - sprite_t* sprite
   - uint16_t x, y - coordinates 
   - uint16_t offset - frame of animation to be drawn

*/
void gfx_draw_merged_sprite_stride(display_context_t display, sprite_t* sprite, uint16_t x, uint16_t y, uint16_t offset);

  /*
   Function: gfx_draw_merged_sprite

   Draws only one layer of a merged sprite

   Parameters:

   - display_context_t display - display reference
   - sprite_t* sprite
   - uint16_t x, y - coordinates 
   - uint8_t offset - either UPPER_LAYER (0) or LOWER_LAYER (1)

*/
 void gfx_draw_merged_sprite(display_context_t display, sprite_t* sprite, uint16_t x, uint16_t y, uint8_t layer);

   /*
   Function: gfx_draw_sprite_hardware

   Supposedly draws a sprite extra fast (not yet tested), only up to 4KB though!!

   Parameters:

   - display_context_t display - display reference
   - sprite_t* sprite
   - uint16_t x, y - coordinates 

*/
void gfx_draw_sprite_hardware(display_context_t display, sprite_t* sprite, int x, int y);


    /*
   Function: gfx_init_sin_cos

  Init precalculated sin and cos - values

*/
void gfx_init_sin_cos();

#endif



