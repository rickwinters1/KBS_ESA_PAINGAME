#include <stdio.h>
#include "includes.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Altera_UP_SD_Card_Avalon_Interface.h"

#define MAX_SUBDIRECTORIES 20

OS_EVENT* gameSem;
OS_EVENT* controllerSem;
OS_EVENT* MailBox;
OS_EVENT* MailBox2;


OS_FLAG_GRP *Flags;
OS_FLAG_GRP *Flags_Games;
OS_FLAG_GRP *Flags_Tutorial;
OS_FLAG_GRP *Flags_Highscores;
OS_FLAG_GRP *Flags_newHighscores;

#define Menu_Flag 0x01
#define Game_Flag 0x02
#define C1_Flag 0x04
#define C2_Flag 0x08
#define Menu2_Flag 0x10
#define Singleplayer_Flag 0x20
#define Tutorial_Flag 0x40
#define Highscores_Flag 0x80
#define newHighscores_Flag 0x100


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

int score1, score2 = 0;
int score3 = 0;
int first = 1;
int check = 1;
int random;
int X;
int leven = beginLevens;
int newHighscoreSelect;
int textWeg;

int oneTime = 1;
int verandert;

char highscores[5][20] = {
			{""},
			{""},
			{""},
			{""},
			{""},
};

char a[2];
char b[2];
char c[2];

typedef struct balk {
	int Hoogte;
} Balk;

void read_file()
{
	short int Read;
	Read = alt_up_sd_card_fopen("hoi.txt", false);

	bzero(highscores, 200);

	int i = 0;
	int k = 0;

	char buffer[1] = "";
	short int c = 1;

	c = alt_up_sd_card_read(Read);

	while (c != -1){
		if(c == '|'){		//check voor komma
			i++;
		}else{
			sprintf(buffer, "%c", c);
			strcat(highscores[i], buffer);
		}
		c = alt_up_sd_card_read(Read);		//lees volgende char
	}

	int j;
	for(j = 0; j < 5; j++){
		printf("Number is : %d data :%s\n",j , highscores[j]);
	}

	alt_up_sd_card_fclose(Read);						//sluit het bestand
}
void write_file(char input[]){
	short int Write;
	Write = alt_up_sd_card_fopen("hoi.txt", false);
	int i = 0;
	bool writing;
	if(Write < 0){
		printf("Can't open\n");
	}else{
		while(input[i] != '\0'){
			writing = alt_up_sd_card_write(Write, input[i]);
			printf("%c", input[i]);
			i++;
		}
		alt_up_sd_card_fclose(Write);
		printf("\nFile is geschreven\n");
	}

}
void sorteer(){
	int i, j, k, l, o;
	int hoogste;

	char letter1, letter2, letter3;


	char cijfers[4];

	if(score3 < 100){
		cijfers[0] = 0;
		cijfers[1] = score3 / 10;
		cijfers[2] = score3 %10;
	}else if(score3 < 10){
		cijfers[0] = 0;
		cijfers[1] = 0;
		cijfers[2] = score3;
	}else if(score3 > 100){
		cijfers[0] = score3 /100;
		cijfers[1] = score3 /10 %10;
		cijfers[2] = score3 %10;
	}


	char buffer[30] = "";
	char input[50] = "";

	for(i = 0; i < 5; i++){
		hoogste = ((highscores[i][4] - '0') *100) +
				   ((highscores[i][5] - '0') *10) +
				   (highscores[i][6] - '0');
		if(hoogste < score3){

			schuifScores(i);
			printf("score = hoger dan %de score\n", i);
			buffer[0] = a[0];
			buffer[1] = b[0];
			buffer[2] = c[0];
			buffer[3] = ' ';
			for(k = 4; k < 7; k++){
				buffer[k] = cijfers[k - 4] + '0';
			}

			for(l = 0; l < 8; l++){
				highscores[i][l] = buffer[l];
			}

			for(o = 0; o <5; o++){
				strcat(highscores[o], "|");
				strcat(input, highscores[o]);
			}

			printf("%s\nde uiteindelijke string!\n", input);
			//[i] = buffer;

			write_file(input);
			break;
		}

		else{
			printf("score is niet hoger\n");
		}


	}
	read_file();

}
void schuifScores(int nummer){
	int i;

	for(i = 5; i > nummer; i--){
		strcpy(highscores[i], highscores[i-1]);
	}

}

void collision(int aantal){
	INT8U err;

	Balk * balkje;
	Balk * balkje2;

	balkje = (Balk*) OSMboxPend(MailBox, 0, &err);
	balkje2 = (Balk*) OSMboxPend(MailBox2, 0, &err);

	//collision linker balkje
	//lange zijde
	if(ALT_x1 == 21 && (ALT_y > balkje->Hoogte - 6 && ALT_y < balkje->Hoogte + 50) || ((ALT_y + 5 > balkje->Hoogte && ALT_y < balkje->Hoogte +5) && ALT_x1 == 21) ){
		ALT_inc_x = -(ALT_inc_x);
	}
	//boven
	if(((ALT_x1 <= 20 && ALT_x1 >= 15) && ALT_y == balkje->Hoogte - 6) || ((ALT_x1 <= 20 && ALT_x1 >= 15) && ALT_y == balkje->Hoogte + 50)){
		ALT_inc_y = -(ALT_inc_y);
	}

	if(aantal == 2){
		//collision rechter balkje
		//lange zijde
		if(ALT_x2 == 299 && (ALT_y > balkje2->Hoogte - 6 && ALT_y < balkje2->Hoogte + 50) || ((ALT_y + 5 > balkje2->Hoogte && ALT_y < balkje2->Hoogte +5) && ALT_x2 == 299) ){
			ALT_inc_x = -(ALT_inc_x);
		}
		//boven
		if(((ALT_x2 <= 305 && ALT_x2 >= 300) && ALT_y == balkje2->Hoogte - 6) || ((ALT_x2 <= 305 && ALT_x2 >= 300) && ALT_y == balkje2->Hoogte + 50)){
			ALT_inc_y = -(ALT_inc_y);
		}
	}



}

void Game(void* pdata) {

	Balk * balkje;
	Balk * balkje2;



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

	balkje = (Balk*) OSMboxPend(MailBox, 0, &err);
	balkje2 = (Balk*) OSMboxPend(MailBox2, 0, &err);
	
	VGA_box (316, 5, 319, 235, zwart); 					// singleplayer balk weghalen voor de zekerheid
	//ALT_x1 = 0; ALT_x2 = 5/* ALTERA = 6 chars */; ALT_y = 0; ALT_inc_x = 1; ALT_inc_y = 1;

	//char_buffer_x = 79; char_buffer_y = 59;

	//blue_x1 = 28; blue_x2 = 52; blue_y1 = 26; blue_y2 = 34;

	while (1) {
		OSFlagPend(Flags_Games, Game_Flag, OS_FLAG_WAIT_CLR_ANY, 0, &err);

		//printf("Game flag is set\n");





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

			collision(2);

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
			random = 0;
		}else if(random == 0){
			ALT_inc_y = 1;
			random = 1;
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

void dakjeOmhoog(int omhoogX, int omhoogY){
	VGA_text(omhoogX, omhoogY, "/\\"); //= dakje omhoog
}

void dakjeOmlaag(int omlaagX, int omlaagY){
	VGA_text(omlaagX, omlaagY, "\\/"); //= dakje omlaag
}

void endSingleplayer(){
	INT8U err;

	printf("end Singleplayer\n");

	clearScreen();

	OSFlagPost(Flags, C1_Flag, OS_FLAG_SET, &err);

	VGA_text(35, 25, "GAME OVER");
	OSTimeDlyHMSM(0, 0, 40, 0);
	VGA_text(35, 25, "         ");


	VGA_text(6,5, "           ");

	leven = beginLevens;

	if(score3 > 0){

		clearScreen();
		OSFlagPost(Flags_newHighscores, newHighscores_Flag, OS_FLAG_CLR, &err);
		OSFlagPost(Flags_Games, Singleplayer_Flag, OS_FLAG_SET, &err);

	} else {


		//knop links is bevestig van alle 3 letters in 1 keer
		//knop rechts is letter naar rechts, ook naar bevestig, eenmaal op bevestig en druk nogmaals op rechts, opnieuw bij de meeste linkse letter.



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
	

	}

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
				endSingleplayer();
			}
		}





	}
	
}

void VGAtextClear(){
	int clear;
	
	for (clear = 0; clear < 60; clear++){
		VGA_text(1, clear, "                                                                       ");
	}
}

void tutorialUitleg(){
	INT8U err;
	int tutorialText;
	tutorialText = 0;
	
	if(controller(3) == 2){
		tutorialText++;
		if (tutorialText >= 7){
			tutorialText = 0;
		}
	}
	
	if(tutorialText == 0){
		VGAtextClear();
		VGA_text(10, 5, ""); // niks
	} else if (tutorialText == 1){
		VGAtextClear();
		VGA_text(10, 38, "De besturing is als volgt:");	
	} else if (tutorialText == 2){
		VGAtextClear();
		VGA_text(10, 40, "De linker balk wordt bestuurd door het op en neer bewegen");
		VGA_text(10, 41, "van switch 17. Bij de multiplayer wordt ook nog vereist om");
		VGA_text(10, 42, "de linker knop in te houden, anders ben je af. Dit geldt ook");
		VGA_text(10, 43, "voor de rechter knop voor de rechter balk.");
	} else if (tutorialText == 3){
		VGAtextClear();
		VGA_text(10, 45, "De rechter balk wordt bestuurd KEY3 en KEY2. KEY3 = omlaag,");
		VGA_text(10, 46, "KEY2 = omhoog.");	
	} else if (tutorialText == 4){
		VGAtextClear();
		VGA_text(10, 5, "Dit spel heeft een Singleplayer en een Multiplayer.");
	} else if (tutorialText == 5){
		VGAtextClear();
		VGA_text(10, 7, "In Singleplayer speel je tegen de computer, waarbij je");
		VGA_text(10, 8, "zoveel mogelijk aantal punten moet bereiken door de bal");
		VGA_text(10, 9, "tegen de muur te kaatsen. Hierbij heb je 3 levens.");		
	} else if (tutorialText == 6){
		VGAtextClear();
		VGA_text(10, 11, "In Multiplayer gaat het er om dat je de bal in het doel");
		VGA_text(10, 12, "van de tegenstander krijgt. Het doel bevindt zich achter");
		VGA_text(10, 13, "het balkje van de tegenstander. Wanneer je scoort krijg je");
		VGA_text(10, 14, "1 punt.");	
	}
	
	VGA_text(10, 56, "Linker knop = ");
	VGA_text(10, 57, "Volgende tutorial uitleg");
	
	terugMenu();
}

void endTutorial(){
	INT8U err;

	printf("end Tutorial\n");

	//clearScreen();
	VGAtextClear();
	
	VGA_text(63, 56, "              ");
	VGA_text(63, 57, "               ");

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

void endNewHighscores(){
	
	INT8U err;

	printf("end newHighscores\n");

	//clearScreen();
	
	for (textWeg = 0; textWeg < 60; textWeg++){
	VGA_text(1, textWeg, "                                                                                ");
	}
	

	OSFlagPost(Flags, C1_Flag, OS_FLAG_SET, &err);

	VGA_text(35, 25, "Einde Nieuwe Highscores");
	OSTimeDlyHMSM(0, 0, 40, 0);
	VGA_text(35, 25, "                           ");


	score3 = 0;

	teken_menu(1);

	OSFlagPost(Flags, Menu_Flag + Menu2_Flag, OS_FLAG_CLR, &err);

	OSFlagPost(Flags_newHighscores, newHighscores_Flag, OS_FLAG_SET, &err);
	
}

void clearHighscoreText(){
	
	VGA_text(20, 35, "  ");
	VGA_text(30, 35, "  ");
	VGA_text(40, 35, "  ");
	VGA_text(50, 35, "           ");
	
	VGA_text(20, 30, "                                                                        ");
	VGA_text(20, 40, "                                                                        ");
	
}

void newHighscores(void *pdata){
	INT8U err;
	a[0] = 'A';
	a[1] = '\0';

	b[0] = 'B';
	b[1] = '\0';

	c[0] = 'C';
	c[1] = '\0';
	newHighscoreSelect = 1;
	
	while(1){
		OSFlagPend(Flags_newHighscores, newHighscores_Flag, OS_FLAG_WAIT_CLR_ANY, 0, &err);

		VGA_text(35, 15, "New Highscore!");
		
		if (controller(3) == 3){
			if(verandert != 0){
				newHighscoreSelect++;
				oneTime = 1;
				verandert = 0;
				VGA_box(19 *4, 34 * 4, 22 * 4, 37 * 4, zwart);
				VGA_box(29 * 4, 34 * 4, 32 * 4, 37 * 4, zwart);
				VGA_box(39 * 4, 34 * 4, 42 * 4, 37 * 4, zwart);
				VGA_box(49 * 4, 34 * 4, 59 * 4, 37 * 4, zwart);
			}			
			if (newHighscoreSelect >= 5){
			newHighscoreSelect = 1;
			}
		}

		if(controller(2) == 1 && newHighscoreSelect == 1){
			if(a[0] >= '[' && controller(2) == 1){
				a[0] = '@';
			}
			a[0]++;
		} else if (controller(2) == 0 && newHighscoreSelect == 1){
			if (a[0] <= '@' && controller(2) == 0){
				a[0] = '[';
			}
			a[0]--;
		}
		
		if (controller(2) == 1 && newHighscoreSelect == 2){
			if(b[0] >= '[' && controller(2) == 1){
				b[0] = '@';
			}
			b[0]++;
		} else if (controller(2) == 0 && newHighscoreSelect == 2){
			if(b[0] <= '@' && controller(2) == 0){
				b[0] = '[';
			}
			b[0]--;
		}
		
		if (controller(2) == 1 && newHighscoreSelect == 3){
			if(c[0] >= '[' && controller(2) == 1){
				c[0] = '@';
			}
			c[0]++;
		} else if (controller(2) == 0 && newHighscoreSelect == 3){
			if(c[0] <= '@' && controller(2) == 1){
				c[0] = '[';
			}
			c[0]--;
		}		
		
		if (newHighscoreSelect == 1){
			if(oneTime == 1){
				VGA_box(19 *4, 34 * 4, 22 * 4, 37 * 4, geel);
				oneTime = 0;
				verandert = 1;
			}
		} else if (newHighscoreSelect == 2){
			if(oneTime == 1){
				VGA_box(29 * 4, 34 * 4, 32 * 4, 37 * 4, geel);
				oneTime = 0;
				verandert = 1;
			}
		} else if (newHighscoreSelect == 3){
			if(oneTime == 1){
				VGA_box(39 * 4, 34 * 4, 42 * 4, 37 * 4, geel);
				oneTime = 0;
				verandert = 1;
			}
		} else if (newHighscoreSelect == 4){
			if(oneTime == 1){
				VGA_box(49 * 4, 34 * 4, 59 * 4, 37 * 4, geel);
				oneTime = 0;
				verandert = 1;
			}
		}
		
		dakjeOmhoog(20, 30);
		VGA_text(20, 35, a);
		dakjeOmlaag(20, 40);
		
		dakjeOmhoog(30, 30);
		VGA_text(30, 35, b);
		dakjeOmlaag(30, 40);
		
		dakjeOmhoog(40, 30);
		VGA_text(40, 35, c);
		dakjeOmlaag(40, 40);
		
		VGA_text(50, 35, "Bevestig");
		


					//controller(3) == 3 ??	
		if (controller(3) == 2 && newHighscoreSelect == 4) {
			//save highscores naar SD kaart
			sorteer();

			VGA_box(48 * 4, 33 * 4, 60 * 4, 37 * 4, zwart); //omdat je alleen terug kan op bevestig, moet daar het gele selecteer gebied zitten, dus die schrijven we alleen weg.
			clearHighscoreText();
			
			endNewHighscores();

		}
		
		OSTimeDly(20);
		
	}
	
}

void endHighscores(){
	INT8U err;

	printf("end Highscores\n");

	//clearScreen();

	VGA_text(35, 18, "                 ");
	VGA_text(35, 20, "                 ");
	VGA_text(35, 22, "                 ");
	VGA_text(35, 24, "                 ");
	VGA_text(35, 26, "                 ");
	
	VGA_text(63, 56, "              ");
	VGA_text(63, 57, "               ");
	
	VGA_text(35, 15, "                          ");
	VGA_text(35, 25, "Einde Highscores");
	OSTimeDlyHMSM(0, 0, 40, 0);
	VGA_text(35, 25, "                  ");




	teken_menu(3);

	VGA_box (316, 4, 319, 235, zwart); 					// singleplayer/tutorial balk weghalen voor de zekerheid

	VGA_box(ALT_x1, ALT_y, ALT_x1 + 5, ALT_y + 5, zwart); // erase


	ALT_x1 = 0;
	ALT_x2 = 165;
	ALT_y = 100;
	ALT_inc_x = -1;
	ALT_inc_y = 1;

	first = 1;

	OSFlagPost(Flags, Menu_Flag + Menu2_Flag, OS_FLAG_CLR, &err);

	OSFlagPost(Flags_Highscores, Highscores_Flag, OS_FLAG_SET, &err);

}

void Highscores(void* pdata){
	
	INT8U err;
	int i, j;
	int ID;
	int count;
	int q;
	
	while(1){
		OSFlagPend(Flags_Highscores, Highscores_Flag, OS_FLAG_WAIT_CLR_ANY, 0, &err);
		VGA_text(35, 17, "Highscores");
		


		// highscores 1 t/m 5

		for(j = 0; j < 5; j++){
			VGA_text(38, 20 + 2 * j, highscores[j]);
		}
		
		VGA_text(35, 20, "1.");
		VGA_text(35, 22, "2.");
		VGA_text(35, 24, "3.");
		VGA_text(35, 26, "4.");
		VGA_text(35, 28, "5.");

		terugMenu();
		
		
		if (controller(3) == 3) {
			endHighscores();
		}
		
		OSTimeDly(1);
		
	}
	
}

void terugMenu(){
	
	INT8U err;
	
	VGA_text(63, 56, "Rechter knop =");
	VGA_text(63, 57, "terug naar menu");
	
}


