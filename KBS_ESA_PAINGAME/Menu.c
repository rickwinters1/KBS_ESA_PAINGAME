#include <stdio.h>
#include "includes.h"
#include "altera_up_avalon_parallel_port.h"

OS_EVENT* menuSem;

int gameModeMenu = 1;
int vorige = 0;
int controller(int ID);

int xLinks = 30;
int xRechts = 45;
int xBoven = 18;
int xOnder = 21;

int xMenu = 30 +2;
int yMenu = 18 +1;

short zwart = 0;
short geel = 0xffff00;

int eenkeer = 1;
int last;

void menu(void* pdata){
	int ID = (int*)pdata;


	VGA_text (xMenu, yMenu, "Singleplayer");
	VGA_text (xMenu, yMenu + 4, "Multiplayer");
	VGA_text (xMenu, yMenu+8, "Highscores");
	VGA_text (xMenu, yMenu+12, "Tutorial");


	while(1){
		if (controller(ID) == 1){
			if(last != 1){
				gameModeMenu++;
				last=1;
				eenkeer = 1;
				printf("heeft 1 gereturned \n");
			}
			if (gameModeMenu >= 5){
				gameModeMenu = 1;
			}
		} else if (controller(ID) == 0){
			if(last !=0){
				gameModeMenu--;
				last=0;
				eenkeer = 1;
				printf("heeft 0 gereturned \n");
			}
			if (gameModeMenu <= 0){
				gameModeMenu = 4;
			}
		}
		OSTimeDly(1);
	}

}

void selecteerMenu(void *pdata){
	INT8U err;

	while(1){
		if (gameModeMenu == 1){
			if(eenkeer == 1){
				clearScreen();
				VGA_box(xLinks*4, xBoven*4, xRechts*4, xOnder*4, geel);
				VGA_text (xMenu, yMenu, "Singleplayer");
				eenkeer = 0;
				vorige = 1;
			}
		} else if (gameModeMenu == 2){
			if(eenkeer == 1){
				clearScreen();
				VGA_box(xLinks*4, xBoven*4 + 16, xRechts*4, xOnder*4 + 16, geel);
				VGA_text (xMenu, yMenu + 4, "Multiplayer");
				eenkeer = 0;
				vorige = 2;

			}
		} else if (gameModeMenu == 3){
			if(eenkeer == 1){
				clearScreen();
				VGA_box(xLinks*4, xBoven*4 + 32, xRechts*4, xOnder*4 + 32, geel);
				VGA_text (xMenu, yMenu +  8, "Highscores");
				eenkeer = 0;
				vorige = 3;

			}
		} else if (gameModeMenu == 4){
			if(eenkeer == 1){
				clearScreen();
				VGA_box(xLinks*4, xBoven*4 + 48, xRechts*4, xOnder*4 + 48, geel);
				VGA_text (xMenu, yMenu + 12, "Tutorial");
				eenkeer = 0;
				vorige = 4;

			}
		}

		OSTimeDly(10);
	}
}

void clearScreen(){
	VGA_box(20,20, 200,200, 0);
}

void tekenBox(int Links, int Boven, int Rechts, int Onder, short Kleur){
	VGA_box(Links, Boven, Rechts, Onder, Kleur);
}
// tekenBox en tekenBox2 maken samen het visuele effect van het selecteren van een menu optie.
void tekenBox2(int Links, int Boven, int Rechts, int Onder, short Kleur){
	Links = Links + 1;
	Boven = Boven + 1;
	Rechts = Rechts - 1;
	Onder = Onder -1;

	VGA_box(Links, Boven, Rechts, Onder, Kleur);
}
