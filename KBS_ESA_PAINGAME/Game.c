#include <stdio.h>
#include "includes.h"

OS_EVENT* gameSem;
OS_EVENT* controllerSem;

OS_FLAG_GRP *Flags;

#define Menu_Flag 0x01
#define Game_Flag 0x02
#define C1_Flag 0x04
#define C2_Flag 0x08

int ALT_x1; int ALT_x2; int ALT_y;
int ALT_inc_x; int ALT_inc_y;
int pixel_buffer_x; int pixel_buffer_y;
int blue_x1; int blue_y1; int blue_x2; int blue_y2;
short balZwart = 0x000000;
short balWit = 0xffffff;

int first = 1;

void Game(void* pdata){
	
	ALT_x1 = 0; ALT_x2 = 165; ALT_y = 100; ALT_inc_x = 1; ALT_inc_y = 1;

	pixel_buffer_x = 319; pixel_buffer_y = 230;
	
	INT8U err;

	//ALT_x1 = 0; ALT_x2 = 5/* ALTERA = 6 chars */; ALT_y = 0; ALT_inc_x = 1; ALT_inc_y = 1;

	//char_buffer_x = 79; char_buffer_y = 59;

	//blue_x1 = 28; blue_x2 = 52; blue_y1 = 26; blue_y2 = 34;



	while(1){
		OSFlagPend(Flags, Game_Flag + C1_Flag + C2_Flag, OS_FLAG_WAIT_CLR_ANY, 0, &err);

		draw_number(0, 1);
		draw_number(8, 2);
		draw_middenlijn();

		VGA_box(ALT_x1, ALT_y, ALT_x1+5, ALT_y+5, balZwart); // erase
		if(first == 1){
			ALT_x1 = 160 + ALT_x1 + ALT_inc_x;
			ALT_x2 += ALT_inc_x;
			ALT_y += ALT_inc_y;
			first = 0;
		}else{
			ALT_x1 += ALT_inc_x;
			ALT_x2 += ALT_inc_x;
			ALT_y += ALT_inc_y;
		}

		if ( (ALT_y == pixel_buffer_y) || (ALT_y == 4) ){
			ALT_inc_y = -(ALT_inc_y);
		}
		if ( (ALT_x2 == pixel_buffer_x) || (ALT_x1 == 0) ){
			ALT_inc_x = -(ALT_inc_x);
			endGame();
		}
		// if balkje rechts collision
//		if ((ALT_y >= hoogte && ALT_y <= hoogte+50) && (ALT_x2 >= X && ALT_x2 <= X))
//			ALT_inc_x = -(ALT_inc_x);
		// if balkje links collision


	
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

void endGame(){
	INT8U err;

	printf("end game\n");
	clearScreen();
	OSFlagPost(Flags, Menu_Flag, OS_FLAG_CLR, &err);
	OSFlagPost(Flags, Game_Flag + C1_Flag + C2_Flag, OS_FLAG_SET, &err);
	teken_menu();
}
