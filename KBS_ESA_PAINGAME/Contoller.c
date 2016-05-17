#include <stdio.h>
#include "includes.h"
#include "altera_up_avalon_parallel_port.h"

OS_EVENT* controllerSem;

OS_FLAG_GRP *Flags;

#define Menu_Flag 0x01
#define Game_Flag 0x02
#define C1_Flag 0x04
#define C2_Flag 0x08

short kleur = 0x0000ff;
int counter1, counter2, counter3 = 0;

void controllers(void* pdata) {
	INT8U err;

	int ID = (int*) pdata;
	int hoogte = 50;

	while (1) {
		OSFlagPend(Flags, C1_Flag + C2_Flag, OS_FLAG_WAIT_CLR_ANY, 0, &err);

		if (controller(ID) == 1) {
			hoogte = moveDown(ID, hoogte);
		} else if (controller(ID) == 0) {
			hoogte = moveUp(ID, hoogte);
		}

		OSTimeDly(1);
	}

}

int controller(int ID) {
	alt_up_parallel_port_dev * KEY_ptr = alt_up_parallel_port_open_dev(
			"/dev/Pushbuttons");
	alt_up_parallel_port_dev * SW_switch_ptr = alt_up_parallel_port_open_dev(
			"/dev/Slider_Switches");

	int KEY_value, SW_value;
	INT8U err;

	SW_value = alt_up_parallel_port_read_data(SW_switch_ptr);
	KEY_value = alt_up_parallel_port_read_data(KEY_ptr); // read the pushbutton KEY values

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
			if (SW_value == 1) {
				return 0;
			}
		} else {
			return 1;
		}
	}

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
		VGA_box(X, boven, X + 5, boven + 1, kleur);
		VGA_box(X, boven + 50, X + 5, boven + 51, 0);
		boven--;
	} else if (boven > 184) {
		VGA_box(X, boven, X + 5, boven + 1, kleur);
		boven--;
	}
	return boven;

}
int moveDown(int ID, int current) {
	int boven = current;

	int X = checkID(ID);

	if (boven <= 184 && boven >= 4) {
		VGA_box(X, boven + 50, X + 5, boven + 51, kleur);
		VGA_box(X, boven, X + 5, boven + 1, 0);
		boven++;
	} else if (boven < 4) {
		VGA_box(X, boven + 50, X + 5, boven + 51, kleur);
		boven++;
	}
	return boven;

}
