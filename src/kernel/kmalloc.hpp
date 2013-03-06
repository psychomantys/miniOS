#ifndef  kmalloc_INC
#define  kmalloc_INC

#include	<stdint.h>
#include	<kernel/boot/multiboot.hpp>

extern "C" {
	extern uint32_t end_malloc_addr;

	typedef uint32_t (*kmalloc_handler_t)(const uint32_t &sz, uint32_t *phys, const bool &align);
	typedef uint32_t (*kfree_handler_t)(const uint32_t addr);

	// page aligned.
	uint32_t kmalloc_a(const uint32_t &sz);

	// returns a physical address.
	uint32_t kmalloc_p(uint32_t sz, uint32_t *phys);

	// page aligned and returns a physical address.
	uint32_t kmalloc_ap(const uint32_t &sz, uint32_t *phys);

	// vanilla (normal).
	uint32_t kmalloc(uint32_t sz);
	
	void kmalloc_set_handler(kmalloc_handler_t new_handler);
	kmalloc_handler_t kmalloc_get_handler();

	void kfree(uint32_t addr);

	void kfree_set_handler(kfree_handler_t new_handler);
	kfree_handler_t kfree_get_handler();
}

#endif   /* ----- #ifndef kmalloc_INC  ----- */

