#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Altera_UP_SD_Card_Avalon_Interface.h"

#define MAX_SUBDIRECTORIES 20

char highscores[5][20] = {
			{""},
			{""},
			{""},
			{""},
			{""},
};


/********************************************************************************
 * This program demonstrates use of the SD Card reader
 *
 * It:
 *  -- Detects if a properly-formatted SD Card is present
 * 	-- Recursively searches the directory tree for files
 * 	-- Prints out the file listing to the terminal window
********************************************************************************/

void read_file()
{
	short int Read;
	Read = alt_up_sd_card_fopen("hoi.txt", false);

	int i = 0;

	char buffer[1] = "";
	short int c = 1;

	c = alt_up_sd_card_read(Read);

	while (c != -1){
		if(c == '|'){		//check voor komma
			i++;
		}else{
			sprintf(buffer, "%c", c);
			strcat(highscores[i], buffer);
		}
		c = alt_up_sd_card_read(Read);		//lees volgende char
	}

	int j;
	for(j = 0; j < 5; j++){
		printf("Number is : %d data :%s\n",j , highscores[j]);
	}

	alt_up_sd_card_fclose(Read);						//sluit het bestand
}

void write_file(char input[]){
	short int Write;
	Write = alt_up_sd_card_fopen("hoi.txt", false);
	int i = 0;
	bool writing;
	if(Write < 0){
		printf("Can't open\n");
	}else{
		while(input[i] != '\0'){
			writing = alt_up_sd_card_write(Write, input[i]);
			printf("%c", input[i]);
			i++;
		}
		alt_up_sd_card_fclose(Write);
		printf("\nFile is geschreven\n");
	}

}


int main (void){

	alt_up_sd_card_dev * sd_card;
	sd_card = alt_up_sd_card_open_dev("/dev/SD_Card");

 	 if (sd_card!=NULL){
		if (alt_up_sd_card_is_Present()){
			printf("An SD Card was found!\n");

		}
		else {
			printf("No SD Card Found. \n Exiting the program.");
			return -1;
		}

 		if (alt_up_sd_card_is_FAT16()){
			printf("FAT-16 partiton found!\n");
		}
		else{
			printf("No FAT-16 partition found - Exiting!\n");
			return -1;
		}


 		char input[25];
 		strcpy(input, "Hoi 005|DOEI 004|GAY 003");

 		//write_file(input);

 		read_file();
	}

	return 0;
}
