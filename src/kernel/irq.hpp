
#ifndef IRQ_HPP_
#define IRQ_HPP_

#include	<kernel/idt.hpp>

/* These are own ISRs that point to our special IRQ handler
 *  instead of the regular 'fault_handler' function */
extern "C" {
	void irq0();
	void irq1();
	void irq2();
	void irq3();
	void irq4();
	void irq5();
	void irq6();
	void irq7();
	void irq8();
	void irq9();
	void irq10();
	void irq11();
	void irq12();
	void irq13();
	void irq14();
	void irq15();
}

class IRQ {
	public:
		/* This array is actually an array of function pointers. We use
		 *  this to handle custom IRQ handlers for a given IRQ */
		static void *irq_routines[16];

		/* This installs a custom IRQ handler for the given IRQ */
		void install_handler(const int &irq, void (*handler)(struct regs *r));

		/* This clears the handler for a given IRQ */
		void uninstall_handler(const int &irq);

		/* Normally, IRQs 0 to 7 are mapped to entries 8 to 15. This
		 *  is a problem in protected mode, because IDT entry 8 is a
		 *  Double Fault! Without remapping, every time IRQ0 fires,
		 *  you get a Double Fault Exception, which is NOT actually
		 *  what's happening. We send commands to the Programmable
		 *  Interrupt Controller (PICs - also called the 8259's) in
		 *  order to make IRQ0 to 15 be remapped to IDT entries 32 to
		 *  47 */
		void remap(void);

		/* We first remap the interrupt controllers, and then we install
		 *  the appropriate ISRs to the correct entries in the IDT. This
		 *  is just like installing the exception handlers */
		void install( IDT &idt );
};

/* Each of the IRQ ISRs point to this function, rather than
 *  the 'fault_handler' in 'isrs.c'. The IRQ Controllers need
 *  to be told when you are done servicing them, so you need
 *  to send them an "End of Interrupt" command (0x20). There
 *  are two 8259 chips: The first exists at 0x20, the second
 *  exists at 0xA0. If the second controller (an IRQ from 8 to
 *  15) gets an interrupt, you need to acknowledge the
 *  interrupt at BOTH controllers, otherwise, you only send
 *  an EOI command to the first controller. If you don't send
 *  an EOI, you won't raise any more IRQs */
extern "C" void irq_handler(struct regs *r);

#endif /*  IRQ_HPP_ */

