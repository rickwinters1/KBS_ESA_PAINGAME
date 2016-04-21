#include <stdio.h>
#include "includes.h"
#include "altera_up_avalon_parallel_port.h"

void menu(void* pdata){
	int ID = (int*)pdata;
	int gameModeMenu = 1;
	int xMenu = 30;
	int yMenu = 25;

	if (controller(ID) == 1){
		gameModeMenu+1;
	} else if (controller(ID) == 0){
		gameModeMenu-1;
	}
	
	VGA_text (xMenu, yMenu, "Singeplayer");
	VGA_text (xMenu, yMenu+4, "Multiplayer");
	VGA_text (xMenu, yMenu+8, "Highscores");
	VGA_text (xMenu, yMenu+12, "Tutorial");

	if (gameModeMenu <= 0){
		gameModeMenu = 4;
	} else if (gameModeMenu == 1){
		selecteerMenu();
	} else if (gameModeMenu == 2){
		
	} else if (gameModeMenu == 3){
		
	} else if (gameModeMenu == 4){
		
	} else if (gameModeMenu >= 5){
		gameModeMenu = 1;
	}


}


void selecteerMenu(){
	short menuTextKleur = 0xffff00;
	
	int xLinks = 140;
	int xRechts = 240;
	int xBoven = 100;
	int xOnder = 150;

	

	VGA_box(xLinks, xBoven, xRechts, xBoven + 1, menuTextKleur); // bovenste
	VGA_box(xLinks, xOnder, xRechts, xOnder + 1, menuTextKleur); // onderste
	VGA_box(xLinks, xBoven, xLinks + 1, xOnder, menuTextKleur); // linkse
	VGA_box(xRechts, xBoven, xRechts + 1, xOnder, menuTextKleur); // rechtse
	printf("AN PRINTEN");
}
