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
#include <grafix.h>
#include <tools.h>
#include <sound.h>

double start;
int boxX, boxDir, boxWidth;
uint16_t boxColor;
int numDebugMessages = 1024;
char *debugMessages[1024];
display_context_t display;
int currentConsolePrint = 0;
int maxCharsPerLine = 35; // Text output max (per line)
ushort frames = 0;
char framesDisplay[30], bytesDisplay[30];
int displayFPS, displayGfxBytes;
int gfxBytes = 0;
int graphics_memory = 1024 * 1024 * 3.5f; // 3.5 MB

void tools_init(char *ver, display_context_t d, int showFPS, int showByteAllocation)
{
	displayGfxBytes = showByteAllocation;
	displayFPS = showFPS;
	consoleIndex = 0;
	boxDir = 5;
	boxX = 0;
	boxWidth = 40;
	display = d;
	for(int i = 0; i < numDebugMessages; i++)
		debugMessages[i] = "-";
    init_interrupts();
    display_init( RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE );
    gfx_init(2);
    controller_init();
    timer_init();
    
	// Filesystem
    dfs_init( DFS_DEFAULT_LOCATION );
    // Hardware Displayd
    rdp_init();

    if(dfs_init( DFS_DEFAULT_LOCATION ) != DFS_ESUCCESS)
    {
        tools_print("Filesystem failed to start!\n");
    }
    
	boxColor = graphics_make_color(150,50,50,100);
	char msg[32];
	sprintf(msg, "v%s", ver);
	tools_print(msg);

	if(displayFPS)
		new_timer(TIMER_TICKS(1000000), TF_CONTINUOUS, fpsUpdater);
}

void tools_update()
{
	start = timer_ticks();
	controller_scan();
	sound_update();
}

void tools_show(display_context_t display, int debug, int consoleScroll)
{
	if(debug)
	{
		graphics_set_color(GFX_COLOR_WHITE,GFX_COLOR_TRANS);			
		// Move Box
		if(boxX >= MAX_X - boxWidth) boxDir = -5; 
		else if(boxX <= ZERO_X) boxDir = 5;
		boxX += boxDir;
		graphics_draw_box(display, boxX + ZERO_X, 10 + ZERO_Y, boxWidth, 5, boxColor);
		graphics_draw_box(display, ZERO_X, MAX_Y - 65, RES_X, 65, boxColor);
		if(!(consoleIndex+consoleScroll < 0 || consoleIndex+consoleScroll == numDebugMessages - 3))
			consoleIndex += consoleScroll;		
		int rowY = 0; // additional height due to multiline debug messages
		for(int i = 0; i + rowY < 4; i++)
		{
			int idx = consoleIndex + i;
			char idxTxt[5];
			snprintf(idxTxt, 5, "%d", idx);
			graphics_draw_text(display, 5 + ZERO_X, MAX_Y - 65 + ZERO_Y + 5 + i * 15 + rowY * 15, idxTxt);
			// One line: graphics_draw_text(display, consoleX + 50, consoleY + 5 + i*15, debugMessages[consoleIndex + i]);
			// Multiple lines
			int conti = 1; // keep going...
			int charIndex = 0; // index of first character in line
			while(conti) // line by line ...
			{
				int charIdx = charIndex; // start at first index in line
				char output[maxCharsPerLine]; // output string for current line
				int k;
				// start at first index in line, go until you've either reached the end of the line or the end of the string
				for(k = charIndex; k < charIndex + maxCharsPerLine - 1 && k < strlen(debugMessages[consoleIndex + i] - 1); k++)
				{
					output[k - charIndex] = debugMessages[consoleIndex + i][k]; // add char to output line
					charIdx++;
				}
				// fill empty characters
				while(k < charIndex + maxCharsPerLine - 1)
				{
					output[k - charIndex] = ' ';
					k++;
				}
				output[maxCharsPerLine - 1] = '\0';
				
				// draw output
				graphics_draw_text(display, 32 + ZERO_X, MAX_Y - 65 + ZERO_Y + 5 + i * 15 + rowY * 15, output);
				// quit if you've reached the end of the message OR the max number of lines
				if(charIdx < strlen(debugMessages[consoleIndex + i]) && i + rowY < 3)
				{
					charIndex = charIdx;
					rowY++;
				} else 
				{
					conti = 0;
					charIndex = 0;
				}
			}
 
		}
	}

	if(displayFPS)
	{
		graphics_set_color(GFX_COLOR_RED, GFX_COLOR_BLACK);
		graphics_draw_text(display, ZERO_X , 10 +ZERO_Y, framesDisplay);
	}

	if(displayGfxBytes)
	{
		graphics_draw_text(display, ZERO_X + 70 , 10 +ZERO_Y, bytesDisplay);
	}

	gfx_finish();
	// Update Display
	display_show(display);
	frames++;
}

void tools_print(char msg[])
{
	if(currentConsolePrint < numDebugMessages)
	{
		debugMessages[currentConsolePrint] = malloc( (1 + strlen(msg)) * sizeof(char)); 
		strcpy(debugMessages[currentConsolePrint], msg); 
		currentConsolePrint++;
	}
}

void fpsUpdater()
{
	sprintf(framesDisplay, "FPS: %d", frames);
	frames = 0;
}

void tools_changeGfxBytes(int bytes)
{
	gfxBytes += bytes;
	sprintf(bytesDisplay, "Mem: %lf\%%", (float)((float)(gfxBytes)/graphics_memory*100));
}

void tools_free_sprite(sprite_t *sprite)
{
	tools_changeGfxBytes(-sizeof(uint16_t) 
								* (int) (sprite->width)
								* (int) (sprite->height)
								- sizeof(sprite_t));
	free(sprite);
}