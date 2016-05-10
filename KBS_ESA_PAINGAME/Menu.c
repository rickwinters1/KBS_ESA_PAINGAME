#include <stdio.h>
#include "includes.h"
#include "altera_up_avalon_parallel_port.h"

OS_EVENT* menuSem;

int gameModeMenu = 1;
int vorige = 0;
int controller(int ID);

int xLinks = 120/4;
int xRechts = 180/4;
int xBoven = 70/4;
int xOnder = 85/4;

short zwart = 0;
short geel = 0xffff00;

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
		OSTimeDlyHMSM(0,0,0,200);
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
			if(vorige == 4){
				tekenBox(xLinks *4, xBoven+30 * 4, xRechts *4, xOnder+30 *4, zwart);
			}else if(vorige == 3){
				tekenBox(xLinks *4, xBoven+20 *4, xRechts *4, xOnder+20 *4, zwart);
			}
			tekenBox(xLinks *4, xBoven *4, xRechts *4, xOnder *4, geel);
			tekenBox2(xLinks *4, xBoven *4, xRechts *4, xOnder *4, zwart);
			VGA_text (xMenu, yMenu, "Singleplayer");
			vorige = 1;
		} else if (gameModeMenu == 2){
			if(vorige == 1){
				tekenBox(xLinks *4, xBoven *4, xRechts *4, xOnder *4, zwart);
			}else if(vorige == 3){
				tekenBox(xLinks *4, xBoven+20 *4, xRechts *4, xOnder+20 *4, zwart);
			}
			tekenBox(xLinks *4, xBoven+10 *4, xRechts *4, xOnder+10 *4, geel);
			tekenBox2(xLinks *4, xBoven+10 *4, xRechts *4, xOnder+10 *4, zwart);
			VGA_text (xMenu, yMenu+4, "Multiplayer");
			vorige = 2;
		} else if (gameModeMenu == 3){
			if(vorige == 2){
				tekenBox(xLinks, xBoven+10, xRechts, xOnder+10, zwart);
			}else if(vorige == 4){
				tekenBox(xLinks, xBoven+30, xRechts, xOnder+30, zwart);
			}
			tekenBox(xLinks, xBoven+20, xRechts, xOnder+20, geel);
			tekenBox2(xLinks, xBoven+20, xRechts, xOnder+20, zwart);
			VGA_text (xMenu, yMenu+8, "Highscores");
			vorige = 3;
		} else if (gameModeMenu == 4){
			if(vorige == 1){
				tekenBox(xLinks, xBoven, xRechts, xOnder, zwart);
			}else if(vorige == 3){
				tekenBox(xLinks, xBoven+20, xRechts, xOnder+20, zwart);
			}
			tekenBox(xLinks, xBoven+30, xRechts, xOnder+30, geel);
			tekenBox2(xLinks, xBoven+30, xRechts, xOnder+30, zwart);
			VGA_text (xMenu, yMenu+12, "Tutorial");
			vorige = 4;
		}
//		if(vorige == 1){
//			tekenBox(xLinks, xBoven, xRechts, xOnder, zwart);
//		}else if(vorige == 2){
//			tekenBox(xLinks, xBoven+10, xRechts, xOnder+10, zwart);
//		}else if(vorige == 3){
//			tekenBox(xLinks, xBoven+20, xRechts, xOnder+20, zwart);
//		}else if(vorige == 4){
//			tekenBox(xLinks, xBoven+30, xRechts, xOnder+30, zwart);
//		}
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
