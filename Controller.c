#include <stdio.h>
#include "includes.h"
#include "altera_up_avalon_parallel_port.h"

OS_EVENT* controllerSem;

int hoogte = 50;

void task1(void* pdata){
	int ID = (int*)pdata;

	while (1){
		if (controller(ID) == 1){
			moveDown(ID, hoogte);

		}else if(controller(ID) == 0){
			moveUp(ID, hoogte);

		}

		OSTimeDlyHMSM(0,0,0,0);
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


void moveUp(int ID, int current){
	int boven = current;

	if(ID == 1){
		if(boven >= 4){
			VGA_box(15, boven, 20, boven + 1, 0x3333CC);
			VGA_box(15, boven +50, 20, boven + 51, 0);
			hoogte--;
		}else if(boven >= 184){
			VGA_box(15, boven, 20, boven + 1, 0x3333CC);
		}
	}else if(ID == 2){
		if(boven >= 4){
			VGA_box(300, boven, 305, boven + 1, 0x3333CC);
			VGA_box(300, boven +50, 305, boven + 51, 0);
			hoogte--;
		}else if(boven >= 184){
			VGA_box(15, boven, 20, boven + 1, 0x3333CC);
		}
	}

}
void moveDown(int ID, int current){
	int boven = current;

	if(ID == 1){
		if(boven <= 184){
			VGA_box(15, boven + 50, 20, boven + 51, 0x3333CC);
			VGA_box(15, boven, 20, boven + 1, 0);
			hoogte++;
		}
	}else if(ID == 2){
		if(boven <= 184){
			VGA_box(300, boven + 50, 305, boven + 51, 0x3333CC);
			VGA_box(300, boven, 305, boven + 1, 0);
			hoogte++;
		}
	}

}
