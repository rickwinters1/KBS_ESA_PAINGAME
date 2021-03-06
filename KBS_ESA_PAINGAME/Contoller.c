#include <stdio.h>
#include "includes.h"
#include "altera_up_avalon_parallel_port.h"

OS_EVENT* controllerSem;
OS_EVENT* MailBox;
OS_EVENT* MailBox2;



OS_FLAG_GRP *Flags;
OS_FLAG_GRP *Flags_Games;
OS_FLAG_GRP *Flags_Tutorial;



#define Menu_Flag 0x01
#define Game_Flag 0x02
#define C1_Flag 0x04
#define C2_Flag 0x08
#define Menu2_Flag 0x10
#define Singleplayer_Flag 0x20


#define Zwart  0x000000
#define Wit  0xffffff
#define geel 0xffff00
#define rood 0xf800
#define blauw 0x0000ff

int counter1, counter2, counter3 = 0;

typedef struct{
	int Hoogte;
}Balk;

alt_up_parallel_port_dev *gpio_dev; //	gpio device


void controllers(void* pdata) {
	INT8U err;

	int ID = (int*) pdata;
	int hoogte = 50;

	Balk balkje;

	while (1) {
		OSFlagPend(Flags, C1_Flag + C2_Flag, OS_FLAG_WAIT_CLR_ANY, 0, &err);
		balkje.Hoogte = hoogte;

		if (controller(ID) == 1) {
			hoogte = moveDown(ID, hoogte);
		} else if (controller(ID) == 0) {
			hoogte = moveUp(ID, hoogte);
		}

		if(ID == 1){
			OSFlagPend(Flags, C1_Flag, OS_FLAG_WAIT_CLR_ANY, 0, &err);
			OSMboxPost(MailBox, (void*)&balkje);
		}else if(ID == 2){
			OSFlagPend(Flags, C2_Flag, OS_FLAG_WAIT_CLR_ANY, 0, &err);
			OSMboxPost(MailBox2, (void*)&balkje);
		}

		OSTimeDly(1);
	}

}

int controller(int ID) {
	alt_up_parallel_port_dev * KEY_ptr = alt_up_parallel_port_open_dev(
			"/dev/Pushbuttons");
	alt_up_parallel_port_dev * SW_switch_ptr = alt_up_parallel_port_open_dev(
			"/dev/Slider_Switches");
	gpio_dev = alt_up_parallel_port_open_dev("/dev/Expansion_JP5");		//	DE2-115 gpio
	alt_up_parallel_port_set_port_direction(gpio_dev, 0x00000001);		// 1-0-1-1	(1 = output; 0 = input)

	int KEY_value, SW_value, gpio_values;
	INT8U err;

	SW_value = alt_up_parallel_port_read_data(SW_switch_ptr);
	KEY_value = alt_up_parallel_port_read_data(KEY_ptr); // read the pushbutton KEY values
	gpio_values = alt_up_parallel_port_read_data(gpio_dev);

	//printf("gpio_value is: %d\n", gpio_values);

	if (ID == 2) {
		if (KEY_value != 0) {
			if (KEY_value == 8) {
				return 1;
			}
			if (KEY_value == 4) {
				return 0;
			}
			if (KEY_value == 2) {
				return 2;
			}
		} else {
			return 3;
		}

	} else if (ID == 1) {
		if (SW_value != 0) {
			if (SW_value == 131072) {
				return 0;
			}else if(SW_value == 4){
				return 4;
			}
		} else {
			return 1;
		}
	} else if(ID == 3){
		if(gpio_values != 0){
			if(gpio_values == -1){
				return 1;
			} else if(gpio_values == -2){
				return 2;
			} else if(gpio_values == -3){
				return 3;
			}
		}else{
			return 0;
		}
	}
	gpio_values &= 0x80000000;		//	negeer alle andere bits, die zijn waarschijnlijk HOOG !

}

int checkID(int ID) {
	if (ID == 1) {
		return 15;
	} else if (ID == 2) {
		return 300;

	}
	return 0;
}

int moveUp(int ID, int current) {
	int boven = current;

	int X = checkID(ID);

	if (boven >= 4 && boven <= 184) {
		VGA_box(X, boven, X + 5, boven + 1, blauw);
		VGA_box(X, boven + 50, X + 5, boven + 51, 0);
		boven--;
	} else if (boven > 184) {
		VGA_box(X, boven, X + 5, boven + 1, blauw);
		boven--;
	}
	return boven;

}
int moveDown(int ID, int current) {
	int boven = current;

	int X = checkID(ID);

	if (boven <= 184 && boven >= 4) {
		VGA_box(X, boven + 50, X + 5, boven + 51, blauw);
		VGA_box(X, boven, X + 5, boven + 1, 0);
		boven++;
	} else if (boven < 4) {
		VGA_box(X, boven + 50, X + 5, boven + 51, blauw);
		boven++;
	}
	return boven;

}
