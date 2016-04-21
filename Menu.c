#include <stdio.h>
#include "includes.h"
#include "altera_up_avalon_parallel_port.h"

void menu(void* pdata){
	int ID = (int*)pdata;
	int gameModeMenu = 1;
	int xMenu = 150;
	int yMenu = 100;
	
	if (controller(ID) == 1){
		gameModeMenu+1;
	} else if (controller(ID) == 0){
		gameModeMenu-1;
	}
	
	VGA_text (xMenu, yMenu, "Singeplayer");
	VGA_text (xMenu, yMenu+25, "Multiplayer");
	VGA_text (xMenu, yMenu+50, "Highscores");
	VGA_text (xMenu, yMenu+75, "Tutorial");
	
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

int controller(int ID){
	alt_up_parallel_port_dev * KEY_ptr = alt_up_parallel_port_open_dev ("/dev/Pushbuttons");
	int KEY_value;
	INT8U err;

	KEY_value = alt_up_parallel_port_read_data(KEY_ptr); 		// read the pushbutton KEY values

	if(KEY_value != 0){
		while(KEY_value == 8){
		return 1;
		}
		while(KEY_value == 4){
			return 0;
		}
	}else{
		return 2;
	}

}

void selecteerMenu(){
	short menuTextKleur = 0xFF0000;
	
	int xLinks = 140;
	int xRechts = 240;
	int xBoven = 100;
	int xOnder = 150;
	
	VGA_box(xLinks, xBoven, xRechts, xBoven + 1, menuTextKleur); // bovenste
	VGA_box(xLinks, xOnder, xRechts, xOnder + 1, menuTextKleur); // onderste
	VGA_box(xLinks, xBoven, xLinks + 1, xOnder, menuTextKleur); // linkse
	VGA_box(xRechts, xBoven, xRechts + 1, xOnder, menuTextKleur); // rechtse
	
}