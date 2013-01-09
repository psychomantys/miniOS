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

extern "C" void fault_handler(struct regs *r){
	if (r->int_no < 32){
		kprintf( exception_messages[r->int_no] );
		kprintf(": ISR LANCADO!!!!!! Desligando.\n");
		halt_machine();
		for (;;);
	}
	kprintf("ISR LANCADO!!!!!! Desligando.\n");
//	kprintf( exception_messages[r->int_no] );
	kprintf("\n");
	halt_machine();
}

