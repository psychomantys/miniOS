
#ifndef  MINIOS_ISR_HPP
#define  MINIOS_ISR_HPP

#include	<kernel/idt.hpp>

extern "C" {
	void isr0();
	void isr1();
	void isr2();
	void isr3();
	void isr4();
	void isr5();
	void isr6();
	void isr7();
	void isr8();
	void isr9();
	void isr10();
	void isr11();
	void isr12();
	void isr13();
	void isr14();
	void isr15();
	void isr16();
	void isr17();
	void isr18();
	void isr19();
	void isr20();
	void isr21();
	void isr22();
	void isr23();
	void isr24();
	void isr25();
	void isr26();
	void isr27();
	void isr28();
	void isr29();
	void isr30();
	void isr31();
}
/*

#define INTERRUPT(vector) \
__asm__(".global isr_vector_" #vector "\n"\
"isr_vector_" #vector ":\n" \
"	pusha;" \
"	push ds;" \
"	push es;" \
"	push fs;" \
"	push gs;" \
"	mov ax, 0x10;" \
"	mov ds, ax;" \
"	mov es, ax;" \
"	mov fs, ax;" \
"	mov gs, ax;" \
"	mov eax, esp;" \
"	push eax;" \
"	mov eax, handle_isr_vector_" #vector ";" \
"	call eax;" \
"	pop eax;" \
"	pop gs;" \
"	pop fs;" \
"	pop es;" \
"	pop ds;" \
"	popa;" \
"	iret;")
*/



/* This is a simple string array. It contains the message that
 *  corresponds to each and every exception. We get the correct
 *  message by accessing like:
 *  exception_message[interrupt_number] */
static const char *exception_messages[] = {
	"Division By Zero",
	"Debug",
	"Non Maskable Interrupt",
	"Breakpoint",
	"Into Detected Overflow",
	"Out of Bounds",
	"Invalid Opcode",
	"No Coprocessor",

	"Double Fault",
	"Coprocessor Segment Overrun",
	"Bad TSS",
	"Segment Not Present",
	"Stack Fault",
	"General Protection Fault",
	"Page Fault",
	"Unknown Interrupt",

	"Coprocessor Fault",
	"Alignment Check",
	"Machine Check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",

	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"
};

/* All of our Exception handling Interrupt Service Routines will
 *  point to this function. This will tell us what exception has
 *  happened! Right now, we simply halt the system by hitting an
 *  endless loop. All ISRs disable interrupts while they are being
 *  serviced as a 'locking' mechanism to prevent an IRQ from
 *  happening and messing up kernel data structures */
extern "C" void fault_handler(struct regs *r);

#endif   /* ----- #ifndef MINIOS_ISR_HPP  ----- */

