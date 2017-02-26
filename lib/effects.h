/**
 * @file grafix.h
 * @brief 2D Graphics extention
 * @ingroup graphics
 */
#ifndef __FX_H
#define __FX_H

/**
 * 	returns the sprite after applying a badass whirl effect
 */ 
sprite_t* fx_sprite_whirl(sprite_t* sprite, int deg, int freeOriginal);

/**
 *  adds offset to the color values (r,g,b,a)
 *  CAREFUL! 
 *  - Loss in Information when values reach 0 or 255!
 *  - Makes completely invisible pixels gray / white!
 *  Don't want all that? - Use fx_sprite_Fade_Alpha() instead
 */ 
void fx_sprite_fade(sprite_t* sprite, int offset, int merged);

/**
 *  adds offset to the alpha values (only a)
 */ 
void fx_sprite_fade_alpha(sprite_t* sprite, int alphaOffset);

/**
 * Uses the new positions of the four corners of the sprite to distort it.
 * Point a is at the top left, the others follow in clockwise order.
 * 
 * updates dest, no changes will be applied to source
 */ 
uint16_t fx_sprite_4_point_transform(sprite_t* source, sprite_t* dest,
										int ax, int ay,
										int bx, int by,
										int cx, int cy,
										int dx, int dy);
#endif



