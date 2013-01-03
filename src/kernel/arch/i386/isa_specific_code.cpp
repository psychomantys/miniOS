
#include	<isa_specific_code.hpp>
#include	<kernel/gdt.hpp>

#include	<stdint.h>

extern "C" {
	void GDT_flush( const GDT_ptr &new_gdt ){
		asm volatile ("lgdt (%0)" : : "dN" (&new_gdt) );
		asm volatile ("mov $0x10, %ax\n\t"
			"mov %ax, %ds\n\t"
			"mov %ax, %es\n\t"
			"mov %ax, %fs\n\t"
			"mov %ax, %gs\n\t"
			"mov %ax, %ss\n\t"
			"ljmp  $0x8,$gdt_jmp_flush\n"
			"gdt_jmp_flush:\n\t"
		);
	}

	void IDT_load( const IDT_ptr &new_idt ){
		asm volatile ("lidt (%0)" : : "dN" (&new_idt) );
	}

	void halt_machine(){
		asm(".hang:\n\t"
		"hlt\n\t"
		"jmp  .hang");
	}

	void disable_interrupts(){
		asm("cli");
	}

	// Write a byte out to the specified port.
	void outb(const uint16_t &port, const uint8_t &value){
		asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
	}

	uint8_t inb(const uint16_t &port){
		uint8_t ret;
		asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
		return ret;
	}

	uint16_t inw(const uint16_t &port){
		uint16_t ret;
		asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
		return ret;
	}
}

