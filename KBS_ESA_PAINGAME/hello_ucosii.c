#include <stdio.h>
#include "includes.h"

OS_EVENT* controllerSem;
OS_EVENT* menuSem;
OS_EVENT* gameSem;

OS_FLAG_GRP *Flags;

OS_EVENT* MailBox;
OS_EVENT* MailBox2;




extern volatile int timeout = 0;							// used to synchronize with the timer
volatile int * interval_timer_ptr = (int *) 0x10002000;	// internal timer base address

/* Definition of Task Stacks */
#define   TASK_STACKSIZE       2048
OS_STK    controller1_stk[TASK_STACKSIZE];
OS_STK    controller2_stk[TASK_STACKSIZE];
OS_STK    Game_stk[TASK_STACKSIZE];
OS_STK	  menu_stk[TASK_STACKSIZE];
OS_STK	  menu_stk2[TASK_STACKSIZE];


/* Definition of Task Priorities */

#define controller1_PRIORITY      	11
#define controller2_PRIORITY      	12
#define Game_PRIORITY      			6
#define menu_PRIORITY		  		9


//Flags
#define Menu_Flag 0x01
#define Game_Flag 0x02
#define C1_Flag 0x04
#define C2_Flag 0x08
#define Menu2_Flag 0x10

#define zwart  0x000000
#define wit  0xffffff
#define geel 0xffff00
#define rood 0xf800
#define blauw 0x0000ff
#define groen 0x0697


void controllers(void* pdata);
void menu(void* pdata);
void selecteerMenu(void *pdata);
void Game(void* pdata);

void * Messages[500];


/* The main function creates two task and starts multi-tasking */
int main(void)
{
	INT8U err;
	OSInit();
	int counter = 0x50000;				// 1/(50 MHz) x (0x960000) ~= 200 msec
	*(interval_timer_ptr + 0x2) = (counter & 0xFFFF);
	*(interval_timer_ptr + 0x3) = (counter >> 16) & 0xFFFF;

	*(interval_timer_ptr + 1) = 0x7;	// STOP = 0, START = 1, CONT = 1, ITO = 1

	Flags = OSFlagCreate(Game_Flag + C1_Flag + C2_Flag, &err);

	gameSem = OSSemCreate(0);
	controllerSem = OSSemCreate(0);

	MailBox = OSMboxCreate((void *)0);
	MailBox2 = OSMboxCreate((void *)0);


	VGA_box (0, 0, 319, 239, 0);						//clear screen
	VGA_box (15, 50, 20, 100, 0x0000ff);				// links
	VGA_box (300, 50, 305, 100, 0x0000ff);				// rechts

	VGA_box (0, 0, 319, 3, groen);					// boven
	VGA_box (0, 236, 319, 239, groen);				// onder


	OSTaskCreate(controllers,(void*) 1,&controller1_stk[TASK_STACKSIZE-1],controller1_PRIORITY);
	OSTaskCreate(controllers,(void*) 2,&controller2_stk[TASK_STACKSIZE-1],controller2_PRIORITY);
	OSTaskCreate(Game,(void*) 0, &Game_stk[TASK_STACKSIZE-1],Game_PRIORITY);

	OSTaskCreate(menu, (void*) 2, &menu_stk[TASK_STACKSIZE-1],menu_PRIORITY);
	OSTaskCreate(selecteerMenu, (void*) 3, &menu_stk2[TASK_STACKSIZE-1], menu_PRIORITY +1);
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

void draw_middenlijn(){
	int i = 0;
	while(i < 240){
		VGA_box (159, i, 160, i+ 7, groen);				// middenlijntje
		i = i + 11;
	}
}

void del_middenlijn(){
	VGA_box(159, 4, 160, 235, 0);
}

void del_number(int side){
	if(side == 1){
		VGA_box(120, 20, 140, 60, 0);
	}
	if(side == 2){
		VGA_box(180, 20, 200, 60, 0);
	}
}

void deleteNummer(){
	VGA_box(120, 20, 180, 60, 0);
}

void draw_number(int nummer, int ID){

	int X = checkIDScore(ID);

	if(nummer == 1){
		VGA_box(X + 5, 20, X+7, 60, wit);
	}else if(nummer == 2){
		VGA_box(X, 20, X+ 20, 22, wit);					//bovenste lijn
		VGA_box(X + 18, 22, X+ 20, 38, wit);			//lijn naar beneden
		VGA_box(X, 38, X+ 20, 40, wit);					//lijn naar links
		VGA_box(X, 40, X+ 2, 58, wit);					//lijn naar beneden
		VGA_box(X, 58, X+ 20, 60, wit);					//lijn naar rechts
	}else if(nummer == 3){
		VGA_box(X, 20, X+ 20, 22, wit);					//bovenste lijn
		VGA_box(X + 18, 22, X+ 20, 60, wit);			//rechter lijn
		VGA_box(X, 39, X+ 20, 41, wit);					//midden lijn
		VGA_box(X, 58, X+ 20, 60, wit);					//onderste lijn
	}else if(nummer == 4){
		VGA_box(X, 20, X+2, 40, wit);					//linkse lijn
		VGA_box(X, 39, X + 20, 41, wit);				//middelste lijn
		VGA_box(X + 18, 20, X + 20, 60, wit);			//rechtse lijn
	}else if(nummer == 5){
		VGA_box(X, 20, X+ 20, 22, wit);					//bovenste lijn
		VGA_box(X, 22, X+ 2, 38, wit);					//lijn naar beneden
		VGA_box(X, 38, X+ 20, 40, wit);					//lijn naar links
		VGA_box(X + 18, 40, X+ 20, 58, wit);			//lijn naar beneden
		VGA_box(X, 58, X+ 20, 60, wit);					//lijn naar rechts
	}else if(nummer == 6){
		VGA_box(X, 20, X+2, 60, wit);					//lijn links
		VGA_box(X, 20, X+ 20, 22, wit);					//bovenste lijn
		VGA_box(X, 58, X+ 20, 60, wit);					//onderste lijn
		VGA_box(X + 18, 40, X+ 20, 58, wit);			//lijn rechts
		VGA_box(X, 39, X + 20, 41, wit);				//middelste lijn
	}else if(nummer == 7){
		VGA_box(X + 18, 20, X + 20, 60, wit);			//rechtse lijn
		VGA_box(X, 20, X+ 20, 22, wit);					//bovenste lijn
	}else if(nummer == 8){
		VGA_box(X + 18, 20, X + 20, 60, wit);			//rechtse lijn
		VGA_box(X, 20, X+2, 60, wit);					//lijn links
		VGA_box(X, 20, X+ 20, 22, wit);					//bovenste lijn
		VGA_box(X, 39, X + 20, 41, wit);				//middelste lijn
		VGA_box(X, 58, X+ 20, 60, wit);					//onderste lijn
	}else if(nummer == 9){
		VGA_box(X + 18, 20, X + 20, 60, wit);			//rechtse lijn
		VGA_box(X, 20, X+2, 40, wit);					//linkse lijn
		VGA_box(X, 20, X+ 20, 22, wit);					//bovenste lijn
		VGA_box(X, 39, X + 20, 41, wit);				//middelste lijn
		VGA_box(X, 58, X+ 20, 60, wit);					//onderste lijn
	}else if(nummer == 0){
		VGA_box(X + 18, 20, X + 20, 60, wit);			//rechtse lijn
		VGA_box(X, 20, X+2, 60, wit);					//lijn links
		VGA_box(X, 20, X+ 20, 22, wit);					//bovenste lijn
		VGA_box(X, 58, X+ 20, 60, wit);					//onderste lijn
	}
}

int checkIDScore(int ID){
	if(ID == 1){
		return 120;
	}else if(ID == 2){
		return 150;
	}else if(ID == 3){
		return 180;
	}
	
	return 0;
}

