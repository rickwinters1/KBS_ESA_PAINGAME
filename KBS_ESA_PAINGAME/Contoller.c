#include <stdio.h>
#include "includes.h"
#include "altera_up_avalon_parallel_port.h"

OS_EVENT* controllerSem;

short kleur = 0x0000ff;
int counter1, counter2, counter3 = 0;


void controllers(void* pdata){



	int ID = (int*)pdata;
	int hoogte = 50;

	while (1){
		if (controller(ID) == 1){
			hoogte = moveDown(ID, hoogte);
		}else if(controller(ID) == 0){
			hoogte = moveUp(ID, hoogte);
		}

		OSTimeDly(1);
	}


}

int controller(int ID){
	alt_up_parallel_port_dev * KEY_ptr = alt_up_parallel_port_open_dev ("/dev/Pushbuttons");
	alt_up_parallel_port_dev * SW_switch_ptr	= alt_up_parallel_port_open_dev ("/dev/Slider_Switches");

	int KEY_value, SW_value;
	INT8U err;

	SW_value = alt_up_parallel_port_read_data(SW_switch_ptr);
	KEY_value = alt_up_parallel_port_read_data(KEY_ptr); 		// read the pushbutton KEY values

	if(ID == 2){
		if(KEY_value != 0){
			while(KEY_value == 8){
				counter1++;
				if(counter1 >= 20){
					return 1;
					counter1 = 0;
				}
			}
			while(KEY_value == 4){
				counter2++;
				if(counter2 >= 20){
					return 0;
					counter2 = 0;
				}
			}
			while(KEY_value == 2){
				counter3++;
				if(counter3 >= 20){
					return 2;
					counter3 = 0;
				}
			}
		}else{
			return 3;
		}

	}else if(ID == 1){
		if(SW_value != 0){
			while(SW_value == 2){
				return 1;
			}
			while(SW_value == 1){
				return 0;
			}
			while(SW_value >= 3){
				return 2;
			}
		}else{
			return 3;
		}
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

int moveUp(int ID, int current){
	int boven = current;

	int X = checkID(ID);

	if(boven >= 4 && boven <= 184){
		VGA_box(X, boven, X + 5, boven + 1, kleur);
		VGA_box(X, boven +50, X + 5, boven + 51, 0);
		boven--;
	}else if(boven > 184){
		VGA_box(X, boven, X + 5, boven + 1, kleur);
		boven--;
	}
	return boven;

}
int moveDown(int ID, int current){
	int boven = current;

	int X = checkID(ID);

	if(boven <= 184 && boven >= 4){
		VGA_box(X, boven + 50, X + 5, boven + 51, kleur);
		VGA_box(X, boven, X + 5, boven + 1, 0);
		boven++;
	}else if(boven < 4){
		VGA_box(X, boven + 50, X+5, boven + 51, kleur);
		boven++;
	}
	return boven;

}
