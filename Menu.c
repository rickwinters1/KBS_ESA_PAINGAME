#include <stdio.h>
#include "includes.h"
#include "altera_up_avalon_parallel_port.h"

void task2(void* pdata){
	int ID = (int*)pdata;

	


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