#include <stdio.h>
#include "includes.h"

void task1(void* pdata)
{
	int ID = (int*)pdata;
	int hoogte = 0;
  while (1)
  {
	  if(ID == 1){
		  VGA_box (300, hoogte, 305, hoogte +50, 0x3333CC);
	  }else{
		  VGA_box (15, hoogte, 20, hoogte+50, 0x3333CC);
	  }

	  if(hoogte < 239){
		  hoogte++;
	  } else{
		  hoogte--;
	  }

  }
}
