#include	<stdint.h>
#include	<stdlib.h>
#include	<string.h>

#include	<kernel/idt.hpp>
#include	<isa_specific_code.hpp>

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
	idtp.limit = (sizeof(struct IDT_entry)*sizeof(idt) ) - 1;
	idtp.base = (uint32_t)(&idt);

	/* Clear out the entire IDT, initializing it to zeros */
	memset(&idt, 0, sizeof(struct IDT_entry)*sizeof(idt));

	/* Add any new ISRs to the IDT here using set_gate */

	/* Points the processor's internal register to the new IDT */
	IDT_load(idtp);
}

