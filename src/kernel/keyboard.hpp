#ifndef _KEYBOARD_HPP_
#define _KEYBOARD_HPP_

#include	<kernel/irq.hpp>
#include	<kernel/timer.hpp>
#include	<kernel/kpp/queue.hpp>

class Keyboard{
	private:
		static SQueue<char> keyboard_buffer;
		Timer &timer;
		IRQ &irq;
	public:
		Keyboard( Timer &timer, IRQ &irq );
		void install();
		static void handler(struct regs *r);
		char getch();
};

#endif	/* _KEYBOARD_HPP_ */

