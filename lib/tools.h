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

/*
   Structure: sprite_queue
   
	Loading-queue to load image files from the cartridge one by one. Would work GREAT if parallel operations (via threads or processes) were supported (not the case though).

   Contains:

        - char paths[SPRITE_LOADING_QUEUE_MAX][64] - file names to be loaded
        - sprite_t** sprites[SPRITE_LOADING_QUEUE_MAX] - buffers for the sprite-data
		- int append_index - current index to append to the queue
		- int hslices[SPRITE_LOADING_QUEUE_MAX] - in case any of the queued sprites should suppord animations
		- int vslices[SPRITE_LOADING_QUEUE_MAX] - in case any of the queued sprites should suppord animations

*/
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
 /*
   Function: tools_init

   Initializes all subsystems, must be called to make proper use of the library

   Parameters:

   char* ver - string for version id
   display_context_t d - reference to display
   int showFPS - flag for whether or not to display the framerate in debug-mode
   int showByteAllocation - flag for whether or not to display the memory-usage in debug-mode
   int showByteAllocation - flag for whether or not the debug-consoleshould scroll automatically

   Returns:

   A pointer to a sprite-instance that contains the image data of the file
*/
void tools_init(char *ver, display_context_t d, int showFPS, int showByteAllocation, int console_auto_scroll);


/*
   Function: tools_update

   Update all requiered subsystems
*/
void tools_update();

/**
 *  Draw the screen and if debugging - show performance
 * 
 *  display - display to draw on
 *  debug mode on or off
 */ 

 /*
   Function: tools_show

   Refresh the screen

   Parameters:

   display_context_t display - reference to display
   int debug - flag for whether or not to show the debug-interface
   int consoleScroll - how far should the console scroll (>0 -> down, <0 -> up)
*/
void tools_show(display_context_t display, int debug, int consoleScroll);

/*
   Function: tools_print

   Print a debug-message on the console

   TODO:

   	- implement ring-buffer so the console won't crash after too many outputs
*/
void tools_print(char *msg);

 /*
   Function: fpsUpdater

   Update display text to show frames per second
*/
void fpsUpdater();

 /*
   Function: tools_changeGfxBytes

   Keep track of memory usage
*/
void tools_changeGfxBytes(int bytes);

 /*
   Function: tools_free_sprite

   Free sprite & register freed memory

   Parameters:

    - sprite_t* sprite - reference to sprite that shall be freed

*/
void tools_free_sprite(sprite_t *sprite);

 /*
   Function: tools_sprite_queue_has_next

   Is there a sprite ready to pop from queue?

   Returns:

   1 if TRUE, 0 otherwise
*/
int tools_sprite_queue_has_next();

 /*
   Function: tools_push_to_sprite_queue

   Append a request to the loading queue

   Parameters:

    - char path[] - path to the file on the cartridge
    - int hslices - number of horizontal slices of animation-franes (if any)
    - int vslices - number of vertical slices of animation-franes (if any)
    - sprite_t** buffer - pointer to the buffer to write the data into

*/
void tools_push_to_sprite_queue(char path[], int hslices, int vslices, sprite_t** buffer);

/*
   Function: sprite_queue_load_next

   Load the next element in queue from the cardridge
*/
void sprite_queue_load_next();

 /*
   Function: sprite_queue_shift

   Shifts the queue by one after an element was loaded
*/
void sprite_queue_shift();

#endif
