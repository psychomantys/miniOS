/*
 * =====================================================================================
 *
 *       Filename:  keyboard.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/08/2013 09:37:00 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include	<isa_specific_code.hpp>
#include	<plataform_specific_code.hpp>
#include	<kernel/monitor.hpp>
#include	<kernel/keyboard.hpp>
#include	<kernel/irq.hpp>
#include	<kernel/timer.hpp>

SQueue<char> Keyboard::keyboard_buffer(20);

/* KBDUS means US Keyboard Layout. This is a scancode table
 *  used to layout a standard US keyboard. I have left some
 *  comments in to give you an idea of what key is what, even
 *  though I set it's array index to 0. You can change that to
 *  whatever you want using a macro, if you wish! */
static unsigned char kbdus[128]= {
	0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
	'9', '0', '-', '=', '\b',	/* Backspace */
	'\t',			/* Tab */
	'q', 'w', 'e', 'r',	/* 19 */
	't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
	0,			/* 29   - Control */
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
	'\'', '`',   0,		/* Left shift */
	'\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
	'm', ',', '.', '/',   0,				/* Right shift */
	'*',
	0,	/* Alt */
	' ',	/* Space bar */
	0,	/* Caps lock */
	0,	/* 59 - F1 key ... > */
	0,   0,   0,   0,   0,   0,   0,   0,
	0,	/* < ... F10 */
	0,	/* 69 - Num lock*/
	0,	/* Scroll Lock */
	0,	/* Home key */
	0,	/* Up Arrow */
	0,	/* Page Up */
	'-',
	0,	/* Left Arrow */
	0,
	0,	/* Right Arrow */
	'+',
	0,	/* 79 - End key*/
	0,	/* Down Arrow */
	0,	/* Page Down */
	0,	/* Insert Key */
	0,	/* Delete Key */
	0,   0,   0,
	0,	/* F11 Key */
	0,	/* F12 Key */
	0,	/* All other keys are undefined */
};

Keyboard::Keyboard( Timer &timer, IRQ& irq ) : timer(timer), irq(irq)
{ }

char Keyboard::getch(){
	while( keyboard_buffer.is_empty() ){
		timer.wait(3);
	}
	return keyboard_buffer.pop();
}

void Keyboard::handler(struct regs *r){
	unsigned char scancode;

	/* Read from the keyboard's data buffer */
	scancode = inb(0x60);

	/* If the top bit of the byte we read from the keyboard is
	 *  set, that means that a key has just been released */
	if (scancode & 0x80){
		/* You can use this one to see if the user released the
		 *  shift, alt, or control keys... */
	}else{
		/* Here, a key was just pressed. Please note that if you
		 *  hold a key down, you will get repeated key press
		 *  interrupts. */

		/* Just to show you how this works, we simply translate
		 *  the keyboard scancode into an ASCII value, and then
		 *  display it to the screen. You can get creative and
		 *  use some flags to see if a shift is pressed and use a
		 *  different layout, or you can add another 128 entries
		 *  to the above layout to correspond to 'shift' being
		 *  held. If shift is held using the larger lookup table,
		 *  you would add 128 to the scancode when you look for it */
//		char c[2];
//		c[0]=(char)(kbdus[scancode]);
//		c[1]=(char)('\0');

		kprintf("%c",kbdus[scancode]);

		if( kbdus[scancode]!=0 && ! keyboard_buffer.is_full() ){
			keyboard_buffer.push( kbdus[scancode] );
		}
	}
}

void Keyboard::install(){
	/* Installs 'handler' to IRQ1 */
	irq.install_handler(1, Keyboard::handler);
}

