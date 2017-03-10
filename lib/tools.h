/**
 * @file tools.h
 * @brief Stuff
 * @ingroup misc
 */
#ifndef __TOOLS_H
#define __TOOLS_H
#define TRUE 1
#define FALSE 0
#define SPRITE_LOADING_QUEUE_MAX 16

typedef struct
{
    char paths[SPRITE_LOADING_QUEUE_MAX][64];
    sprite_t** sprites[SPRITE_LOADING_QUEUE_MAX];
    int append_index;
    int hslices[SPRITE_LOADING_QUEUE_MAX];
    int vslices[SPRITE_LOADING_QUEUE_MAX];
} sprite_queue;

sprite_queue* sprite_loading_queue;
int consoleIndex;

/**
 * INITIALIZE ALL THE THINGS
 * 
 */
void tools_init(char *ver, display_context_t d, int showFPS, int showByteAllocation, int console_auto_scroll);

/**
 * refresh start time, play audio (if need be) and check input 
 */
void tools_update();

/**
 *  Draw the screen and if debugging - show performance
 * 
 *  display - display to draw on
 *  debug mode on or off
 */ 
void tools_show(display_context_t display, int debug, int consoleScroll);

/**
 * Print a debug message on the screen
 * 
 */ 
void tools_print(char *msg);

/**
 * Update display text to show frames per second
 *
 */
void fpsUpdater();

/**
 * Keep track of memory usage
 *
 */
void tools_changeGfxBytes(int bytes);

/**
 * Free sprite & register freed memory
 *
 */
void tools_free_sprite(sprite_t *sprite);

/*
 * returns 1 if there is a sprite to ready to pop
 *
 */
int tools_sprite_queue_has_next();

/**
 * Append a request to the loading queue
 *
 */
void tools_push_to_sprite_queue(char path[], int hslices, int vslices, sprite_t** buffer);

/**
 * Loads the next element in the request queue
 *
 */
void sprite_queue_load_next();

/**
 * Returns the oldest element from the queue and removes it
 *
 */
void sprite_queue_shift();

#endif
