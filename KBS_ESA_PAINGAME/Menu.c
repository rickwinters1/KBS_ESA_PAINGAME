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

short zwart = 0;
short geel = 0xffff00;

void menu(void* pdata){
	int ID = (int*)pdata;

	VGA_text (xLinks, xBoven, "Singleplayer");
	VGA_text (xLinks, xBoven + 4, "Multiplayer");
	VGA_text (xLinks, xBoven+8, "Highscores");
	VGA_text (xLinks, xBoven+12, "Tutorial");


	while(1){
		if (controller(ID) == 1){
			gameModeMenu++;
			if (gameModeMenu >= 5){
				gameModeMenu = 1;
			}
			OSSemPost(menuSem);
			printf("voor post1 \n");

		} else if (controller(ID) == 0){
			gameModeMenu--;
			if (gameModeMenu <= 0){
				gameModeMenu = 4;
			}

			OSSemPost(menuSem);
			printf("voor post2 \n");

		}
		OSTimeDlyHMSM(0,0,0,600);
	}

}

void selecteerMenu(void *pdata){
	INT8U err;

	while(1){
		OSSemPend(menuSem, 0, &err);
		if (gameModeMenu == 1){
			VGA_box(xLinks*4, xBoven*4, xRechts*4, xOnder*4, geel);
		} else if (gameModeMenu == 2){

		} else if (gameModeMenu == 3){

		} else if (gameModeMenu == 4){

		}

		OSTimeDly(10);
	}
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
