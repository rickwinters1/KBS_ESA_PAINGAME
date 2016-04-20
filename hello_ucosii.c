#include <stdio.h>
#include "includes.h"

OS_EVENT* controllerSem;

/* Definition of Task Stacks */
#define   TASK_STACKSIZE       2048
OS_STK    task1_stk[TASK_STACKSIZE];
OS_STK    task2_stk[TASK_STACKSIZE];

/* Definition of Task Priorities */

#define TASK1_PRIORITY      1
#define TASK2_PRIORITY      2

void task1(void* pdata);

/* The main function creates two task and starts multi-tasking */
int main(void)
{
	controllerSem = OSSemCreate(1);
	VGA_box (15, 100, 20, 150, 0x3333CC);				// links
	VGA_box (300, 100, 305, 150, 0x3333CC);				// rechts
	VGA_box (0, 0, 319, 239, 0);						//clear screen
	VGA_box (0, 0, 319, 3, 0xFFFFFF);					// boven
	VGA_box (0, 236, 319, 239, 0xFFFFFF);				// onder

	VGA_box (159, 0, 160, 239, 0xFFFFFF);				// middenlijntje

	OSTaskCreate(task1,(void*) 1,&task1_stk[TASK_STACKSIZE-1],TASK1_PRIORITY);

               
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


