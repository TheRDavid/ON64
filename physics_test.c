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
#include <sound.h>

int main(void)
{
	// INITIALIZE ALL THE THINGS
	static display_context_t disp = 0; 
	char *soundFiles[5] =  {"rom://music.mod", "rom://pacman_intro.wav", "rom://pacman_x.wav", "rom://pacman_x_s.wav"};
	//int SOUND_MUSIC = 0;//, SOUND_EFFECT = 1; //, SOUND_EDD_DOING = 2;
	tools_init("2.3", disp);
	sound_init(soundFiles, 4, 4);
	int numItems = 10;
	
	sprite_t* sprite1 = gfx_load_Sprite("numbers.sprite");
	sprite_t* sprite1_O = gfx_load_Sprite("numbers.sprite");
	
	sprite_t* sprite2 = gfx_load_Sprite("2.sprite");
	sprite_t* sprite2_O = gfx_load_Sprite("2.sprite");
	
	sprite_t* sprite3 = gfx_load_Sprite("3.sprite");
	sprite_t* sprite3_O = gfx_load_Sprite("3.sprite");
	
	sprite_t* sprite4 = gfx_load_Sprite("4.sprite");
	sprite_t* sprite4_O = gfx_load_Sprite("4.sprite");
	
	char tempMsg[24];
	sprintf(tempMsg, "@0,0: %lu", sprite4->data[0]);
	tools_print(tempMsg);
	
	sprite_t* aniSprite = gfx_load_Sprite("1.sprite");
	aniSprite->hslices = 12;
	aniSprite->vslices = 5;
	int aniStep = 0;
	int imgX = 0, imgY = 0, degree = 0;
	
	char menuItems[11][40] = { 	{"Move"},
								{"Scale"},
								{"Rotate"},
								{"Flip"},
								{"Fade"},
								{"Sound"},
								{"Sprite"},
								{"Shapes"},
								{"Measure"},
								{"Joystick"}  }; 
								

	int debug = 1;
	uint16_t fg = GFX_COLOR_WHITE, bg = GFX_COLOR_BLACK;
	int selectedItem = 0;
	int count = 0;
	int scenario = -1;
	int cursorX = 100, cursorY = 100, cursorTextSpacing = 30;
	degree = 0;
	int numLines = 12;
	char msgs[numLines][40];
	int jX = 0, jY = 0;
	for(int i = 0; i < numLines; i++)
	{
		strcpy(msgs[i], "xxx");
	}
	
	int shapeSelect = 0;
	int colorSelectors[6] = {0, 1, 2, 3, 4, 5};
	uint16_t colors[18] = 	{
								GFX_COLOR_WHITE, GFX_COLOR_BLACK, GFX_COLOR_LGRAY, GFX_COLOR_GRAY, GFX_COLOR_DGRAY,
								GFX_COLOR_RED, GFX_COLOR_LRED, GFX_COLOR_DRED,
								GFX_COLOR_LGREEN, GFX_COLOR_DGREEN, GFX_COLOR_GREEN, 
								GFX_COLOR_LBLUE, GFX_COLOR_DBLUE, GFX_COLOR_BLUE,
								GFX_COLOR_YELLOW, GFX_COLOR_BROWN, GFX_COLOR_ORANGE,
								GFX_COLOR_TRANS
							};
	char colorNames[18][40] = 	{
								"WHITE", "BLACK", "LGRAY", "GRAY", "DGRAY",
								"RED", "LRED", "DRED",
								"LGREEN", "DGREEN", "GREEN", 
								"LBLUE", "DBLUE", "BLUE",
								"YELLOW", "BROWN", "ORANGE",
								"TRANS"
							};
							
	char shapeNames[6][18] = 	{
								"Filled Rect", "Outlined Rect", "Filled Circle", "Outlined Circle", "Filled Triangle", "Outlined Triangle"
							};
	
	gfx_shape_rectangle rect1, rect2;
	
	rect1.x = ZERO_X;
	rect1.y = ZERO_Y;
	rect1.width = 50;
	rect1.height = 150;
	rect1.color = colors[colorSelectors[0]];
	rect1.fill = TRUE;
	
	rect2.x = ZERO_X + 60;
	rect2.y = ZERO_Y;
	rect2.width = 80;
	rect2.height = 60;
	rect2.color = colors[colorSelectors[1]];
	rect2.fill = FALSE;
	
	gfx_shape_circle circle1, circle2;
	
	circle1.x = 50;
	circle1.y = 50;
	circle1.radius = 50;
	circle1.color = colors[colorSelectors[2]];
	circle1.fill = TRUE;
	
	circle2.x = 50;
	circle2.y = 50;
	circle2.radius = 20;
	circle2.color = colors[colorSelectors[3]];
	circle2.fill = FALSE;
	
	gfx_shape_triangle triangle1, triangle2;
	
	triangle1.x0 = 40;
	triangle1.y0 = 40;
	triangle1.x1 = 60;
	triangle1.y1 = 60;
	triangle1.x2 = 20;
	triangle1.y2 = 80;
	triangle1.color = colors[colorSelectors[4]];
	triangle1.fill = TRUE;
	
	triangle2.x0 = 150;
	triangle2.y0 = 60;
	triangle2.x1 = 120;
	triangle2.y1 = 180;
	triangle2.x2 = 180;
	triangle2.y2 = 40;
	triangle2.color = colors[colorSelectors[5]];
	triangle2.fill = FALSE;
	
	// GAME LOOP
	while(1) 
	{
		/* Grab a render buffer */
		while( !(disp = display_lock()) );
		graphics_fill_screen( disp, 0 );
		
		tools_update();
		struct controller_data keys = get_keys_down();
		if(keys.c[0].start)
		{
			free(sprite1);
			free(sprite2);
			free(sprite3);
			free(sprite4);
			sprite1 = malloc(sizeof(sprite1_O));
			sprite2 = malloc(sizeof(sprite2_O));
			sprite3 = malloc(sizeof(sprite3_O));
			sprite4 = malloc(sizeof(sprite4_O));
			memcpy(sprite1, sprite1_O, sizeof(sprite1_O));
			memcpy(sprite2, sprite2_O, sizeof(sprite2_O));
			memcpy(sprite3, sprite3_O, sizeof(sprite3_O));
			memcpy(sprite4, sprite4_O, sizeof(sprite4_O));
		}
		if(scenario == 0)
		{
			keys = get_keys_pressed();
			graphics_draw_text(disp, 10+ZERO_X , 190+ ZERO_Y , "Use DPad to\nmove the image");
			if(keys.c[0].up) imgY = imgY > 0+ZERO_X ? imgY-1 : imgY;
			if(keys.c[0].down) imgY = imgY < 240 ? imgY+1 : imgY;
			if(keys.c[0].left) imgX = imgX > 0 + ZERO_Y ? imgX-1 : imgX;
			if(keys.c[0].right) imgX = imgX < 320 ? imgX+1 : imgX;
			if(keys.c[0].Z) scenario = -1;
			graphics_draw_sprite_trans( disp, imgX + ZERO_X, imgY + ZERO_Y, sprite1);
			graphics_draw_sprite_trans( disp, imgX + ZERO_X + 74, imgY + ZERO_Y, sprite2);
			graphics_draw_sprite_trans( disp, imgX + ZERO_X, imgY + ZERO_Y + 74, sprite3);
			graphics_draw_sprite_trans( disp, imgX + ZERO_X + 30, imgY + ZERO_Y + 30, sprite4);
		} else if(scenario == 1)
		{
			graphics_draw_text(disp, 10+ZERO_X , 170 +ZERO_Y, "Use DPad for BILINEAR\nscaling (up and down)");
			graphics_draw_text(disp, 10 +ZERO_X, 190+ZERO_Y , "Use Y-Buttons for\nnearest neighbour scaling");
			
			if(keys.c[0].up)
			{
				sprite1 = gfx_sprite_Scale(sprite1, BILINEAR, 1.2f, 1);
				sprite2 = gfx_sprite_Scale(sprite2, BILINEAR, 1.2f, 1);
				sprite3 = gfx_sprite_Scale(sprite3, BILINEAR, 1.2f, 1);
				sprite4 = gfx_sprite_Scale(sprite4, BILINEAR, 1.2f, 1);
			}
			if(keys.c[0].down)
			{ 
				sprite1 = gfx_sprite_Scale(sprite1, BILINEAR, 0.8f, 1);
				sprite2 = gfx_sprite_Scale(sprite2, BILINEAR, 0.8f, 1);
				sprite3 = gfx_sprite_Scale(sprite3, BILINEAR, 0.8f, 1);
				sprite4 = gfx_sprite_Scale(sprite4, BILINEAR, 0.8f, 1);
			}
			if(keys.c[0].C_up)
			{ 
				sprite1 = gfx_sprite_Scale(sprite1, NEAREST_NEIGHBOUR, 1.2f, 1);
				sprite2 = gfx_sprite_Scale(sprite2, NEAREST_NEIGHBOUR, 1.2f, 1);
				sprite3 = gfx_sprite_Scale(sprite3, NEAREST_NEIGHBOUR, 1.2f, 1);
				sprite4 = gfx_sprite_Scale(sprite4, NEAREST_NEIGHBOUR, 1.2f, 1);
			}
			if(keys.c[0].C_down)
			{ 
				sprite1 = gfx_sprite_Scale(sprite1, NEAREST_NEIGHBOUR, 0.8f, 1);
				sprite2 = gfx_sprite_Scale(sprite2, NEAREST_NEIGHBOUR, 0.8f, 1);
				sprite3 = gfx_sprite_Scale(sprite3, NEAREST_NEIGHBOUR, 0.8f, 1);
				sprite4 = gfx_sprite_Scale(sprite4, NEAREST_NEIGHBOUR, 0.8f, 1);
			}
			if(keys.c[0].Z) scenario = -1;
			graphics_draw_sprite_trans( disp, imgX + ZERO_X, imgY + ZERO_Y, sprite1);
			graphics_draw_sprite_trans( disp, imgX + ZERO_X + 74, imgY + ZERO_Y, sprite2);
			graphics_draw_sprite_trans( disp, imgX + ZERO_X, imgY + ZERO_Y + 74, sprite3);
			graphics_draw_sprite_trans( disp, imgX + ZERO_X + 74, imgY + ZERO_Y + 74, sprite4);
		} else if(scenario == 2)
		{
			keys = get_keys_pressed();
			int rot = 0;
			
			if(keys.c[0].left)
			{ 
				--degree;
				rot = 1;
			}
			
			if(keys.c[0].right)
			{ 
				++degree;
				rot = 1;
			}
			
			if(keys.c[0].C_up)
			{
				degree = 0;
				rot = 1;
			}
			
			if(keys.c[0].C_down)
			{
				degree = 180;
				rot = 1;
			}
			
			if(keys.c[0].C_left)
			{
				degree = 270;
				rot = 1;
			}
			
			if(keys.c[0].C_right)
			{
				degree = 90;
				rot = 1;
			}
			
			if(rot)
			{
				free(sprite2);
				free(sprite3);
				free(sprite4);
				sprite2 = gfx_sprite_Rotate(sprite2_O, degree, FALSE);
				sprite3 = gfx_sprite_Rotate(sprite3_O, degree, FALSE);
				sprite4 = gfx_sprite_Rotate(sprite4_O, degree, FALSE);
			}
			
			graphics_draw_text(disp, 10+ZERO_X , 190+ZERO_Y , "Use DPad (left / right) to rotate");
			char degMessage[30];
			sprintf(degMessage, "%d", degree);
			graphics_draw_text( disp, 10+ZERO_X, 160+ZERO_Y, degMessage);
			if(keys.c[0].Z) scenario = -1;
			graphics_draw_sprite_trans( disp, imgX + ZERO_X + 140, imgY + ZERO_Y, sprite2);
			graphics_draw_sprite_trans( disp, imgX + ZERO_X, imgY + ZERO_Y, sprite3);
			graphics_draw_sprite_trans( disp, imgX + ZERO_X + 140, imgY + ZERO_Y + 100, sprite4);
			
			gfx_drawRectangle(disp, imgX + ZERO_X + 140, imgY + ZERO_Y, sprite2->width, sprite2->height, GFX_COLOR_RED, FALSE);
			gfx_drawRectangle(disp, imgX + ZERO_X, imgY + ZERO_Y, sprite3->width, sprite3->height, GFX_COLOR_GREEN, FALSE);
			gfx_drawRectangle(disp, imgX + ZERO_X + 140, imgY + ZERO_Y + 100, sprite4->width, sprite4->height, GFX_COLOR_BLUE, FALSE);
			
			gfx_drawCircle(disp, imgX + ZERO_X + sprite2->width / 2 + 140, imgY + ZERO_Y + sprite2->height/2, 
				sprite2->width > sprite2->height ? sprite2->width / 2 : sprite2->height / 2, GFX_COLOR_RED, FALSE);
			gfx_drawCircle(disp, imgX + ZERO_X + sprite3->width / 2, imgY + ZERO_Y + sprite3->height/2, 
				sprite3->width > sprite3->height ? sprite3->width / 2 : sprite3->height / 2, GFX_COLOR_GREEN, FALSE);
			gfx_drawCircle(disp, imgX + ZERO_X + sprite4->width / 2 + 140, imgY + ZERO_Y + sprite4->height/2 + 100, 
				sprite4->width > sprite4->height ? sprite4->width / 2 : sprite4->height / 2, GFX_COLOR_BLUE, FALSE);
		} else if(scenario == 3)
		{
			if(keys.c[0].A)
			{
				gfx_sprite_HFlip(sprite1);
				gfx_sprite_HFlip(sprite2);
				gfx_sprite_HFlip(sprite3);
				gfx_sprite_HFlip(sprite4);
			}
			if(keys.c[0].B)
			{
				gfx_sprite_VFlip(sprite1);
				gfx_sprite_VFlip(sprite2);
				gfx_sprite_VFlip(sprite3);
				gfx_sprite_VFlip(sprite4);
			}
			graphics_draw_text(disp, 10 +ZERO_X, 190+ZERO_Y , "Use A to HFlip, B to VFlip");
			if(keys.c[0].Z) scenario = -1;
			graphics_draw_sprite_trans( disp, imgX + ZERO_X, imgY + ZERO_Y, sprite1);
			graphics_draw_sprite_trans( disp, imgX + ZERO_X + 74, imgY + ZERO_Y, sprite2);
			graphics_draw_sprite_trans( disp, imgX + ZERO_X, imgY + ZERO_Y + 74, sprite3);
			graphics_draw_sprite_trans( disp, imgX + ZERO_X + 74, imgY + ZERO_Y + 74, sprite4);
		} else if(scenario == 4)
		{
			keys = get_keys_pressed();
			graphics_draw_text(disp, 10 + ZERO_X, 160 +ZERO_Y , "DPad (up/down) to fade rgb");
			if(keys.c[0].up)
			{
				fx_sprite_Fade(sprite1, 1);
				fx_sprite_Fade(sprite2, 1);
				fx_sprite_Fade(sprite3, 1);
				fx_sprite_Fade(sprite4, 1);
			}
			if(keys.c[0].down)
			{
				fx_sprite_Fade(sprite1, -1);
				fx_sprite_Fade(sprite2, -1);
				fx_sprite_Fade(sprite3, -1);
				fx_sprite_Fade(sprite4, -1);
			}
			if(keys.c[0].Z) scenario = -1;
			graphics_draw_sprite_trans( disp, imgX + ZERO_X, imgY + ZERO_Y, sprite1);
			graphics_draw_sprite_trans( disp, imgX + ZERO_X + 74, imgY + ZERO_Y, sprite2);
			graphics_draw_sprite_trans( disp, imgX + ZERO_X, imgY + ZERO_Y + 74, sprite3);
			graphics_draw_sprite_trans( disp, imgX + ZERO_X + 74, imgY + ZERO_Y + 74, sprite4);
		} else if(scenario == 5)
		{
			
			keys = get_keys_down();
			if(keys.c[0].B) sound_playSample(2);
			if(keys.c[0].A) sound_playSample(1);
			if(keys.c[0].start)sound_playSample(3);
			
			if(keys.c[0].Z) scenario = -1;
		}  else if(scenario == 6)
		{
			keys = get_keys_down();
			if(keys.c[0].A) aniStep++;
			if(keys.c[0].C_down) aniSprite = gfx_sprite_Scale(aniSprite, BILINEAR, 0.8f, 1);
			if(keys.c[0].C_up) aniSprite = gfx_sprite_Scale(aniSprite, BILINEAR, 1.2f, 1);
			aniStep = aniStep > 59 ? 0 : aniStep;
			graphics_draw_sprite_trans_stride( disp, ZERO_X, ZERO_Y, aniSprite, aniStep);
			aniStep++;
			if(keys.c[0].Z) scenario = -1;
		} else if(scenario == 7)
		{
			keys = get_keys_down();
			
			if(keys.c[0].up) shapeSelect = shapeSelect == 5 ? 0 : shapeSelect + 1;
			if(keys.c[0].down) shapeSelect = shapeSelect == 0 ? 5 : shapeSelect - 1;
			if(keys.c[0].right) colorSelectors[shapeSelect] = colorSelectors[shapeSelect] == 17 ? 0 : colorSelectors[shapeSelect] + 1;
			if(keys.c[0].left) colorSelectors[shapeSelect] = colorSelectors[shapeSelect] == 0 ? 17 : colorSelectors[shapeSelect] - 1;
			
			rect1.color = colors[colorSelectors[0]];
			rect2.color = colors[colorSelectors[1]];
			circle1.color = colors[colorSelectors[2]];
			circle2.color = colors[colorSelectors[3]];
			triangle1.color = colors[colorSelectors[4]];
			triangle2.color = colors[colorSelectors[5]];
			
			graphics_draw_text(disp, 96, ZERO_Y + 10, "Switch Shape with up/down\nSwitch Color with left/right");
			char msg1[40], msg2[40];
			sprintf(msg1, "Shape: %s", shapeNames[shapeSelect]);
			sprintf(msg2, "Color: %s", colorNames[colorSelectors[shapeSelect]]);
			graphics_draw_text(disp, 120, ZERO_Y + 80, msg1);
			graphics_draw_text(disp, 120, ZERO_Y + 120, msg2);
			
			
			gfx_drawRectangle(disp, rect1.x, rect1.y, rect1.width, rect1.height, rect1.color, rect1.fill);
			gfx_drawCircle(disp, circle1.x, circle1.y, circle1.radius, circle1.color, circle1.fill);
			gfx_drawCircle(disp, circle2.x, circle2.y, circle2.radius, circle2.color, circle2.fill);
			gfx_drawTriangle(	disp, triangle1.x0, triangle1.y0, 
								triangle1.x1, triangle1.y1, 
								triangle1.x2, triangle1.y2, 
								triangle1.color, triangle1.fill);
			gfx_drawTriangle(	disp, triangle2.x0, triangle2.y0, 
								triangle2.x1, triangle2.y1, 
								triangle2.x2, triangle2.y2, 
								triangle2.color, triangle2.fill);
			gfx_drawRectangle(disp, rect2.x, rect2.y, rect2.width, rect2.height, rect2.color, rect2.fill);
			if(keys.c[0].Z) scenario = -1;
		} else if(scenario == 8)
		{
			keys = get_keys_pressed();
			graphics_draw_text(disp, 10 +ZERO_X, 190 +ZERO_Y, "Use DPad to\nmove the cursor");
			if(keys.c[0].up) cursorY--;
			if(keys.c[0].down) cursorY++;
			if(keys.c[0].left) cursorX--;
			if(keys.c[0].right) cursorX++; 
			char msg[24];
			sprintf(msg, "%dx%d",cursorX, cursorY);
			if(keys.c[0].x < 0 || keys.c[0].y < 0)
			{
				consoleIndex++;
				tools_print(msg);
			}
			graphics_draw_box(disp, cursorX, cursorY,1,1, GFX_COLOR_RED);
			graphics_draw_text(disp, cursorX - cursorTextSpacing, cursorY - cursorTextSpacing, msg);
			graphics_draw_text(disp, cursorX + cursorTextSpacing, cursorY - cursorTextSpacing, msg);
			graphics_draw_text(disp, cursorX - cursorTextSpacing, cursorY + cursorTextSpacing, msg);
			graphics_draw_text(disp, cursorX + cursorTextSpacing, cursorY + cursorTextSpacing, msg);
			if(keys.c[0].Z) scenario = -1;
		} else if(scenario == 9)
		{
			keys = get_keys_pressed();
			for(int i = 1; i < numLines; i++)
			{
				 strcpy(msgs[i-1], msgs[i]);
			}
			jX = keys.c[0].x;
			jY = keys.c[0].y;++count;
			sprintf(msgs[numLines - 1], "#%d :: \tx: %d - \ty: %d", ++count, jX, jY);
			for(int i = 0; i < numLines; i++)
			{
				graphics_draw_text( disp, 10+ZERO_X, i * 20 + ZERO_Y, msgs[i]);
			}
			if(keys.c[0].Z) scenario = -1;
		} else
		{
			for(int i = 0; i < numItems; i++)
			{
				int x = 20 + (i % 2) * 120, y = ZERO_Y + (i % numItems) * 13;
				fg = GFX_COLOR_WHITE;
				bg = GFX_COLOR_BLACK;
				if(i == selectedItem)
				{
					bg = GFX_COLOR_WHITE;
					fg = GFX_COLOR_BLACK;
					graphics_draw_box(disp, x - 10 + ZERO_X, y - 2 + ZERO_Y, 105, 10, bg);
				}
				graphics_set_color(fg,bg);
				graphics_draw_text(disp, x + ZERO_X , y + ZERO_Y, menuItems[i]);
			}
			
			if(keys.c[0].up) selectedItem--;
			else if(keys.c[0].down) selectedItem++;
			if(selectedItem >= numItems) selectedItem = numItems - 1;
			else if(selectedItem < 0) selectedItem = 0;		
			if(keys.c[0].A) scenario = selectedItem;
		}
		if(keys.c[0].B) debug = 1 - debug;
		keys = get_keys_pressed();
		int consoleScroll = 0;
		if(keys.c[0].L)	consoleScroll = -1;
		if(keys.c[0].R) consoleScroll = 1;
		
		tools_show(disp, debug, consoleScroll);
	}
}
