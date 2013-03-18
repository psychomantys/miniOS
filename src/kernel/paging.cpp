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
#include	<isa_specific_code.hpp>

// Function to allocate a frame.
void Paging::alloc_frame(page_t *page, const bool &is_kernel, const bool &is_writeable){
	if( page->frame!=0 ){
		return; // Frame was already allocated, return straight away.
	}else{
		size_t idx = frames.first(); // idx is now the index of the first free frame.
		if( idx==(size_t)(-1) ){
			// PANIC is just a macro that prints a message to the screen then hits an infinite loop.
			PANIC("No free frames!");
		}
		frames.set(idx); // this frame is now ours!
		page->present = 1; // Mark it as present.

		page->rw=is_writeable; // Should the page be writeable?
		page->user=!is_kernel; // Should the page be user-mode?

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
	set_register_cr3((uint32_t)(dir->tablesPhysical));
//	asm volatile("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
	uint32_t cr0;
	get_register_cr0(cr0);
//	asm volatile("mov %%cr0, %0": "=r"(cr0));
	cr0 |= 0x80000000; // Enable paging!
	set_register_cr0(cr0);
//	asm volatile("mov %0, %%cr0":: "r"(cr0));
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
//	asm volatile("mov %%cr2, %0" : "=r" (faulting_address));
	get_register_cr2(faulting_address);

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
	if (id) { kprintf("instruction-fetch ");}
	if (reserved) { kprintf("reserved ");}
	kprintf(") at 0x%p\n",faulting_address);
	PANIC("Page fault");
	halt_machine();
}

page_directory_t *Paging::clone_directory(page_directory_t *src){
	uint32_t phys;
	// Make a new page directory and obtain its physical address.
	page_directory_t *dir = (page_directory_t*)kmalloc_ap(sizeof(page_directory_t), &phys);
	// Ensure that it is blank.
	memset(dir, 0, sizeof(page_directory_t));

	// Get the offset of tablesPhysical from the start of the page_directory_t structure.
	uint32_t offset = (uint32_t)dir->tablesPhysical - (uint32_t)dir;

	// Then the physical address of dir->tablesPhysical is:
	dir->physicalAddr = phys + offset;

	// Go through each page table. If the page table is in the kernel directory, do not make a new copy.
	int i;
	for (i = 0; i < 1024; i++)
	{
		if (!src->tables[i])
			continue;

		if (kernel_directory->tables[i] == src->tables[i])
		{
			// It's in the kernel, so just use the same pointer.
			dir->tables[i] = src->tables[i];
			dir->tablesPhysical[i] = src->tablesPhysical[i];
		}
		else
		{
			// Copy the table.
			uint32_t phys;
			dir->tables[i] = clone_table(src->tables[i], &phys);
			dir->tablesPhysical[i] = phys | 0x07;
		}
	}
	return dir;
}


void copy_page_physical( uint32_t dest, uint32_t src){
	disable_interrupts();

	register uint32_t cr0;
	get_register_cr0(cr0);
	cr0 &= 0x7fffffff;
	set_register_cr0(cr0);

	memset(dest, src, 4*1024 );

	get_register_cr0(cr0);
	cr0 |= 0x80000000;
	set_register_cr0(cr0);
}


page_table_t *Paging::clone_table(page_table_t *src, uint32_t *physAddr){
    // Make a new page table, which is page aligned.
    page_table_t *table = (page_table_t*)kmalloc_ap(sizeof(page_table_t), physAddr);
    // Ensure that the new table is blank.
    memset(table, 0, sizeof(page_directory_t));

    // For every entry in the table...
    int i;
    for (i = 0; i < 1024; i++)
    {
        // If the source entry has a frame associated with it...
        if (src->pages[i].frame)
        {
            // Get a new frame.
            alloc_frame(&table->pages[i], 0, 0);
            // Clone the flags from source to destination.
            if (src->pages[i].present) table->pages[i].present = 1;
            if (src->pages[i].rw) table->pages[i].rw = 1;
            if (src->pages[i].user) table->pages[i].user = 1;
            if (src->pages[i].accessed) table->pages[i].accessed = 1;
            if (src->pages[i].dirty) table->pages[i].dirty = 1;
            // Physically copy the data across. This function is in process.s.
            copy_page_physical(src->pages[i].frame*0x1000, table->pages[i].frame*0x1000);
        }
    }
    return table;
}

