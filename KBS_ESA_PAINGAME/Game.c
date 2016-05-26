#include <stdio.h>
#include "includes.h"
#include <string.h>

OS_EVENT* gameSem;
OS_EVENT* controllerSem;
OS_EVENT* MailBox;
OS_EVENT* MailBox2;


OS_FLAG_GRP *Flags;
OS_FLAG_GRP *Flags_Games;
OS_FLAG_GRP *Flags_Tutorial;


#define Menu_Flag 0x01
#define Game_Flag 0x02
#define C1_Flag 0x04
#define C2_Flag 0x08
#define Menu2_Flag 0x10
#define Singleplayer_Flag 0x20
#define Tutorial_Flag 0x40


#define zwart  0x000000
#define wit  0xffffff
#define geel 0xffff00
#define rood 0xf800
#define blauw 0x0000ff
#define groen 0x0697

#define beginLevens 3

int ALT_x1;
int ALT_x2;
int ALT_y;
int ALT_inc_x;
int ALT_inc_y;
int pixel_buffer_x;
int pixel_buffer_y;

int score1, score2, score3 = 0;
int first = 1;
int check = 1;
int random;
int X;
int leven = beginLevens;

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
	
	VGA_box (316, 5, 319, 235, zwart); 					// singleplayer balk weghalen voor de zekerheid
	//ALT_x1 = 0; ALT_x2 = 5/* ALTERA = 6 chars */; ALT_y = 0; ALT_inc_x = 1; ALT_inc_y = 1;

	//char_buffer_x = 79; char_buffer_y = 59;

	//blue_x1 = 28; blue_x2 = 52; blue_y1 = 26; blue_y2 = 34;

	while (1) {
		OSFlagPend(Flags_Games, Game_Flag, OS_FLAG_WAIT_CLR_ANY, 0, &err);

		//printf("Game flag is set\n");

		balkje = (Balk*) OSMboxPend(MailBox, 0, &err);
		balkje2 = (Balk*) OSMboxPend(MailBox2, 0, &err);



		for (i = 0; i <= 9; i++) {

			if (score1 == i) {
				draw_number(i, 1);
			}
			if (score2 == i) {
				draw_number(i, 3);
			}
			if(score1 == 9 || score2 == 9){
				score1 =0;
				score2 =0;

			}
		}
		draw_middenlijn();
		if (check == 1) {
			del_middenlijn();

			VGA_box(300, balkje2->Hoogte, 305, balkje2->Hoogte + 50, blauw);

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

		} else if (controller(3) == 2) {
			//links
			endGame(2);
		}else if(controller(3) == 3){
			//rechts
			endGame(1);
		} else if(controller(2) != 2){
			endGame(1);
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

void endGame(int ID) {
	INT8U err;

	printf("end game\n");

	clearScreen();

	OSFlagPost(Flags, C1_Flag + C2_Flag, OS_FLAG_SET, &err);
	OSFlagPost(Flags_Games, Game_Flag, OS_FLAG_SET, &err);


	if(ID == 1){
		VGA_text(35, 25, "Player 1 lost");
		OSTimeDlyHMSM(0, 0, 40, 0);
		VGA_text(35, 25, "             ");
	} else if(ID == 2){
		VGA_text(35, 25, "Player 2 lost");
		OSTimeDlyHMSM(0, 0, 40, 0);
		VGA_text(35, 25, "             ");
	}


	OSFlagPost(Flags, Menu_Flag + Menu2_Flag, OS_FLAG_CLR, &err);

	teken_menu(2);
	deleteNummer();


	VGA_box(ALT_x1, ALT_y, ALT_x1 + 5, ALT_y + 5, zwart); // erase

	ALT_x1 = 0;
	ALT_x2 = 165;
	ALT_y = 100;
	ALT_inc_x = 1;
	ALT_inc_y = 1;

	check = 1;
	first = 1;

}

void endSingleplayer(){
	INT8U err;

	printf("end Singleplayer\n");

	//clearScreen();

	OSFlagPost(Flags, C1_Flag, OS_FLAG_SET, &err);

	VGA_text(35, 25, "GAME OVER");
	OSTimeDlyHMSM(0, 0, 40, 0);
	VGA_text(35, 25, "         ");


	VGA_text(6,5, "           ");


	teken_menu(1);

	VGA_box (316, 4, 319, 235, zwart); 					// singleplayer balk weghalen voor de zekerheid

	VGA_box(ALT_x1, ALT_y, ALT_x1 + 5, ALT_y + 5, zwart); // erase
	deleteNummer();


	ALT_x1 = 0;
	ALT_x2 = 165;
	ALT_y = 100;
	ALT_inc_x = -1;
	ALT_inc_y = 1;

	leven = beginLevens;
	score3 = 0;
	first = 1;

	OSFlagPost(Flags, Menu_Flag + Menu2_Flag, OS_FLAG_CLR, &err);

	OSFlagPost(Flags_Games, Singleplayer_Flag, OS_FLAG_SET, &err);

}

void Singleplayer(void* pdata){
	
	ALT_x1 = 0;
	ALT_x2 = 165;
	ALT_y = 100;
	ALT_inc_x = -1;
	ALT_inc_y = 1;

	pixel_buffer_x = 315;
	pixel_buffer_y = 230;

	INT8U err;
	int i;
	int hoogte;
	int ID;
	int count;
	int q;
	Balk * balkje;
	
	char levens[10];

	score3 = 0;
	

	while (1) {
		OSFlagPend(Flags_Games, Singleplayer_Flag, OS_FLAG_WAIT_CLR_ALL, 0, &err);

		//printf("Single flag is set\n");

		balkje = (Balk*) OSMboxPend(MailBox, 0, &err);

		//printf("ID is: %d\tHoogte is: %d\n", balkje->ID, balkje->Hoogte);




		VGA_box(ALT_x1, ALT_y, ALT_x1 + 5, ALT_y + 5, zwart); // erase

		if (first == 1) {
			printf("FIRST");
			ALT_x1 = 160 + ALT_x1 + ALT_inc_x;
			ALT_x2 += ALT_inc_x;
			ALT_y += ALT_inc_y;

			del_middenlijn();
			VGA_box (316, 0, 319, 239, groen); 					// rechts
			VGA_box(300, 4, 305, 235, zwart);

			first = 0;
		} else {
			ALT_x1 += ALT_inc_x;
			ALT_x2 += ALT_inc_x;
			ALT_y += ALT_inc_y;
		}


		//collision rand boven en onder
		if ((ALT_y == pixel_buffer_y) || (ALT_y == 4)) {
			ALT_inc_y = -(ALT_inc_y);
		}
		//collision rand rechts en links
		if ((ALT_x2 == pixel_buffer_x)) {
			ALT_inc_x = -(ALT_inc_x);
			deleteNummer();
			score3++;

		}

		VGA_box(ALT_x1, ALT_y, ALT_x1 + 5, ALT_y + 5, wit); // ball

		//links dood
		if (ALT_x1 == 0) {
			leven--;
			gescoord(3);
		}

		//kaats tegen muur, score erbij
//			if (ALT_x2 == pixel_buffer_x) {
//				score3++;
//			}

		//collision linker balkje
		//lange zijde
		if(ALT_x1 == 21 && (ALT_y > balkje->Hoogte && ALT_y < balkje->Hoogte + 50)){
			ALT_inc_x = -(ALT_inc_x);
		}
		//boven
		if((ALT_x1 < 24 && ALT_x1 > 21) && ALT_y == balkje->Hoogte){
			ALT_inc_y = -(ALT_inc_y);
		}
		//onder
		if((ALT_x1 < 24 && ALT_x1 > 21) && ALT_y == balkje->Hoogte + 50){
			ALT_inc_y = -(ALT_inc_y);
		}


		sprintf(levens, "LEVENS: %d", leven);
		VGA_text(6,5, levens);
		OSTimeDly(1);

		for (i = 0; i <= 999; i++) {

			if(score3 == i){

				if (score3 >= 0 && score3 <= 9){
					draw_number(i, 3);
				} else if (score3 >= 10 && score3 <= 99){
					draw_number(i /10 , 2);
					draw_number(i %10, 3);
				} else if (score3 >= 100 && score3 <= 999){
					draw_number(i /100 , 1);
					draw_number(i /10 %10, 2);
					draw_number(i %10, 3);
				}
			}
			if((score3 == 999) || (leven == 0)){ // max score of geen levens meer, spel eindigt.
				leven = 0;
				score3 =0;

				endSingleplayer();
			}
		}





	}
	
}

void tutorialUitleg(){
	INT8U err;
	
	VGA_text(10, 5, "Er zijn twee soorten spellen: Singleplayer en Multiplayer.");
	VGA_text(10, 7, "In Singleplayer speel je tegen de computer, waarbij je");
	VGA_text(10, 8, "zoveel mogelijk aantal punten moet bereiken door de bal");
	VGA_text(10, 9, "tegen de muur te kaatsen. Hierbij heb je 3 levens.");
	VGA_text(10, 11, "In Multiplayer gaat het er om dat je de bal in het doel");
	VGA_text(10, 12, "van de tegenstander krijgt. Het doel bevindt zich achter");
	VGA_text(10, 13, "het balkje van de tegenstander. Wanneer je scoort krijg je");
	VGA_text(10, 14, "1 punt.");
	
	VGA_text(10, 38, "De besturing gaat als volgt:");
	VGA_text(10, 40, "De linker balk wordt bestuurd door het op en neer bewegen");
	VGA_text(10, 41, "van switch 17. Bij de multiplayer wordt ook nog vereist om");
	VGA_text(10, 42, "de linker knop in te houden, anders ben je af. Dit geldt ook");
	VGA_text(10, 43, "voor de rechter knop voor de rechter balk.");
	VGA_text(10, 45, "De rechter balk wordt bestuurd KEY3 en KEY2. KEY3 = omlaag,");
	VGA_text(10, 46, "KEY2 = omhoog.");
}

void endTutorial(){
	INT8U err;

	printf("end Tutorial\n");

	//clearScreen();
	VGA_text(10, 5, "                                                                              ");
	VGA_text(10, 6, "                                                                              ");
	VGA_text(10, 7, "                                                                              ");
	VGA_text(10, 8, "                                                                              ");
	VGA_text(10, 9, "                                                                              ");
	VGA_text(10, 11, "                                                                              ");
	VGA_text(10, 12, "                                                                              ");
	VGA_text(10, 13, "                                                                              ");
	VGA_text(10, 14, "                                                                              ");
	VGA_text(10, 38, "                                                                              ");
	VGA_text(10, 40, "                                                                              ");
	VGA_text(10, 41, "                                                                              ");
	VGA_text(10, 42, "                                                                              ");
	VGA_text(10, 43, "                                                                              ");
	VGA_text(10, 45, "                                                                              ");
	VGA_text(10, 46, "                                                                              ");

	OSFlagPost(Flags, C1_Flag, OS_FLAG_SET, &err);

	VGA_text(35, 25, "Einde Tutorial");
	OSTimeDlyHMSM(0, 0, 40, 0);
	VGA_text(35, 25, "              ");




	teken_menu(4);

	VGA_box (316, 4, 319, 235, zwart); 					// singleplayer/tutorial balk weghalen voor de zekerheid

	VGA_box(ALT_x1, ALT_y, ALT_x1 + 5, ALT_y + 5, zwart); // erase


	ALT_x1 = 0;
	ALT_x2 = 165;
	ALT_y = 100;
	ALT_inc_x = -1;
	ALT_inc_y = 1;

	first = 1;

	OSFlagPost(Flags, Menu_Flag + Menu2_Flag, OS_FLAG_CLR, &err);

	OSFlagPost(Flags_Tutorial, Tutorial_Flag, OS_FLAG_SET, &err);

}

void Tutorial(void* pdata){
	
	ALT_x1 = 0;
	ALT_x2 = 165;
	ALT_y = 100;
	ALT_inc_x = 1;
	ALT_inc_y = 1;

	pixel_buffer_x = 315;
	pixel_buffer_y = 230;
	
	INT8U err;
	int i;
	int hoogte;
	int ID;
	int count;
	int q;
	Balk * balkje;
	
	while (1) {
		OSFlagPend(Flags_Tutorial, Tutorial_Flag, OS_FLAG_WAIT_CLR_ANY, 0, &err);

		balkje = (Balk*) OSMboxPend(MailBox, 0, &err);

		//printf("TUTORIAL SET\n");

		//printf("ID is: %d\tHoogte is: %d\n", balkje->ID, balkje->Hoogte);

		tutorialUitleg();
		
		VGA_box(ALT_x1, ALT_y, ALT_x1 + 5, ALT_y + 5, zwart); // erase


		if (first == 1) {
			printf("FIRST");
			ALT_x1 = 160 + ALT_x1 + ALT_inc_x;
			ALT_x2 += ALT_inc_x;
			ALT_y += ALT_inc_y;

			del_middenlijn();
			VGA_box (316, 0, 319, 239, groen); 					// rechts
			VGA_box(300, 4, 305, 235, zwart);

			first = 0;
		} else {
			ALT_x1 += ALT_inc_x;
			ALT_x2 += ALT_inc_x;
			ALT_y += ALT_inc_y;
		}
		
		



		//collision rand boven en onder
		if ((ALT_y == pixel_buffer_y) || (ALT_y == 4)) {
			ALT_inc_y = -(ALT_inc_y);
		}
		//collision rand rechts en links
		if ((ALT_x2 == pixel_buffer_x)) {
			ALT_inc_x = -(ALT_inc_x);

		}

		VGA_box(ALT_x1, ALT_y, ALT_x1 + 5, ALT_y + 5, wit); // ball
		
		//links dood
		if (ALT_x1 == 0) {
			gescoord(3);
		}

		//kaats tegen muur, score erbij
//			if (ALT_x2 == pixel_buffer_x) {
//				score3++;
//			}

		//collision linker balkje
		//lange zijde
		if(ALT_x1 == 21 && (ALT_y > balkje->Hoogte && ALT_y < balkje->Hoogte + 50)){
			ALT_inc_x = -(ALT_inc_x);
		}
		//boven
		if((ALT_x1 < 24 && ALT_x1 > 21) && ALT_y == balkje->Hoogte){
			ALT_inc_y = -(ALT_inc_y);
		}
		//onder
		if((ALT_x1 < 24 && ALT_x1 > 21) && ALT_y == balkje->Hoogte + 50){
			ALT_inc_y = -(ALT_inc_y);
		}

		
		if (controller(3) == 3) {
			endTutorial();
		}

		OSTimeDly(1);


	}
	
}



