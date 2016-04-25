#include <stdio.h>
#include "includes.h"
#include "altera_up_avalon_parallel_port.h"

int gameModeMenu = 1;
int vorige = 0;
int controller(int ID);

short menuTextKleur = 0xffff00;

int xLinks = 100;
int xRechts = 180;
int xBoven = 70;
int xOnder = 90;

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
		} else if (controller(ID) == 0){
			gameModeMenu--;
		}


		selecteerMenu();
		OSTimeDly(10);
	}
	VGA_text (xMenu, yMenu, "Singleplayer");
	VGA_text (xMenu, yMenu+4, "Multiplayer");
	VGA_text (xMenu, yMenu+8, "Highscores");
	VGA_text (xMenu, yMenu+12, "Tutorial");
}


void selecteerMenu(){

	//  VGA_box(x1, y1, x2, y2, menuTextKleur);
	if (gameModeMenu == 1){
		tekenBox(xLinks, xBoven, xRechts, xOnder, 0xffff00);
		tekenBox2(xLinks, xBoven, xRechts, xOnder, 0x000000);
		VGA_text (xMenu, yMenu, "Singleplayer");
	} else if (gameModeMenu == 2){
		tekenBox(xLinks, xBoven+20, xRechts, xOnder+20, 0xffff00);
		tekenBox2(xLinks, xBoven+20, xRechts, xOnder+20, 0x000000);
		VGA_text (xMenu, yMenu+4, "Multiplayer");
	} else if (gameModeMenu == 3){
		tekenBox(xLinks, xBoven+40, xRechts, xOnder+40, 0xffff00);
		tekenBox2(xLinks, xBoven+40, xRechts, xOnder+40, 0x000000);
		VGA_text (xMenu, yMenu+8, "Highscores");
	} else if (gameModeMenu == 4){
		tekenBox(xLinks, xBoven+60, xRechts, xOnder+60, 0xffff00);
		tekenBox2(xLinks, xBoven+60, xRechts, xOnder+60, 0x000000);
		VGA_text (xMenu, yMenu+12, "Tutorial");
	}
	
}

void tekenBox(int Links, int Boven, int Rechts, int Onder, short Kleur){
	
	VGA_box(Links, Boven, Rechts, Onder, Kleur);
	
}

void tekenBox2(int Links, int Boven, int Rechts, int Onder, short Kleur){
	Links = Links - 1;
	Boven = Boven - 1;
	Rechts = Rechts - 1;
	Onder = Onder -1;
	
	VGA_box(Links, Boven, Rechts, Onder, Kleur);
}
