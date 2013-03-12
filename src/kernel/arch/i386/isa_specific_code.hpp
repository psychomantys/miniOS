/*
 * =====================================================================================
 *
 *       Filename:  isa_specific_code.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/18/2012 02:31:18 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef	_ISA_SPECIFIC_CODE__INC
#define	_ISA_SPECIFIC_CODE__INC

#include	<stdint.h>
#include	<kernel/gdt.hpp>

// Some nice typedefs, to standardise sizes across platforms.
// These typedefs are written for 32-bit X86.
typedef	uint32_t	dword_t;
typedef	uint64_t	word_t;

extern "C" {
	void halt_machine();
	void disable_interrupts();
	void enable_interrupts();

	void outb(const uint16_t &port, const uint8_t &value);
	uint8_t inb(const uint16_t &port);
	uint16_t inw(const uint16_t &port);
	void GDT_flush( const volatile struct GDT_ptr &new_gdt );
	void IDT_load( const volatile struct IDT_ptr &new_idt );

	void get_register_eax(volatile uint32_t &ret);
	void get_register_ebx(volatile uint32_t &ret);

	void get_register_cr0( volatile uint32_t &ret);
	void get_register_cr1( volatile uint32_t &ret);
	void get_register_cr2( volatile uint32_t &ret);
	void get_register_cr3( volatile uint32_t &ret);

	void set_register_eax(volatile uint32_t ret);
	void set_register_ebx(volatile uint32_t ret);

	void set_register_cr0( volatile uint32_t ret);
	void set_register_cr1( volatile uint32_t ret);
	void set_register_cr2( volatile uint32_t ret);
	void set_register_cr3( volatile uint32_t ret);
}

#endif	/* ----- #ifndef _ISA_SPECIFIC_CODE__INC  ----- */

