#include <stdio.h>
#include "includes.h"
#include "altera_up_avalon_character_lcd.h"
#include "altera_up_avalon_parallel_port.h"
#include "string.h"
#include <os/alt_sem.h>

/* Definition of Task Stacks */
#define   TASK_STACKSIZE       2048
OS_STK task1_stk[TASK_STACKSIZE];
OS_STK task2_stk[TASK_STACKSIZE];
OS_STK task3_stk[TASK_STACKSIZE];
OS_STK task4_stk[TASK_STACKSIZE];

/* Definition of Task Priorities */

#define TASK1_PRIORITY      10
#define TASK2_PRIORITY      11
#define TASK3_PRIORITY      12
#define TASK4_PRIORITY      9

ALT_SEM(sem)

alt_up_character_lcd_dev *lcd_dev;
alt_up_parallel_port_dev *red_LEDs_dev;
alt_up_parallel_port_dev *green_LEDs_dev;
alt_up_parallel_port_dev *slider_switches_dev;
alt_up_parallel_port_dev *hex3_hex0_dev, *hex7_hex4_dev;

/* create a message to be displayed on the VGA and LCD displays */
char text_top_row[40] = "HOI";
char text_bottom_row[40];


void task1(void* pdata) {
	int count = 0;

	while (1) {
		ALT_SEM_PEND(sem, 0);

		sprintf(text_bottom_row, "%d", count++);

		alt_up_character_lcd_set_cursor_pos(lcd_dev, 0, 1); // set LCD cursor location to bottom row
		alt_up_character_lcd_string(lcd_dev, text_bottom_row);

		OSTimeDlyHMSM(0, 0, 0, 500);
		ALT_SEM_POST(sem);

		OSTimeDlyHMSM(0, 0, 1, 0);
	}
}


void task2(void* pdata)
{
	int toggle = 0;
	while (1)
	{
		ALT_SEM_PEND(sem, 0);

		if (toggle == 0)
		{
			strcpy(text_bottom_row, "Task2");
			toggle = 1;
		}
		else
		{
			strcpy(text_bottom_row, "     ");
			toggle = 0;
		}

		alt_up_character_lcd_set_cursor_pos(lcd_dev, 5, 1); // set LCD cursor location to bottom row
		alt_up_character_lcd_string(lcd_dev, text_bottom_row);

		OSTimeDlyHMSM(0, 0, 0, 500);
		ALT_SEM_POST(sem);
		OSTimeDlyHMSM(0, 0, 1, 0);
	}
}

void task3(void* pdata) {
	int sw_values, hex_values;

	while (1) {
		sw_values = alt_up_parallel_port_read_data(slider_switches_dev);

		sw_values &= 0x000000FF;
		hex_values = sw_values | sw_values << 8 | sw_values << 16 | sw_values << 24;

		alt_up_parallel_port_write_data(hex3_hex0_dev, hex_values);

		OSTimeDlyHMSM(0, 0, 0, 100);
	}
}

void task4(void* pdata) {
	unsigned int countR = 1;
	unsigned int countG = 1 << 8;
	int toggle = 1;

	while (1) {
		alt_up_parallel_port_write_data(red_LEDs_dev, countR);
		alt_up_parallel_port_write_data(green_LEDs_dev, countG);

		OSTimeDlyHMSM(0, 0, 0, 100);

		alt_up_parallel_port_write_data(red_LEDs_dev, 0x000000);
		alt_up_parallel_port_write_data(green_LEDs_dev, 0x000000);


		if (countR < 1 << 17)
			countR = countR << 1;
		else
			countR = 1;

		if (toggle)
		{
			if (countG == 1)
				countG = countG << 8;
			else
				countG = countG >> 1;
		}
		toggle = !toggle;
	}
}

/* The main function creates two task and starts multi-tasking */
int main(void) {
	OSInit();

	int err = ALT_SEM_CREATE(&sem, 1);
	if (err != 0)
		printf("Semaphore NOT created\n");

	/* output text message to the LCD */
	ALT_SEM_PEND(sem, 0);

	lcd_dev = alt_up_character_lcd_open_dev("/dev/Char_LCD_16x2");
	if (lcd_dev == NULL) {
		printf("Error: could not open character LCD device\n");
		return -1;
	} else
		printf("Opened character LCD device\n");

	alt_up_character_lcd_set_cursor_pos(lcd_dev, 0, 0); // set LCD cursor location to top row
	alt_up_character_lcd_string(lcd_dev, text_top_row);
	alt_up_character_lcd_set_cursor_pos(lcd_dev, 0, 1); // set LCD cursor location to bottom row
	alt_up_character_lcd_string(lcd_dev, text_bottom_row);
	alt_up_character_lcd_cursor_off(lcd_dev); // turn off the LCD cursor
	ALT_SEM_POST(sem);

	VGA_text (35, 29, "                                     ");
	//VGA_text (35, 30, text_bottom_row);
	VGA_box (0, 0, 319, 239, 0);						// clear the screen
	VGA_box (15, 100, 20, 150, 0x3333CC);				// links
	VGA_box (300, 100, 305, 150, 0x3333CC);				// rechts

	VGA_box (0, 0, 319, 3, 0xFFFFFF);					// boven
	VGA_box (0, 236, 319, 239, 0xFFFFFF);				// onder

	VGA_box (159, 0, 160, 239, 0xFFFFFF);				// middenlijntje

	VGA_box (100, 50, 105, 55, 0xFFFFFF);

	red_LEDs_dev = alt_up_parallel_port_open_dev("/dev/Red_LEDs");
	green_LEDs_dev = alt_up_parallel_port_open_dev("/dev/Green_LEDs");
	slider_switches_dev = alt_up_parallel_port_open_dev("/dev/Slider_Switches");

	hex3_hex0_dev = alt_up_parallel_port_open_dev("/dev/HEX3_HEX0");
	hex7_hex4_dev = alt_up_parallel_port_open_dev("/dev/HEX7_HEX4");

	OSTaskCreateExt(task1, NULL, (void *) &task1_stk[TASK_STACKSIZE - 1],
			TASK1_PRIORITY, TASK1_PRIORITY, task1_stk, TASK_STACKSIZE, NULL, 0);

	OSTaskCreateExt(task2, NULL, (void *) &task2_stk[TASK_STACKSIZE - 1],
			TASK2_PRIORITY, TASK2_PRIORITY, task2_stk, TASK_STACKSIZE, NULL, 0);

	OSTaskCreateExt(task3, NULL, (void *) &task3_stk[TASK_STACKSIZE - 1],
			TASK3_PRIORITY, TASK3_PRIORITY, task3_stk, TASK_STACKSIZE, NULL, 0);

	OSTaskCreateExt(task4, NULL, (void *) &task4_stk[TASK_STACKSIZE - 1],
			TASK4_PRIORITY, TASK4_PRIORITY, task4_stk, TASK_STACKSIZE, NULL, 0);

	OSStart();
	return 0;
}

/****************************************************************************************
 * Subroutine to send a string of text to the VGA monitor
****************************************************************************************/
void VGA_text(int x, int y, char * text_ptr)
{
	int offset;
  	volatile char * character_buffer = (char *) 0x09000000;	// VGA character buffer

	/* assume that the text string fits on one line */
	offset = (y << 7) + x;
	while ( *(text_ptr) )
	{
		*(character_buffer + offset) = *(text_ptr);	// write to the character buffer
		++text_ptr;
		++offset;
	}
}

/****************************************************************************************
 * Draw a filled rectangle on the VGA monitor
****************************************************************************************/
void VGA_box(int x1, int y1, int x2, int y2, short pixel_color)
{
	int offset, row, col;
  	volatile short * pixel_buffer = (short *) 0x08000000;	// VGA pixel buffer

	/* assume that the box coordinates are valid */
	for (row = y1; row <= y2; row++)
	{
		col = x1;
		while (col <= x2)
		{
			offset = (row << 9) + col;
			*(pixel_buffer + offset) = pixel_color;	// compute halfword address, set pixel
			++col;
		}
	}
}

