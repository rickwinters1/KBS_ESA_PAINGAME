#include <stdio.h>
#include "includes.h"
#include "altera_up_avalon_parallel_port.h"

int gameModeMenu = 1;

int controller(int ID);

void menu(void* pdata){
	int ID = (int*)pdata;

	int xMenu = 32;
	int yMenu = 18;


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
		OSTimeDly(1);
	}
	VGA_text (xMenu, yMenu, "Singeplayer");
	VGA_text (xMenu, yMenu+4, "Multiplayer");
	VGA_text (xMenu, yMenu+8, "Highscores");
	VGA_text (xMenu, yMenu+12, "Tutorial");






}


void selecteerMenu(){
	short menuTextKleur = 0xffff00;
	
	int xLinks = 100;
	int xRechts = 180;
	int xBoven = 70;
	int xOnder = 90;
	//  VGA_box(x1, y1, x2, y2, menuTextKleur);
	if (gameModeMenu == 1){
		VGA_box(xLinks, xBoven, xRechts, xBoven + 1, menuTextKleur); // bovenste
		VGA_box(xLinks, xOnder, xRechts, xOnder + 1, menuTextKleur); // onderste
		VGA_box(xLinks, xBoven, xLinks + 1, xOnder, menuTextKleur); // linkse
		VGA_box(xRechts, xBoven, xRechts + 1, xOnder, menuTextKleur); // rechtse
	} else if (gameModeMenu == 2){
		VGA_box(xLinks, xOnder, xRechts, xOnder + 1, menuTextKleur); // bovenste
		VGA_box(xLinks, xOnder + 20, xRechts, xOnder + 1 + 20, menuTextKleur); // onderste
		VGA_box(xLinks, xBoven + 20, xLinks + 1 , xOnder + 20, menuTextKleur); // linkse
		VGA_box(xRechts, xBoven + 20, xRechts + 1, xOnder + 20, menuTextKleur); // rechtse
	} else if (gameModeMenu == 3){
		VGA_box(xLinks, xOnder + 20, xRechts, xOnder + 1 + 20, menuTextKleur); // bovenste
		VGA_box(xLinks, xOnder + 40, xRechts, xOnder + 1 + 40, menuTextKleur); // onderste
		VGA_box(xLinks, xBoven + 40, xLinks + 1 , xOnder + 40, menuTextKleur); // linkse
		VGA_box(xRechts, xBoven + 40, xRechts + 1, xOnder + 40, menuTextKleur); // rechtse
	} else if (gameModeMenu == 4){
		VGA_box(xLinks, xOnder + 40, xRechts, xOnder + 1 + 40, menuTextKleur); // bovenste
		VGA_box(xLinks, xOnder + 60, xRechts, xOnder + 1 + 60, menuTextKleur); // onderste
		VGA_box(xLinks, xBoven + 60, xLinks + 1 , xOnder + 60, menuTextKleur); // linkse
		VGA_box(xRechts, xBoven + 60, xRechts + 1, xOnder + 60, menuTextKleur); // rechtse
	}
	
}
