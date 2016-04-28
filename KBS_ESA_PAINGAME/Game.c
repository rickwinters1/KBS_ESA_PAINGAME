#include <stdio.h>
#include "includes.h"
#include "altera_up_avalon_parallel_port.h"

short kleur1 = 0xffffff;

int ALT_x1; int ALT_x2; int ALT_y;
int ALT_inc_x; int ALT_inc_y;
int char_buffer_x; int char_buffer_y;
int blue_x1; int blue_y1; int blue_x2; int blue_y2;

void Game(void* pdata){
	ALT_x1 = 0; ALT_x2 = 5; ALT_y = 0; ALT_inc_x = 1; ALT_inc_y = 1;

	char_buffer_x = 79; char_buffer_y = 59;

	blue_x1 = 28; blue_x2 = 52; blue_y1 = 26; blue_y2 = 34;

	while(1){

		VGA_text (ALT_x1, ALT_y, "          ");		// erase
		ALT_x1 += ALT_inc_x;
		ALT_x2 += ALT_inc_x;
		ALT_y += ALT_inc_y;

		if ( (ALT_y == char_buffer_y) || (ALT_y == 0) )
			ALT_inc_y = -(ALT_inc_y);
		if ( (ALT_x2 == char_buffer_x) || (ALT_x1 == 0) )
			ALT_inc_x = -(ALT_inc_x);



		VGA_text (ALT_x1, ALT_y, "HALLOOO");

		OSTimeDly(1);
	}
}
