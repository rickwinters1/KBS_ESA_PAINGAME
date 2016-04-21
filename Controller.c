#include <stdio.h>
#include "includes.h"
#include "altera_up_avalon_parallel_port.h"

OS_EVENT* controllerSem;

int hoogte = 50;


void controllers(void* pdata){
	int ID = (int*)pdata;
	printf("controller: %d\n", ID);
	while (1){
		if (controller(ID) == 1){
			moveDown(ID, hoogte);

		}else if(controller(ID) == 0){
			moveUp(ID, hoogte);
		}

		OSTimeDly(1);
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
		while(KEY_value == 2){
			return 2;
		}
	}else{
		return 3;
	}

}

int checkID(int ID){
	if(ID == 1){
		return 15;
	}else if(ID == 2){
		return 300;

	}
	return 0;
}

void moveUp(int ID, int current){
	int boven = current;

	int X = checkID(ID);

	if(boven >= 4 && boven <= 184){
		VGA_box(X, boven, X + 5, boven + 1, 0x3333CC);
		VGA_box(X, boven +50, X + 5, boven + 51, 0);
		hoogte--;
	}else if(boven > 184){
		VGA_box(X, boven, X + 5, boven + 1, 0x3333CC);
		hoogte--;
	}

}
void moveDown(int ID, int current){
	int boven = current;

	int X = checkID(ID);

	if(boven <= 184 && boven >= 4){
		VGA_box(X, boven + 50, X + 5, boven + 51, 0x3333CC);
		VGA_box(X, boven, X + 5, boven + 1, 0);
		hoogte++;
	}else if(boven < 4){
		VGA_box(X, boven + 50, X+5, boven + 51, 0x3333CC);
		hoogte++;
	}


}
