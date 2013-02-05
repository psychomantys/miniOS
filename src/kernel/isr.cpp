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
		kprintf("%s: ISR LANCADO!!!!!! Desligando.\n", exception_messages[r->int_no]);
		halt_machine();
	}
	kprintf("%s: ISR LANCADO!!!!!! Desligando.\n", exception_messages[r->int_no]);
	halt_machine();
}

