#include <stdio.h>
#include "includes.h"

int ALT_x1; int ALT_x2; int ALT_y;
int ALT_inc_x; int ALT_inc_y;
int pixel_buffer_x; int pixel_buffer_y;
int blue_x1; int blue_y1; int blue_x2; int blue_y2;
short balZwart = 0x000000;
short balWit = 0xffffff;

void Game(void* pdata){
	
	ALT_x1 = 0; ALT_x2 = 20; ALT_y = 3; ALT_inc_x = 1; ALT_inc_y = 1;

	pixel_buffer_x = 340; pixel_buffer_y = 236;

	blue_x1 = 28; blue_x2 = 52; blue_y1 = 26; blue_y2 = 34;
	
	//ALT_x1 = 0; ALT_x2 = 5/* ALTERA = 6 chars */; ALT_y = 0; ALT_inc_x = 1; ALT_inc_y = 1;

	//char_buffer_x = 79; char_buffer_y = 59;

	//blue_x1 = 28; blue_x2 = 52; blue_y1 = 26; blue_y2 = 34;
	while(1){
		
		VGA_box(ALT_x1, ALT_y, ALT_x1+5, ALT_y+5, balZwart); // erase
		ALT_x1 += ALT_inc_x;
		ALT_x2 += ALT_inc_x;
		ALT_y += ALT_inc_y;

		if ( (ALT_y == pixel_buffer_y) || (ALT_y == 0) )
			ALT_inc_y = -(ALT_inc_y);
		if ( (ALT_x2 == pixel_buffer_x) || (ALT_x1 == 0) )
			ALT_inc_x = -(ALT_inc_x);


	
		VGA_box(ALT_x1, ALT_y, ALT_x1+5, ALT_y+5, balWit); // ball
		
		/*
		VGA_text (ALT_x1, ALT_y, "          ");		// erase
		ALT_x1 += ALT_inc_x;
		ALT_x2 += ALT_inc_x;
		ALT_y += ALT_inc_y;

		if ( (ALT_y == char_buffer_y) || (ALT_y == 0) )
			ALT_inc_y = -(ALT_inc_y);
		if ( (ALT_x2 == char_buffer_x) || (ALT_x1 == 0) )
			ALT_inc_x = -(ALT_inc_x);



		VGA_text (ALT_x1, ALT_y, "HALLOOO");
		*/

		OSTimeDly(1);
	}
}
