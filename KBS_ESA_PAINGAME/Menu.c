#include <stdio.h>
#include "includes.h"
#include "altera_up_avalon_parallel_port.h"

OS_EVENT* menuSem;
OS_EVENT* gameSem;
OS_EVENT* controllerSem;
OS_FLAG_GRP *Flags;
OS_FLAG_GRP *Flags_Games;
OS_FLAG_GRP *Flags_Tutorial;
OS_FLAG_GRP *Flags_Highscores;



#define Menu_Flag 0x01
#define Game_Flag 0x02
#define C1_Flag 0x04
#define C2_Flag 0x08
#define Menu2_Flag 0x10
#define Singleplayer_Flag 0x20
#define Tutorial_Flag 0x40
#define Highscores_Flag 0x80

#define Zwart  0x000000
#define Wit  0xffffff
#define geel 0xffff00
#define rood 0xf800

int gameModeMenu = 1;
int vorige = 0;
int controller(int ID);

int xLinks = 32;
int xRechts = 47;
int xBoven = 18;
int xOnder = 21;

int xMenu = 32 +2;
int yMenu = 18 +1;

int eenkeer = 1;
int onePress = 1;
int last;
int changed;

void menu(void* pdata){
	int ID = (int*)pdata;
	INT8U err;

	teken_menu();

	while(1){
		OSFlagPend(Flags, Menu_Flag, OS_FLAG_WAIT_CLR_ALL, 0, &err);

		if (controller(ID) == 1){
			if(changed != 0){
				gameModeMenu++;
				eenkeer = 1;
				changed=0;
				printf("heeft 1 gereturned \n");
			}
			if (gameModeMenu >= 5){
				gameModeMenu = 1;
			}
		} else if (controller(ID) == 0){
			if(changed !=0){
				gameModeMenu--;
				last=0;
				changed=0;
				eenkeer = 1;
				printf("heeft 0 gereturned \n");
			}
			if (gameModeMenu <= 0){
				gameModeMenu = 4;
			}
		}
		OSTimeDly(20);
	}

}

void selecteerMenu(void *pdata){
	INT8U err;
	int ID = (int*)pdata;



	while(1){
	OSFlagPend(Flags, Menu2_Flag, OS_FLAG_WAIT_CLR_ALL, 0, &err);

		if (gameModeMenu == 1){
			if(eenkeer == 1){
				clearScreen();
				VGA_box(xLinks*4, xBoven*4, xRechts*4, xOnder*4, rood);
				VGA_text (xMenu, yMenu, "Singleplayer");
				eenkeer = 0;
				changed = 1;
			}
			if(controller(ID) == 2){
				clearScreen();
				clearText();
				printf("start Singleplayer\n");
				OSFlagPost(Flags,C1_Flag, OS_FLAG_CLR, &err);
				OSFlagPost(Flags_Games, Singleplayer_Flag, OS_FLAG_CLR, &err);
				OSFlagPost(Flags, Menu_Flag + Menu2_Flag, OS_FLAG_SET, &err);
			}
		} else if (gameModeMenu == 2){
			if(eenkeer == 1){
				clearScreen();
				VGA_box(xLinks*4, xBoven*4 + 16, xRechts*4, xOnder*4 + 16, rood);
				VGA_text (xMenu, yMenu + 4, "Multiplayer");
				eenkeer = 0;
				changed = 1;
			}
			if(controller(ID) == 2 || controller(ID) == 1){
				clearScreen();
				clearText();
				printf("start game\n");
				OSFlagPost(Flags, C1_Flag + C2_Flag, OS_FLAG_CLR, &err);
				OSFlagPost(Flags_Games,Game_Flag, OS_FLAG_CLR, &err);
				OSFlagPost(Flags, Menu_Flag + Menu2_Flag, OS_FLAG_SET, &err);
			}

		} else if (gameModeMenu == 3){
			if(eenkeer == 1){
				clearScreen();
				VGA_box(xLinks*4, xBoven*4 + 32, xRechts*4, xOnder*4 + 32, rood);
				VGA_text (xMenu, yMenu +  8, "Highscores");
				eenkeer = 0;
				changed = 1;				
			}
			if(controller(ID) == 2){
				clearScreen();
				clearText();
				printf("Start Highscores\n");
				OSFlagPost(Flags_Highscores, Highscores_Flag, OS_FLAG_CLR, &err);
				OSFlagPost(Flags, Menu_Flag + Menu2_Flag, OS_FLAG_SET, &err);
			}
		} else if (gameModeMenu == 4){
			if(eenkeer == 1){
				clearScreen();
				VGA_box(xLinks*4, xBoven*4 + 48, xRechts*4, xOnder*4 + 48, rood);
				VGA_text (xMenu, yMenu + 12, "Tutorial");
				eenkeer = 0;
				changed = 1;

			}
			
			if(controller(ID) == 2){
				clearScreen();
				clearText();
				printf("Start Tutorial\n");
				OSFlagPost(Flags, C1_Flag, OS_FLAG_CLR, &err);
				OSFlagPost(Flags_Tutorial,Tutorial_Flag, OS_FLAG_CLR, &err);
				OSFlagPost(Flags, Menu_Flag + Menu2_Flag, OS_FLAG_SET, &err);
			}
		}

		OSTimeDly(20);
	}
}

void clearScreen(){
	VGA_box(25,70, 200,135, 0);
	del_middenlijn();
}

void clearText(){
	VGA_text (xMenu, yMenu, "            ");
	VGA_text (xMenu, yMenu + 4, "           ");
	VGA_text (xMenu, yMenu+8, "          ");
	VGA_text (xMenu, yMenu+12, "        ");
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

void teken_menu(int ID){
	VGA_text (xMenu, yMenu, "Singleplayer");
	VGA_text (xMenu, yMenu + 4, "Multiplayer");
	VGA_text (xMenu, yMenu+8, "Highscores");
	VGA_text (xMenu, yMenu+12, "Tutorial");
	if(ID == 1){
		VGA_box(xLinks*4, xBoven*4, xRechts*4, xOnder*4, rood);
	}else if(ID == 2){
		VGA_box(xLinks*4, xBoven*4 + 16, xRechts*4, xOnder*4 + 16, rood);
	}else if(ID == 3){
		VGA_box(xLinks*4, xBoven*4 + 32, xRechts*4, xOnder*4 + 32, rood);
	}else if(ID == 4){
		VGA_box(xLinks*4, xBoven*4 + 48, xRechts*4, xOnder*4 + 48, rood);
	}


}
