#ifndef  idt_INC
#define  idt_INC

#include	<stdint.h>
#include	<kernel/idt.hpp>
#include	<isa_specific_code.hpp>

/* Defines an IDT entry */
struct IDT_entry{
	uint16_t	base_low;
	uint16_t	selector;        /* Our kernel segment goes here! */
	uint8_t	zero;     /* This will ALWAYS be set to 0! */
	uint8_t	flags;       /* Set using the above table! */
	uint16_t	base_high;
} __attribute__((packed));
/*struct IDT_entry IDT_entry;

union packet_IDT_entry{
	struct IDT_entry	p;
	uint64_t	s;
};
*/

struct IDT_ptr{
	uint16_t	limit;
	uint32_t	base;
} __attribute__((packed));
/*typedef struct IDT_ptr IDT_ptr;

union	packet_IDT_ptr{
	struct IDT_ptr	p;
	uint8_t	s[6];
};
*/
/* Declare an IDT of 256 entries. Although we will only use the
 *  first 32 entries in this tutorial, the rest exists as a bit
 *  of a trap. If any undefined IDT entry is hit, it normally
 *  will cause an "Unhandled Interrupt" exception. Any descriptor
 *  for which the 'presence' bit is cleared (0) will generate an
 *  "Unhandled Interrupt" exception */
//struct idt_entry idt[256];
//struct idt_ptr idtp;

class IDT{
	public:
		IDT();
		/* Use this function to set an entry in the IDT. Alot simpler
		 *  than twiddling with the GDT ;) */
		void set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
		/* Installs the IDT */
		void install();

	private:
		struct IDT_entry idt[256];
		struct IDT_ptr idtp;
//		union packet_IDT_entry idt[256];
//		union packet_IDT_ptr idtp;
};

#endif	/* ----- #ifndef idt_INC  ----- */

