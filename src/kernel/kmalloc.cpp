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

extern "C" {
	uint32_t kmalloc(uint32_t sz){
		kprintf("end_malloc_addr=%p\n",end_malloc_addr);
		uint32_t tmp = end_malloc_addr;
		end_malloc_addr += sz;
		return tmp;
	}

	uint32_t kmalloc_a(const uint32_t &sz ){
		if( (end_malloc_addr & 0xFFFFF000) ){ // If the address is not already page-aligned
			// Align it.
			end_malloc_addr &= 0xFFFFF000;
			end_malloc_addr += 0x1000;
		}
		uint32_t tmp=end_malloc_addr;
		end_malloc_addr += sz;
		return tmp;
	} 

	uint32_t kmalloc_ap(const uint32_t &sz, uint32_t *phys){
		if( (end_malloc_addr & 0xFFFFF000) ){ // If the address is not already page-aligned
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

	uint32_t kmalloc_p(uint32_t sz, uint32_t *phys){
		if (phys){
			*phys = end_malloc_addr;
		}
		uint32_t tmp = end_malloc_addr;
		end_malloc_addr += sz;
		return tmp;
	}

}

