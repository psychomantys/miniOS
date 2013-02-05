/*
 * =====================================================================================
 *
 *       Filename:  monitor.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/13/2012 08:51:35 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#include	<kernel/monitor.hpp>
#include	<isa_specific_code.hpp>

#include	<stdint.h>
#include	<stdarg.h>
#include	<kernel/kpp/snprintf.hpp>

#ifndef USE_MONITOR_IN_C

uint16_t *video_memory = (uint16_t *)0xB8000;
uint8_t cursor_x=0,cursor_y = 0;

// Updates the hardware cursor.
static void move_cursor(){
	// The screen is 80 characters wide...
	uint16_t cursorLocation = cursor_y * 80 + cursor_x;
	outb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
	outb(0x3D5, cursorLocation >> 8); // Send the high cursor byte.
	outb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
	outb(0x3D5, cursorLocation);      // Send the low cursor byte.
}


// Scrolls the text on the screen up by one line.
static void scroll(){
	// Get a space character with the default colour attributes.
	uint8_t attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
	uint16_t blank = 0x20 /* space */ | (attributeByte << 8);

	// Row 25 is the end, this means we need to scroll up
	if(cursor_y >= 25){
		// Move the current text chunk that makes up the screen
		// back in the buffer by a line
		int i;
		for (i = 0*80; i < 24*80; i++){
			video_memory[i] = video_memory[i+80];
		}

		// The last line should now be blank. Do this by writing
		// 80 spaces to it.
		for(i = 24*80; i < 25*80; i++){
			video_memory[i] = blank;
		}
		// The cursor should now be on the last line.
		cursor_y = 24;
	}
}

// Writes a single character out to the screen.
void monitor_put(const char c){
	// The background colour is black (0), the foreground is white (15).
	uint8_t backColour = 0;
	uint8_t foreColour = 15;

	// The attribute byte is made up of two nibbles - the lower being the
	// foreground colour, and the upper the background colour.
	uint8_t  attributeByte = (backColour << 4) | (foreColour & 0x0F);
	// The attribute byte is the top 8 bits of the word we have to send to the
	// VGA board.
	uint16_t attribute = attributeByte << 8;
	uint16_t *location;

	// Handle a backspace, by moving the cursor back one space
	if (c == 0x08 && cursor_x){
		cursor_x--;
	}
	// Handle a tab by increasing the cursor's X, but only to a point
	// where it is divisible by 8.
	else if (c == 0x09){
		cursor_x = (cursor_x+8) & ~(8-1);
	}

	// Handle carriage return
	else if (c == '\r'){
		cursor_x = 0;
	}
	// Handle newline by moving cursor back to left and increasing the row
	else if (c == '\n'){
		cursor_x = 0;
		cursor_y++;
	}
	// Handle any other printable character.
	else if(c >= ' '){
		location = video_memory + (cursor_y*80 + cursor_x);
		*location = c | attribute;
		cursor_x++;
	}
	// Check if we need to insert a new line because we have reached the end
	// of the screen.
	if (cursor_x >= 80){
		cursor_x = 0;
		cursor_y ++;
	}

	// Scroll the screen if needed.
	scroll();
	// Move the hardware cursor.
	move_cursor();
}

// Clears the screen, by copying lots of spaces to the framebuffer.
void monitor_clear(){
	// Make an attribute byte for the default colours
	uint8_t attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
	uint16_t blank = 0x20 /* space */ | (attributeByte << 8);

	int i;
	for(i = 0; i < 80*25; i++){
		video_memory[i] = blank;
	}
	// Move the hardware cursor back to the start.
	cursor_x = 0;
	cursor_y = 0;
	move_cursor();
}

// Outputs a null-terminated ASCII string to the monitor.
void monitor_write(const char *c){
	int i = 0;
	while( c[i] ){
		monitor_put(c[i++]);
	}
}

#endif

int kprintf( const char *format, ... ){
	static VGA tela;
	int rv=0;
	va_list ap;
	char kprintf_buff [KPRINTF_BUFFER_MAX_SIZE];

	va_start(ap, format);
	rv = vsnprintf(kprintf_buff, KPRINTF_BUFFER_MAX_SIZE, format, ap);
//	Deveria funcionar isso:
	va_end(ap);

//	tela.write("print:\n");
//	tela.write(format);
	tela.write(kprintf_buff);
//	tela.write("/print\n");
//	tela.write(format);
	return rv;
}

VGA::VGA() :
	video_memory( (uint16_t *)(0xB8000) ),
	cursor_x(0),
	cursor_y(0),
	attribute(0x0F)
{
	clear();
	settextcolor(VGA::light_green, VGA::black);
}

/* Sets the forecolor and backcolor that we will use */
void VGA::settextcolor( const uint8_t &forecolor, const uint8_t &backcolor){
//void VGA::settextcolor( VGA::Color forecolor, VGA::Color backcolor){
	/* Top 4 bytes are the background, bottom 4 bytes
	*  are the foreground color */
	this->attribute = (backcolor << 4) | (forecolor & 0x0F);
}

// Updates the hardware cursor.
void VGA::move_cursor(){
	// The screen is 80 characters wide...
	uint16_t cursorLocation = this->cursor_y * 80 + this->cursor_x;
	outb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
	outb(0x3D5, cursorLocation >> 8); // Send the high cursor byte.
	outb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
	outb(0x3D5, cursorLocation);      // Send the low cursor byte.
}


// Scrolls the text on the screen up by one line.
void VGA::scroll(){
	// Get a space character with the default colour attributes.
	uint16_t blank = 0x20 /* space */ | ( this->attribute << 8 );

	// Row 25 is the end, this means we need to scroll up
	if(this->cursor_y >= 25){
		// Move the current text chunk that makes up the screen
		// back in the buffer by a line
		int i;
		for (i = 0*80; i < 24*80; i++){
			video_memory[i] = video_memory[i+80];
		}

		// The last line should now be blank. Do this by writing
		// 80 spaces to it.
		for(i = 24*80; i < 25*80; i++){
			video_memory[i] = blank;
		}
		// The cursor should now be on the last line.
		this->cursor_y = 24;
	}
}

// Writes a single character out to the screen.
void VGA::put(const char &c){
	// The attribute byte is the top 8 bits of the word we have to send to the
	// VGA board.
	uint16_t *location;

	// Handle a backspace, by moving the cursor back one space
	if (c == 0x08 && this->cursor_x){
		cursor_x--;
	}
	// Handle a tab by increasing the cursor's X, but only to a point
	// where it is divisible by 8.
	else if (c == 0x09){
		cursor_x = (this->cursor_x+8) & ~(8-1);
	}

	// Handle carriage return
	else if (c == '\r'){
		this->cursor_x = 0;
	}
	// Handle newline by moving cursor back to left and increasing the row
	else if (c == '\n'){
		this->cursor_x = 0;
		this->cursor_y++;
	}
	// Handle any other printable character.
	else if(c >= ' '){
		location = this->video_memory + (this->cursor_y*80 + this->cursor_x);
		*location = c | ( this->attribute << 8 );
		this->cursor_x++;
	}
	// Check if we need to insert a new line because we have reached the end
	// of the screen.
	if (this->cursor_x >= 80){
		this->cursor_x = 0;
		this->cursor_y ++;
	}

	// Scroll the screen if needed.
	this->scroll();
	// Move the hardware cursor.
	this->move_cursor();
}

// Clears the screen, by copying lots of spaces to the framebuffer.
void VGA::clear(){
	uint16_t blank = 0x20 /* space */ | (this->attribute << 8);

	int i;
	for(i = 0; i < 80*25; i++){
		this->video_memory[i] = blank;
	}
	// Move the hardware cursor back to the start.
	this->cursor_x = 0;
	this->cursor_y = 0;
	this->move_cursor();
}

// Outputs a null-terminated ASCII string to the monitor.
void VGA::write(const char *c){
	int i = 0;
	while( c[i] ){
		this->put(c[i++]);
	}
}

