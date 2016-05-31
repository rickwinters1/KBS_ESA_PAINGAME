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

	//alt_up_sd_card_fclose(Read);						//sluit het bestand
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

void sorteer(){
	int score3 = 102;
	int i, j, k;
	int hoogste;

	char letter1, letter2, letter3;
	char letters[4] = {'A','B','C'};
	char cijfers[4];

	cijfers[0] = score3 /100;
	cijfers[1] = score3 /10 %10;
	cijfers[3] = score3 %10;

	char nieuw[20] = "";

	for(i = 0; i < 5; i++){
		hoogste = ((highscores[i][4] - '0') *100) +
				   ((highscores[i][5] - '0') *10) +
				   (highscores[i][6] - '0');
		if(hoogste < score3){

			schuifScores(i);
			printf("score = hoger dan %de score\n", i);
			for(j = 0; j < 3; j++){
				highscores[i][j] = letters[j];
			}
			for(k = 0; k < 3; k++){
				highscores[i][4 + k] = cijfers[k] + '0';
			}
			write_file(highscores);
			break;
		}

		else{
			printf("score is niet hoger\n");
		}


	}
	read_file();

}

void schuifScores(int nummer){
	int i;
	for(i = 5; i > nummer; i--){
		strcpy(highscores[i], highscores[i-1]);
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


 		char input[50];
 		strcpy(input, "HOI 125|DOE 100|GAY 075|PIT 002|KUT 001|");

 		write_file(input);

 		read_file();

 		//sorteer();
	}

	return 0;
}
