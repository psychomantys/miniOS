/*
 * =====================================================================================
 *
 *       Filename:  paging.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/05/2013 01:46:18 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include	<kernel/paging.hpp>
#include	<kernel/kpp/bitset.hpp>
#include	<kernel/kpp/utils.hpp>
#include	<kernel/monitor.hpp>

// Function to allocate a frame.
void Paging::alloc_frame(page_t *page, const bool &is_kernel, const bool &is_writeable){
	if( page->frame!=0 ){
		return; // Frame was already allocated, return straight away.
	}else{
		size_t idx = frames.first(); // idx is now the index of the first free frame.
		if( idx==(size_t)-1 ){
			// PANIC is just a macro that prints a message to the screen then hits an infinite loop.
			PANIC("No free frames!");
		}
		frames.set(idx); // this frame is now ours!
		page->present = 1; // Mark it as present.

		page->rw=is_writeable; // Should the page be writeable?
		page->user=!is_kernel; // Should the page be user-mode?

//		page->rw=is_writeable?1:0; // Should the page be writeable?
//		page->user=is_kernel?0:1; // Should the page be user-mode?

		page->frame=idx;
	}
}

// Function to deallocate a frame.
void Paging::free_frame(page_t *page){
	uint32_t frame;
	if( !(frame=page->frame) ){
		return; // The given page didn't actually have an allocated frame!
	}else{
		frames.clear(frame); // Frame is now free again.
		page->frame = 0x0; // Page now doesn't have a frame.
	}
}

void Paging::install(){
	idt.set_gate(14, Paging::page_fault, 0x08, 0x8E);
	switch_page_directory(kernel_directory);
}

void Paging::switch_page_directory(page_directory_t *dir){
	current_directory = dir;
	asm volatile("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
	uint32_t cr0;
	asm volatile("mov %%cr0, %0": "=r"(cr0));
	cr0 |= 0x80000000; // Enable paging!
	asm volatile("mov %0, %%cr0":: "r"(cr0));
}

page_t *Paging::get_page(uint32_t address, int make, page_directory_t *dir){
	// Turn the address into an index.
	address /= this->frame_size;
	// Find the page table containing this address.
	uint32_t table_idx = address / 1024;
	if (dir->tables[table_idx]){ // If this table is already assigned
		return &dir->tables[table_idx]->pages[address%1024];
	}else if( make ){
		uint32_t tmp;
		dir->tables[table_idx] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &tmp);
		memset(dir->tables[table_idx], 0, this->frame_size);
		dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, US.
		return &dir->tables[table_idx]->pages[address%1024];
	}else{
		return 0;
	}
}

void Paging::page_fault(struct regs *r){
	// A page fault has occurred.
	// The faulting address is stored in the CR2 register.
	uint32_t faulting_address;
	asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

	// The error code gives us details of what happened.
	int present   = !(r->err_code & 0x1); // Page not present
	int rw = r->err_code & 0x2;           // Write operation?
	int us = r->err_code & 0x4;           // Processor was in user-mode?
	int reserved = r->err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
	int id = r->err_code & 0x10;          // Caused by an instruction fetch?

	// Output an error message.
	kprintf("Page fault! ( ");
	if (present) { kprintf("present ");}
	if (rw) { kprintf("read-only ");}
	if (us) { kprintf("user-mode ");}
	if (reserved) { kprintf("reserved ");}
	kprintf(") at 0x");
	kprintf("%p\n",faulting_address);
	PANIC("Page fault");
	halt_machine();
}

