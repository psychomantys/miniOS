#ifndef  paging_INC
#define  paging_INC

#include	<kernel/isr.hpp>
#include	<kernel/idt.hpp>
#include	<kernel/monitor.hpp>
#include	<kernel/kmalloc.hpp>
#include	<kernel/kpp/bitset.hpp>
#include	<isa_specific_code.hpp>
#include	<plataform_specific_code.hpp>

#include	<stdlib.h>

typedef struct page{
	uint32_t present    : 1;   // Page present in memory
	uint32_t rw         : 1;   // Read-only if clear, readwrite if set
	uint32_t user       : 1;   // Supervisor level only if clear
	uint32_t accessed   : 1;   // Has the page been accessed since last refresh?
	uint32_t dirty      : 1;   // Has the page been written to since last refresh?
	uint32_t unused     : 7;   // Amalgamation of unused and reserved bits
	uint32_t frame      : 20;  // Frame address (shifted right 12 bits)
} page_t;

typedef struct page_table{
	page_t pages[1024];
} page_table_t;

typedef struct page_directory{
	/**
	  Array of pointers to pagetables.
	 **/
	page_table_t *tables[1024];
	/**
	  Array of pointers to the pagetables above, but gives their *physical*
	  location, for loading into the CR3 register.
	 **/
	uint32_t tablesPhysical[1024];
	/**
	  The physical address of tablesPhysical. This comes into play
	  when we get our kernel heap allocated and the directory
	  may be in a different location in virtual memory.
	 **/
	uint32_t physicalAddr;
} page_directory_t;

class Paging{
	public:
		Paging( const size_t &mem_size, IDT &idt ) :
			frame_size(0x1000),
			frame_qtd(0x2000000/frame_size),
			frames(frame_qtd),
			idt(idt)
		{
			kernel_directory = (page_directory_t*)kmalloc_a(sizeof(page_directory_t));
			memset(kernel_directory, 0, sizeof(page_directory_t));
			current_directory = kernel_directory;

			uint32_t i=0;
			while( i<end_malloc_addr ){
				// Kernel code is readable but not writeable from userspace.
				alloc_frame( get_page(i, 1, kernel_directory), true, false);
				i += this->frame_size;
			}
		}

		/**
		 * Sets up the environment, page directories etc and
		 * enables paging.
		**/
		void install();

		/**
		 * Causes the specified page directory to be loaded into the
		 * CR3 register.
		**/
		void switch_page_directory(page_directory_t *new_page);

		/**
		 * Retrieves a pointer to the page required.
		 * If make == 1, if the page-table in which this page should
		 * reside isn't created, create it!
		**/
		page_t *get_page(uint32_t address, int make, page_directory_t *dir);

		/**
		 * Handler for page faults.
		 **/
		static void page_fault(struct regs *r);

	private:
		const uint32_t frame_size;
		const uint32_t frame_qtd;
		bitset frames;
		IDT &idt;

		page_directory_t *kernel_directory;
		page_directory_t *current_directory;


		void alloc_frame(page_t *page, const bool &is_kernel, const bool &is_writeable);
		void free_frame(page_t *page);
};

#endif   /* ----- #ifndef paging_INC  ----- */

