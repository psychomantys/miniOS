#include	<stdint.h>
#include	<stdlib.h>
#include	<string.h>

#include	<isa_specific_code.hpp>
#include	<kernel/idt.hpp>
#include	<kernel/isr.hpp>

volatile struct IDT_entry IDT::idt[256];
volatile struct IDT_ptr IDT::idtp;

IDT::IDT(){}

/* Declare an IDT of 256 entries. Although we will only use the
 *  first 32 entries in this tutorial, the rest exists as a bit
 *  of a trap. If any undefined IDT entry is hit, it normally
 *  will cause an "Unhandled Interrupt" exception. Any descriptor
 *  for which the 'presence' bit is cleared (0) will generate an
 *  "Unhandled Interrupt" exception */

/* Use this function to set an entry in the IDT. Alot simpler
*  than twiddling with the GDT ;) */
void IDT::set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags){
	/* We'll leave you to try and code this function: take the
	 *  argument 'base' and split it up into a high and low 16-bits,
	 *  storing them in idt[num].base_hi and base_lo. The rest of the
	 *  fields that you must set in idt[num] are fairly self-
	 *  explanatory when it comes to setup */
	/* The interrupt routine's base address */
	idt[num].base_low = (base & 0xFFFF);
	idt[num].base_high = (base >> 16) & 0xFFFF;

	/* The segment or 'selector' that this IDT entry will use
	 *  is set here, along with any access flags */
	idt[num].selector = sel;
	idt[num].zero = 0;
	idt[num].flags = flags;
}

/* Installs the IDT */
void IDT::install(){
	/* Sets the special IDT pointer up, just like in 'gdt.c' */
//	idtp.p.limit = (sizeof(struct IDT_entry)*sizeof(idt) ) - 1;
//	idtp.p.base = (uint32_t)(&idt);
	idtp.limit = ( sizeof(struct IDT_entry)*256 ) - 1;
	idtp.base = (uint32_t)(&idt);
//	idtp.base = &idt;

	/* Clear out the entire IDT, initializing it to zeros */
	memset((void*)(&idt), 0, sizeof(struct IDT_entry)*256 );

	/* Add any new ISRs to the IDT here using set_gate */
	set_gate(0, (unsigned)isr0, 0x08, 0x8E);
	set_gate(1, (unsigned)isr1, 0x08, 0x8E);
	set_gate(2, (unsigned)isr2, 0x08, 0x8E);
	set_gate(3, (unsigned)isr3, 0x08, 0x8E);
	set_gate(4, (unsigned)isr4, 0x08, 0x8E);
	set_gate(5, (unsigned)isr5, 0x08, 0x8E);
	set_gate(6, (unsigned)isr6, 0x08, 0x8E);
	set_gate(7, (unsigned)isr7, 0x08, 0x8E);
	set_gate(8, (unsigned)isr8, 0x08, 0x8E);
	set_gate(9, (unsigned)isr9, 0x08, 0x8E);
	set_gate(10, (unsigned)isr10, 0x08, 0x8E);
	set_gate(11, (unsigned)isr11, 0x08, 0x8E);
	set_gate(12, (unsigned)isr12, 0x08, 0x8E);
	set_gate(13, (unsigned)isr13, 0x08, 0x8E);
	set_gate(14, (unsigned)isr14, 0x08, 0x8E);
	set_gate(15, (unsigned)isr15, 0x08, 0x8E);
	set_gate(16, (unsigned)isr16, 0x08, 0x8E);
	set_gate(17, (unsigned)isr17, 0x08, 0x8E);
	set_gate(18, (unsigned)isr18, 0x08, 0x8E);
	set_gate(19, (unsigned)isr19, 0x08, 0x8E);
	set_gate(20, (unsigned)isr20, 0x08, 0x8E);
	set_gate(21, (unsigned)isr21, 0x08, 0x8E);
	set_gate(22, (unsigned)isr22, 0x08, 0x8E);
	set_gate(23, (unsigned)isr23, 0x08, 0x8E);
	set_gate(24, (unsigned)isr24, 0x08, 0x8E);
	set_gate(25, (unsigned)isr25, 0x08, 0x8E);
	set_gate(26, (unsigned)isr26, 0x08, 0x8E);
	set_gate(27, (unsigned)isr27, 0x08, 0x8E);
	set_gate(28, (unsigned)isr28, 0x08, 0x8E);
	set_gate(29, (unsigned)isr29, 0x08, 0x8E);
	set_gate(30, (unsigned)isr30, 0x08, 0x8E);
	set_gate(31, (unsigned)isr31, 0x08, 0x8E);
	/*
*/

	/* Points the processor's internal register to the new IDT */
	IDT_load(idtp);
}

