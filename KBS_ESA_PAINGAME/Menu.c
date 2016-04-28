#include <stdio.h>
#include "includes.h"
#include "altera_up_avalon_parallel_port.h"

OS_EVENT* menuSem;

int gameModeMenu = 1;
int vorige = 0;
int controller(int ID);

short menuTextKleur = 0xffff00;

int xLinks = 130;
int xRechts = 180;
int xBoven = 70;
int xOnder = 80;

// 0xffff00 = wit
// 0x000000 = zwart
int xMenu = 32;
int yMenu = 18;

void menu(void* pdata){
	int ID = (int*)pdata;

	while(1){
		if (gameModeMenu <= 0){
			gameModeMenu = 4;
		} else if (gameModeMenu >= 5){
			gameModeMenu = 1;
		}

		if (controller(ID) == 1){
			gameModeMenu++;
			OSSemPost(menuSem);
			printf("voor post1 \n");

		} else if (controller(ID) == 0){
			gameModeMenu--;
			OSSemPost(menuSem);
			printf("voor post2 \n");

		}
		OSTimeDly(10);
	}
	VGA_text (xMenu, yMenu, "Singleplayer");
	VGA_text (xMenu, yMenu+4, "Multiplayer");
	VGA_text (xMenu, yMenu+8, "Highscores");
	VGA_text (xMenu, yMenu+12, "Tutorial");
}

void selecteerMenu(void *pdata){
	INT8U err;

	while(1){
		OSSemPend(menuSem, 0, &err);
		if (gameModeMenu == 1){
			tekenBox(xLinks, xBoven, xRechts, xOnder, 0xffff00);
			tekenBox2(xLinks, xBoven, xRechts, xOnder, 0x000000);
			VGA_text (xMenu, yMenu, "Singleplayer");
		} else if (gameModeMenu == 2){
			tekenBox(xLinks, xBoven+10, xRechts, xOnder+10, 0xffff00);
			tekenBox2(xLinks, xBoven+10, xRechts, xOnder+10, 0x000000);
			VGA_text (xMenu, yMenu+4, "Multiplayer");
		} else if (gameModeMenu == 3){
			tekenBox(xLinks, xBoven+20, xRechts, xOnder+20, 0xffff00);
			tekenBox2(xLinks, xBoven+20, xRechts, xOnder+20, 0x000000);
			VGA_text (xMenu, yMenu+8, "Highscores");
		} else if (gameModeMenu == 4){
			tekenBox(xLinks, xBoven+30, xRechts, xOnder+30, 0xffff00);
			tekenBox2(xLinks, xBoven+30, xRechts, xOnder+30, 0x000000);
			VGA_text (xMenu, yMenu+12, "Tutorial");
		}
		OSTimeDly(10);
	}
}

void tekenBox(int Links, int Boven, int Rechts, int Onder, short Kleur){
	VGA_box(Links, Boven, Rechts, Onder, Kleur);
}
// tekenBox en tekenBox2 maken samen het visuele effect van het selecteren van een menu optie.
void tekenBox2(int Links, int Boven, int Rechts, int Onder, short Kleur){
	Links = Links - 1;
	Boven = Boven - 1;
	Rechts = Rechts - 1;
	Onder = Onder -1;

	VGA_box(Links, Boven, Rechts, Onder, Kleur);
}
