/*
 * =====================================================================================
 *
 *       Filename:  isr.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/06/2013 10:00:17 PM
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
#include	<kernel/isr.hpp>
#include	<kernel/monitor.hpp>

/* This is a simple string array. It contains the message that
 *  corresponds to each and every exception. We get the correct
 *  message by accessing like:
 *  exception_message[interrupt_number] */
extern "C" {
	
const char *exception_messages[] = {
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
}
extern "C" void fault_handler(struct regs *r){
	if (r->int_no < 32){
		kprintf("%s: ISR LANCADO!!!!!! Desligando.\n", exception_messages[r->int_no]);
		halt_machine();
	}
	kprintf("%s: ISR LANCADO!!!!!! Desligando.\n", exception_messages[r->int_no]);
	halt_machine();
}

