#ifndef	_TIMER_HPP_
#define	_TIMER_HPP_

#include	<kernel/irq.hpp>

class Timer{
	private:
		/* This will keep track of how many ticks that the system
		 *  has been running for */
		static int timer_ticks;

	public:
		void phase(int hz);
		Timer();

		/* Handles the timer. In this case, it's very simple: We
		 *  increment the 'timer_ticks' variable every time the
		 *  timer fires. By default, the timer fires 18.222 times
		 *  per second. Why 18.222Hz? Some engineer at IBM must've
		 *  been smoking something funky */
		static void handler(struct regs *r);

		/* Sets up the system clock by installing the timer handler
		 *  into IRQ0 */
		void install( IRQ &irq );

		/* This will continuously loop until the given time has
		*  been reached */
		void wait(const unsigned long &ticks);
};

#endif	/*  _TIMER_HPP_ */

