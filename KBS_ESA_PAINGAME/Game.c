#include <stdio.h>
#include "includes.h"

OS_EVENT* gameSem;
OS_EVENT* controllerSem;
OS_EVENT* MailBox;

OS_FLAG_GRP *Flags;

#define Menu_Flag 0x01
#define Game_Flag 0x02
#define C1_Flag 0x04
#define C2_Flag 0x08
#define Menu2_Flag 0x10

int ALT_x1;
int ALT_x2;
int ALT_y;
int ALT_inc_x;
int ALT_inc_y;
int pixel_buffer_x;
int pixel_buffer_y;
int blue_x1;
int blue_y1;
int blue_x2;
int blue_y2;
short balZwart = 0x000000;
short balWit = 0xffffff;

int score1, score2 = 0;
int first = 1;
int check = 1;

typedef struct balk {
	int ID;
	int Hoogte;
} Balk;

void Game(void* pdata) {

	ALT_x1 = 0;
	ALT_x2 = 165;
	ALT_y = 100;
	ALT_inc_x = 1;
	ALT_inc_y = 1;

	pixel_buffer_x = 319;
	pixel_buffer_y = 230;

	INT8U err;
	int i;
	int hoogte;
	int ID;
	Balk * balkje;

	//ALT_x1 = 0; ALT_x2 = 5/* ALTERA = 6 chars */; ALT_y = 0; ALT_inc_x = 1; ALT_inc_y = 1;

	//char_buffer_x = 79; char_buffer_y = 59;

	//blue_x1 = 28; blue_x2 = 52; blue_y1 = 26; blue_y2 = 34;

	while (1) {
		OSFlagPend(Flags, Game_Flag + C1_Flag + C2_Flag, OS_FLAG_WAIT_CLR_ANY, 0, &err);

		balkje = (Balk*) OSQPend(MailBox, 0, &err);

		//printf("ID is: %d\tHoogte is: %d\n", balkje->ID, balkje->Hoogte);

		for (i = 0; i <= 9; i++) {
			if (score1 == i) {
				draw_number(i, 1);
			}
			if (score2 == i) {
				draw_number(i, 2);
			}
			if(score1 == 9 || score2 == 9){
				score1 =0;
				score2 =0;
				endGame();
			}
		}
		draw_middenlijn();
		if (check == 1) {
			del_middenlijn();
			VGA_text(20, 20, "Houdt de knop ingedrukt om te spelen");
			OSTimeDlyHMSM(0, 1, 0, 0);
			VGA_text(20, 20, "                                    ");

			del_number(1);
			del_number(2);

			score1 = 0;
			score2 = 0;

			i = 0;

			check = 0;
		}

		if (controller(3) == 1) {

			VGA_box(ALT_x1, ALT_y, ALT_x1 + 5, ALT_y + 5, balZwart); // erase
			if (first == 1) {
				ALT_x1 = 160 + ALT_x1 + ALT_inc_x;
				ALT_x2 += ALT_inc_x;
				ALT_y += ALT_inc_y;
				first = 0;
			} else {
				ALT_x1 += ALT_inc_x;
				ALT_x2 += ALT_inc_x;
				ALT_y += ALT_inc_y;
			}
			VGA_box(ALT_x1, ALT_y, ALT_x1 + 5, ALT_y + 5, balWit); // ball
			if ((ALT_y == pixel_buffer_y) || (ALT_y == 4)) {
				ALT_inc_y = -(ALT_inc_y);
			}
			if ((ALT_x2 == pixel_buffer_x) || (ALT_x1 == 0)) {
				ALT_inc_x = -(ALT_inc_x);
			}

			//links dood
			if (ALT_x1 == 0) {
				score2++;
				del_number(2);
			}

			//rechts dood
			if (ALT_x2 == pixel_buffer_x) {
				score1++;
				del_number(1);

			}

			//collision linker balkje
			if(balkje->ID == 1){
				if(ALT_x1 == 21 && (ALT_y > balkje->Hoogte && ALT_y < balkje->Hoogte + 50)){
					ALT_inc_x = -(ALT_inc_x);
				}
			}

			//collision rechter balkje
			if(balkje->ID == 2){
				if(ALT_x2 == 299 && (ALT_y > balkje->Hoogte && ALT_y < balkje->Hoogte + 50)){
					ALT_inc_x = -(ALT_inc_x);
				}
			}



			OSTimeDly(1);
		} else if (controller(2) != 2) {
			endGame();
		}
	}
}

void endGame() {
	INT8U err;

	printf("end game\n");

	clearScreen();

	OSFlagPost(Flags, Game_Flag + C1_Flag + C2_Flag, OS_FLAG_SET, &err);

	VGA_text(35, 25, "GAME OVER");
	OSTimeDlyHMSM(0, 0, 40, 0);
	VGA_text(35, 25, "         ");

	OSFlagPost(Flags, Menu_Flag + Menu2_Flag, OS_FLAG_CLR, &err);

	teken_menu();
	VGA_box(ALT_x1, ALT_y, ALT_x1 + 5, ALT_y + 5, balZwart); // erase

	ALT_x1 = 0;
	ALT_x2 = 165;
	ALT_y = 100;
	ALT_inc_x = 1;
	ALT_inc_y = 1;

	check = 1;
	first = 1;

}
