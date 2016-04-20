#include <stdio.h>
#include "includes.h"
#include "altera_up_avalon_parallel_port.h"



void task1(void* pdata){
	alt_up_parallel_port_dev * KEY_ptr			= alt_up_parallel_port_open_dev ("/dev/Pushbuttons");
	alt_up_parallel_port_dev * HEX3_HEX0_ptr	= alt_up_parallel_port_open_dev ("/dev/HEX3_HEX0");

	int KEY_value;
	int HEX_bits = 0x0000000F;
	int ID = (int*)pdata;
	int hoogte = 0;
	while (1){
		KEY_value = alt_up_parallel_port_read_data(KEY_ptr); 		// read the pushbutton KEY values
		if (KEY_value == 8){
			printf("de waarde van de keys is: %d\n", KEY_value);
			moveDown(ID, hoogte);
			hoogte++;
		}else if(KEY_value == 4){
			printf("de waarde van de keys is: %d\n", KEY_value);
			moveUp(ID, hoogte);
			hoogte--;
		}
		OSTimeDlyHMSM(0,0,0,60);
	}


}


void moveUp(int ID, int current){
	int boven = current;

	if(ID == 1){
		VGA_box(15, boven, 20, boven + 1, 0x3333CC);
		VGA_box(15, boven +50, 20, boven + 51, 0);
	}else if(ID == 2){
		VGA_box(300, boven, 305, boven + 1, 0x3333CC);
		VGA_box(300, boven +50, 305, boven + 51, 0);
	}
}
void moveDown(int ID, int current){
	int boven = current;

	if(ID == 1){
		VGA_box(15, boven + 50, 20, boven + 51, 0x3333CC);
		VGA_box(15, boven, 20, boven + 1, 0);
	}else if(ID == 2){
		VGA_box(300, boven + 50, 305, boven + 51, 0x3333CC);
		VGA_box(300, boven, 305, boven + 1, 0);
	}
}
