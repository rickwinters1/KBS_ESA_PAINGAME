#include <stdio.h>
#include "includes.h"

OS_EVENT* controllerSem;

/* Definition of Task Stacks */
#define   TASK_STACKSIZE       2048
OS_STK    controller1_stk[TASK_STACKSIZE];
OS_STK    controller2_stk[TASK_STACKSIZE];
OS_STK	  menu_stk[TASK_STACKSIZE];

/* Definition of Task Priorities */

#define controller1_PRIORITY      6
#define controller2_PRIORITY      7
#define menu_PRIORITY		  		8

void controllers(void* pdata);
void menu(void* pdata);

/* The main function creates two task and starts multi-tasking */
int main(void)
{

	controllerSem = OSSemCreate(1);
	VGA_box (0, 0, 319, 239, 0);						//clear screen
	VGA_box (15, 50, 20, 100, 0x0000ff);				// links
	VGA_box (300, 50, 305, 100, 0x0000ff);				// rechts

	VGA_box (0, 0, 319, 3, 0xFFFFFF);					// boven
	VGA_box (0, 236, 319, 239, 0xFFFFFF);				// onder

//	VGA_box (159, 0, 160, 239, 0xFFFFFF);				// middenlijntje

	OSTaskCreate(controllers,(void*) 1,&controller1_stk[TASK_STACKSIZE-1],controller1_PRIORITY);
	OSTaskCreate(controllers,(void*) 2,&controller2_stk[TASK_STACKSIZE-1],controller2_PRIORITY);
	OSTaskCreate(menu, (void*) 2, &menu_stk[TASK_STACKSIZE-1],menu_PRIORITY);
               
	OSStart();
	return 0;
}

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


