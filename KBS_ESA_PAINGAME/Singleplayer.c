void Singleplayer(void* pdata){
	
	ALT_x1 = 0;
	ALT_x2 = 165;
	ALT_y = 100;
	ALT_inc_x = 1;
	ALT_inc_y = 1;

	pixel_buffer_x = 315;
	pixel_buffer_y = 230;

	INT8U err;
	int i;
	int hoogte;
	int ID;
	int count;
	Balk * balkje;
	Balk * balkje2;
	
	int score3 = 0;
	int leven = 3;
	
	VGA_box (316, 0, 319, 239, groen); 					// rechts
	
	while (1) {
		OSFlagPend(Flags, Game_Flag + C1_Flag + C2_Flag, OS_FLAG_WAIT_CLR_ANY, 0, &err);

		balkje = (Balk*) OSMboxPend(MailBox, 0, &err);
		balkje2 = (Balk*) OSMboxPend(MailBox2, 0, &err);

		//printf("ID is: %d\tHoogte is: %d\n", balkje->ID, balkje->Hoogte);

		for (i = 0; i <= 999; i++) {
			if (score3 == i) {
				draw_number(i, 1);
			}
			if(score3 == 999) || (leven == 0){ // max score of geen levens meer, spel eindigt.
				score3 =0;
				endGame();
			}
		}
		
		if (check == 1) {
			del_middenlijn();
			VGA_text(20, 20, "Houdt de knop ingedrukt om te spelen");
			OSTimeDlyHMSM(0, 1, 0, 0);
			VGA_text(20, 20, "                                    ");

			del_number(1);
			del_number(2);

			score3 = 0;

			i = 0;

			check = 0;
		}

		if (controller(3) == 1) {

			VGA_box(ALT_x1, ALT_y, ALT_x1 + 5, ALT_y + 5, zwart); // erase
			if (first == 1) {
				ALT_x1 = 160 + ALT_x1 + ALT_inc_x;
				ALT_x2 += ALT_inc_x;
				ALT_y += ALT_inc_y;
				first = 0;
			} else {
				ALT_x1 += ALT_inc_x;
				ALT_x2 += ALT_inc_x;
				ALT_y += ALT_inc_y;
			}
			VGA_box(ALT_x1, ALT_y, ALT_x1 + 5, ALT_y + 5, wit); // ball

			//collision rand boven en onder
			if ((ALT_y == pixel_buffer_y) || (ALT_y == 4)) {
				ALT_inc_y = -(ALT_inc_y);
			}
			//collision rand rechts en links
			if ((ALT_x2 == pixel_buffer_x)) {
				ALT_inc_x = -(ALT_inc_x);
			}

			//links dood
			if (ALT_x1 == 0) {
				leven--;
				gescoord(3);
			}

			//kaats tegen muur, score erbij
			if (ALT_x2 == pixel_buffer_x) {
				score3++;
			}

			//collision linker balkje
			//lange zijde
			if(ALT_x1 == 21 && (ALT_y > balkje->Hoogte && ALT_y < balkje->Hoogte + 50)){
				ALT_inc_x = -(ALT_inc_x);
			}
			//boven
			if((ALT_x1 < 24 && ALT_x1 > 21) && ALT_y == balkje->Hoogte){
				ALT_inc_y = -(ALT_inc_y);
			}
			//onder
			if((ALT_x1 < 24 && ALT_x1 > 21) && ALT_y == balkje->Hoogte + 50){
				ALT_inc_y = -(ALT_inc_y);
			}



			OSTimeDly(1);

		} else if (controller(2) != 2) {
			endGame();
		}

	}
	
}

void Tutorial(void* pdata){
	
	ALT_x1 = 0;
	ALT_x2 = 165;
	ALT_y = 100;
	ALT_inc_x = 1;
	ALT_inc_y = 1;

	pixel_buffer_x = 315;
	pixel_buffer_y = 230;

	INT8U err;
	int i;
	int hoogte;
	int ID;
	int count;
	Balk * balkje;
	Balk * balkje2;
	
	VGA_box (316, 0, 319, 239, groen); 					// rechts
	
	while (1) {
		OSFlagPend(Flags, Game_Flag + C1_Flag + C2_Flag, OS_FLAG_WAIT_CLR_ANY, 0, &err);

		balkje = (Balk*) OSMboxPend(MailBox, 0, &err);
		balkje2 = (Balk*) OSMboxPend(MailBox2, 0, &err);

		//printf("ID is: %d\tHoogte is: %d\n", balkje->ID, balkje->Hoogte);

		
		if (check == 1) {
			del_middenlijn();
			VGA_text(20, 20, "Druk op de in om de tutorial de beëindigen");
			OSTimeDlyHMSM(0, 5, 0, 0);
			VGA_text(20, 20, "                                    ");

			del_number(1);
			del_number(2);

			score3 = 0;

			i = 0;

			check = 0;
		}

		if (controller(3) == 1) {
			endgame();
		}
			VGA_box(ALT_x1, ALT_y, ALT_x1 + 5, ALT_y + 5, zwart); // erase
			if (first == 1) {
				ALT_x1 = 160 + ALT_x1 + ALT_inc_x;
				ALT_x2 += ALT_inc_x;
				ALT_y += ALT_inc_y;
				first = 0;
			} else {
				ALT_x1 += ALT_inc_x;
				ALT_x2 += ALT_inc_x;
				ALT_y += ALT_inc_y;
			}
			VGA_box(ALT_x1, ALT_y, ALT_x1 + 5, ALT_y + 5, wit); // ball

			//collision rand boven en onder
			if ((ALT_y == pixel_buffer_y) || (ALT_y == 4)) {
				ALT_inc_y = -(ALT_inc_y);
			}
			//collision rand rechts en links
			if ((ALT_x2 == pixel_buffer_x)) {
				ALT_inc_x = -(ALT_inc_x);
			}

			//links dood
			if (ALT_x1 == 0) {
				gescoord(3);
			}

			//kaats tegen muur, score erbij
			if (ALT_x2 == pixel_buffer_x) {
				score3++;
			}

			//collision linker balkje
			//lange zijde
			if(ALT_x1 == 21 && (ALT_y > balkje->Hoogte && ALT_y < balkje->Hoogte + 50)){
				ALT_inc_x = -(ALT_inc_x);
			}
			//boven
			if((ALT_x1 < 24 && ALT_x1 > 21) && ALT_y == balkje->Hoogte){
				ALT_inc_y = -(ALT_inc_y);
			}
			//onder
			if((ALT_x1 < 24 && ALT_x1 > 21) && ALT_y == balkje->Hoogte + 50){
				ALT_inc_y = -(ALT_inc_y);
			}



			OSTimeDly(1);

		 

	}
	

	
}