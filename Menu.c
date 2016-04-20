int main(void){
	
	OSInit();
	
	VGA_text(200, 250, "Tutorial");
	VGA_text(200, 300, "Singleplayer");
	VGA_text(200, 350, "Multiplayer");
	VGA_text(200, 400, "Singleplayer highscores");
}

void VGA_text(int x, int y, char * text_ptr){
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