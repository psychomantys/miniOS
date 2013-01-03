
#include	<kernel/gdt.hpp>
#include	<isa_specific_code.hpp>

GDT::GDT(){}

/* Setup a descriptor in the Global Descriptor Table */
void GDT::set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran){
	/* Setup the descriptor base address */
	this->gdt[num].base_low = (base & 0xFFFF);
	this->gdt[num].base_middle = (base >> 16) & 0xFF;
	this->gdt[num].base_high = (base >> 24) & 0xFF;

	/* Setup the descriptor limits */
	gdt[num].limit_low = (limit & 0xFFFF);
	gdt[num].granularity = ((limit >> 16) & 0x0F);

	/* Finally, set up the granularity and access flags */
	gdt[num].granularity |= (gran & 0xF0);
	gdt[num].access = access;
}

/* Should be called by main. This will setup the special GDT
 *  pointer, set up the first 3 entries in our GDT, and then
 *  finally call gdt_flush() in our assembler file in order
 *  to tell the processor where the new GDT is and update the
 *  new segment registers */
void GDT::install(){
	/* Setup the GDT pointer and limit */
	this->gp.limit = (sizeof(struct GDT_entry) * 3) - 1;
	this->gp.base = (uint32_t)(&(this->gdt));

	/* Our NULL descriptor */
	this->set_gate(0, 0, 0, 0, 0);

	/* The second entry is our Code Segment. The base address
	 *  is 0, the limit is 4GBytes, it uses 4KByte granularity,
	 *  uses 32-bit opcodes, and is a Code Segment descriptor.
	 *  Please check the table above in the tutorial in order
	 *  to see exactly what each value means */
	this->set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

	/* The third entry is our Data Segment. It's EXACTLY the
	 *  same as our code segment, but the descriptor type in
	 *  this entry's access byte says it's a Data Segment */
	this->set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

	/* Flush out the old GDT and install the new changes! */
	GDT_flush( this->gp );
}

