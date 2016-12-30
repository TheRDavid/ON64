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
sprite_t* fx_sprite_Whirl(sprite_t* sprite, int deg, int freeOriginal);

/**
 *  adds offset to the color values (r,g,b,a)
 *  CAREFUL! 
 *  - Loss in Information when values reach 0 or 255!
 *  - Makes completely invisible pixels gray / white!
 *  Don't want all that? - Use fx_sprite_Fade_Alpha() instead
 */ 
void fx_sprite_Fade(sprite_t* sprite, int offset);

/**
 *  adds offset to the alpha values (only a)
 */ 
void fx_sprite_Fade_Alpha(sprite_t* sprite, int alphaOffset);

#endif



