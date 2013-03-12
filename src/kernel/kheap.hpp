#ifndef  KERNEL_KHEAP_INC
#define  KERNEL_KHEAP_INC

#include	<kernel/paging.hpp>
#include	<kernel/kpp/utils.hpp>
#include	<kernel/kpp/ordered_array.hpp>
#include	<stdint.h>

#define	KHEAP_START	0xC0000000
#define	KHEAP_INITIAL_SIZE	0x100000
#define	KHEAP_INDEX_SIZE	0x20000
#define	KHEAP_MAGIC	0x123890AB
#define	KHEAP_MIN_SIZE	0x70000

/**
  Size information for a hole/block
 **/
typedef struct{
	uint32_t magic;   // Magic number, used for error checking and identification.
	bool is_hole;   // true if this is a hole. false if this is a block.
	uint32_t size;    // size of the block, including the end footer.
} header_t;

const bool operator<( const header_t &lhs, const header_t &rhs );
/*{
	return lhs.size<rhs.size;
}*/

typedef struct{
	uint32_t magic;     // Magic number, same as in header_t.
	header_t *header; // Pointer to the block header.
} footer_t;

class KHeap{
		declare_am(uint32_t,start_address); // The start of our allocated space.
		declare_am(uint32_t,end_address);  // The end of our allocated space. May be expanded up to max_address.
		declare_am(uint32_t,max_address);  // The maximum address the heap can be expanded to.
		declare_am(bool,supervisor);     // Should extra pages requested by us be mapped as supervisor-only?
		declare_am(bool,readonly);       // Should extra pages requested by us be mapped as read-only?
	private:
		uint32_t find_smallest_hole(const uint32_t &size, const bool &page_align);
		void expand( uint32_t new_size);
		uint32_t contract(uint32_t new_size);
	public:
		Paging &paging;
		Ordered_array<header_t*> *index;

		/**
		  Create a new heap.
		**/
		KHeap( Paging &paging);
		void install(uint32_t start, const uint32_t &end, const uint32_t &max, const bool &supervisor, const bool &readonly);

		/**
		  Allocates a contiguous region of memory 'size' in size. If
		  is_page_align==true, it creates that block starting
		  on a page boundary.
		 **/
		void *alloc(uint32_t size, const bool &is_page_align);

		/**
		  Releases a block allocated with 'alloc'.
		 **/
		void free(void *p);
};

#endif	/* ----- #ifndef kheap_INC  ----- */

