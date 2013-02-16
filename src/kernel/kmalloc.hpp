#ifndef  kmalloc_INC
#define  kmalloc_INC

#include	<stdint.h>
#include	<kernel/boot/multiboot.hpp>
#include	<kernel/kmalloc.hpp>

extern "C" {
//	static uint32_t end_malloc_addr=multiboot_addr->bss_end_addr;
	extern uint32_t end_malloc_addr;
//	extern uint32_t end;
//	static uint32_t end_malloc_addr=end_malloc_addr;

	// page aligned.
	uint32_t kmalloc_a(const uint32_t &sz);

	// returns a physical address.
	uint32_t kmalloc_p(uint32_t sz, uint32_t *phys);

	// page aligned and returns a physical address.
	uint32_t kmalloc_ap(const uint32_t &sz, uint32_t *phys);

	// vanilla (normal).
	uint32_t kmalloc(uint32_t sz);
}

#endif   /* ----- #ifndef kmalloc_INC  ----- */

