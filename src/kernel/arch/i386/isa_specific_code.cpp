
#include	<isa_specific_code.hpp>
#include	<kernel/gdt.hpp>
#include	<config.hpp>

#include	<stdint.h>

extern "C" {
	void GDT_flush( const volatile GDT_ptr &new_gdt ){
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

	void IDT_load( const volatile IDT_ptr &new_idt ){
		asm volatile("lidt (%0)" : : "dN" (&new_idt) );
	}

	void halt_machine(){
		asm volatile(".hang:\n\t"
		"hlt\n\t"
		"jmp  .hang");
	}

	void disable_interrupts(){
		asm volatile("cli");
	}

	void enable_interrupts(){
		asm volatile("sti");
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


#define macro_get_reg_func(reg) void get_register_##reg (volatile uint32_t &ret){ asm volatile("mov %% " #reg ", %0" : "=r"(ret) );	}

#define macro_set_reg_func(reg) void set_register_##reg (volatile uint32_t ret){ asm volatile("mov %0, %%" #reg : "=r"(ret) );	}

	macro_get_reg_func(eax)
	macro_get_reg_func(ebx)
	macro_get_reg_func(ebp)
	macro_get_reg_func(esp)
	macro_get_reg_func(cr0)
	macro_get_reg_func(cr1)
	macro_get_reg_func(cr2)
	macro_get_reg_func(cr3)

	macro_set_reg_func(eax)
	macro_set_reg_func(ebx)
	macro_set_reg_func(ebp)
	macro_set_reg_func(esp)
	macro_set_reg_func(cr0)
	macro_set_reg_func(cr1)
	macro_set_reg_func(cr2)
	macro_set_reg_func(cr3)
}

