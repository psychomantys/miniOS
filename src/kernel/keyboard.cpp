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
		char c[2];
		c[0]=(char)(kbdus[scancode]);
		c[1]=(char)('\0');
		kprintf( c );
	}
}

void Keyboard::install( IRQ &irq ){
	/* Installs 'handler' to IRQ1 */
	irq.install_handler(1, Keyboard::handler);
}

