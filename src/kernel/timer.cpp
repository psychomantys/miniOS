/*
 * =====================================================================================
 *
 *       Filename:  timer.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/08/2013 12:39:16 PM
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
#include	<kernel/timer.hpp>

int Timer::timer_ticks=0;

Timer::Timer() { }

void Timer::phase(int hz){
	int divisor = 1193180 / hz;       /* Calculate our divisor */
	outb(0x43, 0x36);             /* Set our command byte 0x36 */
	outb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
	outb(0x40, divisor >> 8);     /* Set high byte of divisor */
}

/* Handles the timer. In this case, it's very simple: We
 *  increment the 'timer_ticks' variable every time the
 *  timer fires. By default, the timer fires 18.222 times
 *  per second. Why 18.222Hz? Some engineer at IBM must've
 *  been smoking something funky */
void Timer::handler(struct regs *r){
	/* Increment our 'tick count' */
	timer_ticks++;

	/* Every 18 clocks (approximately 1 second), we will
	 *  display a message on the screen */
	if( timer_ticks % 18 == 0){
		kprintf("One second has passed\n");
	}
}

/* Sets up the system clock by installing the timer handler
 *  into IRQ0 */
void Timer::install( IRQ &irq ){
	/* Installs 'timer_handler' to IRQ0 */
	irq.install_handler(0, Timer::handler);
}

/* This will continuously loop until the given time has
*  been reached */
void Timer::wait(const unsigned long &ticks){
	unsigned long eticks;

	eticks = timer_ticks + ticks;
	while(timer_ticks < eticks);
}

