/**
 * @file effects.h
 * @brief 2D Graphics extention
 * @ingroup graphics
 */
#ifndef __FX_H
#define __FX_H

/*
   Function: fx_sprite_fade

   Fades a sprite-image

   Parameters:

      sprite_t* sprite - Pointer to the sprite-instance
      int offset - Offset from current state (negative to fade out, positive to fade in)
      int merged (bool) - Flag for whether or not the sprite consists of two merged images
*/
void fx_sprite_fade(sprite_t* sprite, int offset, int merged);

 /*
   Function: fx_sprite_fade_alpha

   Fades only the Alpha-Channel of the given sprite. Currently irrelevant - only works for 32-bit.
   16-bit colors only leave 1 bit for the alpha-value, not much to fade there...

   Parameters:

      sprite_t* sprite - Pointer to the sprite-instance
      int alphaOffset - Offset from current state (negative to fade out, positive to fade in)
*/
void fx_sprite_fade_alpha(sprite_t* sprite, int alphaOffset);


/*
	Function: fx_sprite_4_point_transform

		Transforms the source-sprite by translating the corner-coordinates, thus distorting the whole image.
		The new corner-coordinates are assigned clockwise.

	Parameters:

		sprite_t* source - Pointer to the source-sprite, which contains the pixel-data
		sprite_t* dest - Pointer to the destination-sprite, into which the distorted data will be stored
		int ax - X - coordinate of the first (upper-left) corner
		int ay - Y - coordinate of the first (upper-left) corner
		int bx - X - coordinate of the second (upper-right) corner
		int by - Y - coordinate of the second (upper-right) corner
		int cx - X - coordinate of the third (lower-right) corner
		int cy - Y - coordinate of the third (lower-right) corner
		int dx - X - coordinate of the fourth (lower-left) corner
		int dy - Y - coordinate of the fourth (lower-left) corner

	Details:

		- Elaborate explanation: https://medium.com/@fulumbler/4-point-distortion-df458207ca55
		- Does Forward-Mapping (yeah, I know, sorry)
		- Is pretty cool though
*/
void fx_sprite_4_point_transform(sprite_t* source, sprite_t* dest,
										int ax, int ay,
										int bx, int by,
										int cx, int cy,
										int dx, int dy);
#endif



