#include <stdio.h>
#include "includes.h"

OS_EVENT* gameSem;
OS_EVENT* controllerSem;
OS_EVENT* MailBox;
OS_EVENT* MailBox2;


OS_FLAG_GRP *Flags;

#define Menu_Flag 0x01
#define Game_Flag 0x02
#define C1_Flag 0x04
#define C2_Flag 0x08
#define Menu2_Flag 0x10

#define zwart  0x000000
#define wit  0xffffff
#define geel 0xffff00
#define rood 0xf800
#define blauw 0x0000ff
#define groen 0x0697

int ALT_x1;
int ALT_x2;
int ALT_y;
int ALT_inc_x;
int ALT_inc_y;
int pixel_buffer_x;
int pixel_buffer_y;

int score1, score2 = 0;
int first = 1;
int check = 1;
int random;

typedef struct balk {
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
	int count;
	Balk * balkje;
	Balk * balkje2;

	//ALT_x1 = 0; ALT_x2 = 5/* ALTERA = 6 chars */; ALT_y = 0; ALT_inc_x = 1; ALT_inc_y = 1;

	//char_buffer_x = 79; char_buffer_y = 59;

	//blue_x1 = 28; blue_x2 = 52; blue_y1 = 26; blue_y2 = 34;

	while (1) {
		OSFlagPend(Flags, Game_Flag + C1_Flag + C2_Flag, OS_FLAG_WAIT_CLR_ANY, 0, &err);

		balkje = (Balk*) OSMboxPend(MailBox, 0, &err);
		balkje2 = (Balk*) OSMboxPend(MailBox2, 0, &err);



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

			VGA_box(ALT_x1, ALT_y, ALT_x1 + 5, ALT_y + 5, zwart); // erase
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
			VGA_box(ALT_x1, ALT_y, ALT_x1 + 5, ALT_y + 5, wit); // ball

			//collision rand boven en onder
			if ((ALT_y == pixel_buffer_y) || (ALT_y == 4)) {
				ALT_inc_y = -(ALT_inc_y);
			}
			//collision rand rechts en links
			if ((ALT_x2 == pixel_buffer_x) || (ALT_x1 == 0)) {
				//ALT_inc_x = -(ALT_inc_x);
			}

			//links dood
			if (ALT_x1 == 0) {
				score2++;
				gescoord(2);
				del_number(2);
			}

			//rechts dood
			if (ALT_x2 == pixel_buffer_x) {
				score1++;
				gescoord(1);
				del_number(1);
			}

			//collision linker balkje
			//lange zijde
			if(ALT_x1 == 21 && (ALT_y > balkje->Hoogte - 6 && ALT_y < balkje->Hoogte + 50)){
				ALT_inc_x = -(ALT_inc_x);
			}
			//boven
			if(((ALT_x1 <= 20 && ALT_x1 >= 15) && ALT_y == balkje->Hoogte - 6) || ((ALT_x1 <= 20 && ALT_x1 >= 15) && ALT_y == balkje->Hoogte + 50)){
				ALT_inc_y = -(ALT_inc_y);
			}

			//collision rechter balkje
			//lange zijde
			if(ALT_x2 == 299 && (ALT_y > balkje2->Hoogte - 6 && ALT_y < balkje2->Hoogte + 50)){
				ALT_inc_x = -(ALT_inc_x);
			}
			//boven
			if(((ALT_x2 <= 305 && ALT_x2 >= 300) && ALT_y == balkje2->Hoogte - 6) || ((ALT_x2 <= 305 && ALT_x2 >= 300) && ALT_y == balkje2->Hoogte + 50)){
				ALT_inc_y = -(ALT_inc_y);
			}



			OSTimeDly(1);

		} else if (controller(2) != 2) {
			endGame();
		}

	}
}

void gescoord(int ID){

	VGA_box(ALT_x1, ALT_y, ALT_x1 + 5, ALT_y + 5, zwart); // erase

	first = 1;

	if(ID == 1){
		VGA_text(30, 25, "Speler 1 heeft gescoord!");
		OSTimeDlyHMSM(0, 0, 35, 0);
		VGA_text(30, 25, "                        ");
		ALT_x1 = 0;
		ALT_x2 = 165;
		ALT_y = 100;
		ALT_inc_x = -1;
		if(random == 1){
			ALT_inc_y = -1;
			random == 0;
		}else if(random == 0){
			ALT_inc_y = 1;
			random == 1;
		}
	}else if(ID == 2){
		VGA_text(30, 25, "Speler 2 heeft gescoord!");
		OSTimeDlyHMSM(0, 0, 35, 0);
		VGA_text(30, 25, "                        ");
		ALT_x1 = 0;
		ALT_x2 = 165;
		ALT_y = 100;
		ALT_inc_x = 1;
		if(random == 1){
			ALT_inc_y = 1;
			random = 0;
		}else if(random == 0){
			ALT_inc_y = -1;
			random = 1;
		}
	}else if(ID == 3){
		VGA_text(30, 25, "De computer heeft gescoord!");
		OSTimeDlyHMSM(0, 0, 35, 0);
		VGA_text(30, 25, "                           ");
		ALT_x1 = 0;
		ALT_x2 = 165;
		ALT_y = 100;
		ALT_inc_x = -1;
		if(random == 1){
			ALT_inc_y = 1;
			random = 0;
		}else if(random == 0){
			ALT_inc_y = -1;
			random = 1;
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
	VGA_box(ALT_x1, ALT_y, ALT_x1 + 5, ALT_y + 5, zwart); // erase

	ALT_x1 = 0;
	ALT_x2 = 165;
	ALT_y = 100;
	ALT_inc_x = 1;
	ALT_inc_y = 1;

	check = 1;
	first = 1;

}
