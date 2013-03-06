/*
 * =====================================================================================
 *
 *       Filename:  kmalloc.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/31/2013 01:52:28 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include	<kernel/kmalloc.hpp>
#include	<kernel/monitor.hpp>
#include	<kernel/boot/multiboot.hpp>

static kmalloc_handler_t kmalloc_handler=0;
static kfree_handler_t kfree_handler=0;

inline uint32_t kmalloc_basic(const uint32_t &sz, uint32_t *phys, const bool &align){
	if( align && (end_malloc_addr & 0xFFFFF000) ){ // If the address is not already page-aligned
		// Align it.
		end_malloc_addr &= 0xFFFFF000;
		end_malloc_addr += 0x1000;
	}
	if (phys){
		*phys = end_malloc_addr;
	}
	uint32_t tmp = end_malloc_addr;
	end_malloc_addr += sz;
	return tmp;
}

inline uint32_t kmalloc_int(const uint32_t &sz, uint32_t *phys, const bool &align){
	if( kmalloc_handler ){
		return kmalloc_handler(sz, phys, align);
	}
	return kmalloc_basic(sz, phys, align);
}

extern "C" {
	uint32_t kmalloc(uint32_t sz){
		return kmalloc_int(sz, 0, false);
	}

	uint32_t kmalloc_a(const uint32_t &sz ){
		return kmalloc_int(sz, 0, true);
	} 

	uint32_t kmalloc_ap(const uint32_t &sz, uint32_t *phys){
		return kmalloc_int(sz, phys, true);
	}

	uint32_t kmalloc_p(uint32_t sz, uint32_t *phys){
		return kmalloc_int(sz, phys, false);
	}

	void kmalloc_set_handler( kmalloc_handler_t new_handler){
		kmalloc_handler=new_handler;
	}

	kmalloc_handler_t kmalloc_get_handler(){
		return kmalloc_handler;
	}

	void kfree(uint32_t addr){
		kprintf("Not free yet! %p\n",addr);
	}

	void kfree_set_handler(kfree_handler_t new_handler){
		kfree_handler=new_handler;
	}

	kfree_handler_t kfree_get_handler(){
		return kfree_handler;
	}
}

