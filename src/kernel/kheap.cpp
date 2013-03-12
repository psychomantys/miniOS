/*
 * =====================================================================================
 *
 *       Filename:  kheap.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/26/2013 07:34:24 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include	<kernel/kheap.hpp>
#include	<string.h>
#include	<kernel/debug.hpp>

const bool operator<( const header_t &lhs, const header_t &rhs ){
	return lhs.size<rhs.size;
}

uint32_t KHeap::find_smallest_hole(const uint32_t &size, const bool &page_align){
	// Find the smallest hole that will fit.
	uint32_t iterator=0;
	while( iterator<index->size() ){
//		header_t *header = (header_t *)lookup_ordered_array(iterator, &heap->index);
		header_t *header=(header_t*)((*index)[iterator]);
		// If the user has requested the memory be page-aligned
		if( page_align>0 ){
			// Page-align the starting point of this header.
			uint32_t location=(uint32_t)(header);
			uint32_t offset=0;
			if( ( (location+sizeof(header_t) )&0xFFFFF000 ) != 0 ){
				offset = 0x1000 /* page size */  - (location+sizeof(header_t))%0x1000;
			}
			uint32_t hole_size=(uint32_t)(header->size - offset);
			// Can we fit now?
			if( hole_size>=(uint32_t)(size) ){
				break;
			}
		}else if( header->size>=size ){
			break;
		}
		++iterator;
	}
	// Why did the loop exit?
	if( iterator==index->size() ){
		return -1; // We got to the end and didn't find anything.
	}else{
		return iterator;
	}
} 

static KHeap *kheap_kfree_handler=0;

void kheap_kfree(uint32_t p){
	kdebug(8,"KHeap *kheap_kfree_handler=%p\n",kheap_kfree_handler);
	kheap_kfree_handler->free(p);
}

static KHeap *kheap_kmalloc_handler=0;

inline uint32_t kheap_kmalloc(const uint32_t &sz, uint32_t *phys, const bool &align){
	uint32_t addr=(uint32_t)( kheap_kmalloc_handler->alloc(sz, align) );
	if( phys!=0 ){
		page_t *page=kheap_kmalloc_handler->paging.get_page( (uint32_t)(addr), false, kheap_kmalloc_handler->paging.kernel_directory);
// ?
		*phys = (page->frame*0x1000) + (uint32_t)(addr&0xFFF);
	}
	return (uint32_t)(addr);
}

KHeap::KHeap(Paging &paging ) :
	paging(paging)
{ }

void KHeap::install(uint32_t start, const uint32_t &end, const uint32_t &max, const bool &supervisor, const bool &readonly)
{
	index=new Ordered_array<header_t*>( (void*)(start), KHEAP_INDEX_SIZE );
////	index=(Ordered_array<header_t*>*)( kmalloc( sizeof(index) ) );
////	index->init( (void*)(start), KHEAP_INDEX_SIZE );
//	heap_t *heap = (heap_t*)( kmalloc(sizeof(heap_t)) );

	// All our assumptions are made on startAddress and endAddress being page-aligned.
	ASSERT(start%0x1000 == 0);
	ASSERT(end_address()%0x1000 == 0);

	// Initialise the index.
//	heap->index = place_ordered_array( (void*)start, HEAP_INDEX_SIZE, &header_t_less_than);

	// Shift the start address forward to resemble where we can start putting data.
//	start += sizeof(type_t)*HEAP_INDEX_SIZE;
	start += index->abs_size();

	// Make sure the start address is page-aligned.
	if( (start&0xFFFFF000)!=0 ){
		start &= 0xFFFFF000;
		start += 0x1000;
	}
	// Write the start, end and max addresses into the heap structure.
	start_address(start);
	end_address(end);
	max_address(max);
	this->supervisor(supervisor);
	this->readonly(readonly);

	// We start off with one large hole in the index.
	header_t *hole=(header_t *)(start);
	hole->size=end_address()-start_address();
	hole->magic=KHEAP_MAGIC;
	hole->is_hole=true;
/*	kprintf("FREEEEEEEE!!! header->magic=%p\n",hole->magic);
	kprintf("FREEEEEEEE! header->size=%u\n",hole->size);
	kprintf("FREEEEEEEE! header=%p\n",hole);*/
	index->insert(hole);

	kheap_kfree_handler=this;
	kfree_set_handler(kheap_kfree);

	kheap_kmalloc_handler=this;
	kmalloc_set_handler(kheap_kmalloc);
}

void KHeap::expand( uint32_t new_size ){
	// Sanity check.
	ASSERT(new_size>end_address()-start_address());
	// Get the nearest following page boundary.
	if( (new_size&0xFFFFF000)!=0 ){
		new_size &= 0xFFFFF000;
		new_size += 0x1000;
	}
	// Make sure we are not overreaching ourselves.
	ASSERT( start_address()+new_size<=max_address());

	// This should always be on a page boundary.
	uint32_t old_size=end_address()-start_address();
	uint32_t i=old_size;
	while( i<new_size ){
		paging.alloc_frame( paging.get_page(start_address()+i, true, paging.kernel_directory),
				supervisor(), !readonly() );
		i += 0x1000 /* page size */;
	}
	end_address( start_address()+new_size );
}

uint32_t KHeap::contract(uint32_t new_size){
	// Sanity check.
	ASSERT( new_size<end_address()-start_address());
	// Get the nearest following page boundary.
	if( new_size&0x1000 ){
		new_size&=0x1000;
		new_size+=0x1000;
	}
	// Don't contract too far!
	if( new_size<KHEAP_MIN_SIZE ){
		new_size = KHEAP_MIN_SIZE;
	}
	uint32_t old_size=end_address()-start_address();
	uint32_t i=old_size-0x1000;
	while( new_size<i ){
		paging.free_frame( paging.get_page(start_address()+i, false, paging.kernel_directory) );
		i -= 0x1000;
	}
	end_address( start_address()+new_size );
	return new_size;
}

void *KHeap::alloc(uint32_t size, const bool &page_align){
	// Make sure we take the size of header/footer into account.
	uint32_t new_size = size + sizeof(header_t) + sizeof(footer_t);
	// Find the smallest hole that will fit.
	uint32_t iterator = find_smallest_hole(new_size, page_align);

	if( iterator==(uint32_t)(-1) ){ // If we didn't find a suitable hole
		// Save some previous data.
		uint32_t old_length=end_address()-start_address();
		uint32_t old_end_address=end_address();

		// We need to allocate some more space.
		expand(old_length+new_size);
		uint32_t new_length=end_address()-start_address();

		// Find the endmost header. (Not endmost in size, but in location).
		iterator=0;
		// Vars to hold the index of, and value of, the endmost header found so far.
		uint32_t idx=-1;
		uint32_t value = 0x0;
		while( iterator<index->size() ){
			uint32_t tmp=(uint32_t)( (*index)[iterator] );
			if( tmp>value ){
				value=tmp;
				idx=iterator;
			}
			++iterator;
		}

		// If we didn't find ANY headers, we need to add one.
		if( idx==(uint32_t)(-1) ){
			header_t *header=(header_t*)(old_end_address);
			header->magic=KHEAP_MAGIC;
			header->size=new_length-old_length;
			header->is_hole=true;
			footer_t *footer=(footer_t *)(old_end_address+header->size-sizeof(footer_t));
			footer->magic=KHEAP_MAGIC;
			footer->header=header;
/*			kprintf("FREEEEEEEE! header->magic=%p\n",header->magic);
			kprintf("FREEEEEEEE! header->size=%u\n",header->size);
			kprintf("FREEEEEEEE! header=%p\n",header);*/
			index->insert( header );
		}else{
			// The last header needs adjusting.
			header_t *header=(*index)[idx];
			header->size+=new_length-old_length;
			// Rewrite the footer.
			footer_t *footer=(footer_t*)( (uint32_t)header+header->size-sizeof(footer_t) );
			footer->header=header;
			footer->magic=KHEAP_MAGIC;
		}
		// We now have enough space. Recurse, and call the function again.
		return alloc(size, page_align);
	}
	header_t *orig_hole_header=(*index)[iterator];
	uint32_t orig_hole_pos=(uint32_t)orig_hole_header;
	uint32_t orig_hole_size=orig_hole_header->size;
	// Here we work out if we should split the hole we found into two parts.
	// Is the original hole size - requested hole size less than the overhead for adding a new hole?
	if( orig_hole_size-new_size<sizeof(header_t)+sizeof(footer_t) ){
		// Then just increase the requested size to the size of the hole we found.
		size += orig_hole_size-new_size;
		new_size = orig_hole_size;
	}
	// If we need to page-align the data, do it now and make a new hole in front of our block.
	if( page_align && orig_hole_pos&0xFFFFF000 ){
		uint32_t new_location = orig_hole_pos+0x1000 /* page size */ - (orig_hole_pos&0xFFF) - sizeof(header_t);
		header_t *hole_header = (header_t *)orig_hole_pos;
		hole_header->size     = 0x1000 /* page size */ - (orig_hole_pos&0xFFF) - sizeof(header_t);
		hole_header->magic    = KHEAP_MAGIC;
		hole_header->is_hole  = 1;
		footer_t *hole_footer = (footer_t *) ( (uint32_t)new_location - sizeof(footer_t) );
		hole_footer->magic    = KHEAP_MAGIC;
		hole_footer->header   = hole_header;
		orig_hole_pos         = new_location;
		orig_hole_size        = orig_hole_size - hole_header->size;
	}else{
		// Else we don't need this hole any more, delete it from the index.
		index->remove(iterator);
	}
	// Overwrite the original header...
	header_t *block_header  = (header_t*)orig_hole_pos;
	block_header->magic     = KHEAP_MAGIC;
	block_header->is_hole   = 0;
	block_header->size      = new_size;
	// ...And the footer
	footer_t *block_footer  = (footer_t *) (orig_hole_pos + sizeof(header_t) + size);
	block_footer->magic     = KHEAP_MAGIC;
	block_footer->header    = block_header;


	// We may need to write a new hole after the allocated block.
	// We do this only if the new hole would have positive size...
	if( orig_hole_size-new_size>0 ){
		header_t *hole_header = (header_t *) (orig_hole_pos + sizeof(header_t) + size + sizeof(footer_t));
		hole_header->magic    = KHEAP_MAGIC;
		hole_header->is_hole  = 1;
		hole_header->size     = orig_hole_size - new_size;
		footer_t *hole_footer = (footer_t *) ( (uint32_t)hole_header + orig_hole_size - new_size - sizeof(footer_t) );
		if( (uint32_t)hole_footer<end_address() ){
			hole_footer->magic = KHEAP_MAGIC;
			hole_footer->header = hole_header;
		}
		// Put the new hole in the index;
/*		kprintf("FREEEEEEEE! header->magic=%p\n",hole_header->magic);
		kprintf("FREEEEEEEE! header->size=%u\n",hole_header->size);
		kprintf("FREEEEEEEE! header=%p\n",hole_header);*/
		index->insert(hole_header);
	}

	// ...And we're done!
	return (void*)( (uint32_t)block_header+sizeof(header_t) );
}

void KHeap::free(void *p){
	// Exit gracefully for null pointers.
	if( p==0 )
		return;

	// Get the header and footer associated with this pointer.
	header_t *header=(header_t*)( (uint32_t)(p)-sizeof(header_t));
	footer_t *footer=(footer_t*)( (uint32_t)(header)+header->size-sizeof(footer_t) );
//	header_t *header = (header_t*) ( (u32int)p - sizeof(header_t) );
//	footer_t *footer = (footer_t*) ( (u32int)header + header->size - sizeof(footer_t) );

	// Sanity checks.
	kdebug(8,"FREEEEEEEE! p=%p\n",p);
	kdebug(8,"FREEEEEEEE! header=%p\n",header);
	kdebug(8,"FREEEEEEEE! header->magic=%p\n",header->magic);
	kdebug(8,"FREEEEEEEE! header->size=%u\n",header->size);
	kdebug(8,"FREEEEEEEE! footer=%p\n",footer);
	kdebug(8,"FREEEEEEEE! footer->magic=%p\n",footer->magic);
	kdebug(8,"KHEAP_MAGIC=%p h=%p f=%p\n\n",KHEAP_MAGIC,header,footer);
/*
	*/

//	ASSERT(header->magic==KHEAP_MAGIC);
//	ASSERT(footer->magic==KHEAP_MAGIC);

	kdebug(8,"FREEEEEEEE! header->magic=%p KHEAP_MAGIC=%p h=%p f=%p\n\n",header->magic,KHEAP_MAGIC,header,footer);

	// Make us a hole.
	header->is_hole=true;

	// Do we want to add this header into the 'free holes' index?
	bool do_add=true;

	// Unify left
	// If the thing immediately to the left of us is a footer...
	footer_t *test_footer = (footer_t*) ( (uint32_t)header - sizeof(footer_t) );
	kdebug(8,"FREEEEEEEE! footer->magic=%p\n",test_footer->magic);
	if( test_footer->magic == KHEAP_MAGIC &&
		test_footer->header->is_hole )
	{
		uint32_t cache_size=header->size; // Cache our current size.
		header=test_footer->header;     // Rewrite our header with the new one.
		footer->header = header;          // Rewrite our footer to point to the new header.
		header->size += cache_size;       // Change the size.
		do_add=false;                       // Since this header is already in the index, we don't want to add it again.
	}

	// Unify right
	// If the thing immediately to the right of us is a header...
	header_t *test_header = (header_t*) ( (uint32_t)footer + sizeof(footer_t) );
	if( test_header->magic == KHEAP_MAGIC &&
		test_header->is_hole )
	{
		kdebug(8,"FREEEEEEEE! test_header->magic=%p\n",test_header->magic);
		header->size += test_header->size; // Increase our size.
		test_footer = (footer_t*) ( (uint32_t)test_header + // Rewrite it's footer to point to our header.
			test_header->size - sizeof(footer_t) );
		footer=test_footer;
		// Find and remove this header from the index.
		uint32_t iterator=0;
//		kprintf("Kheap_free! %p\n",p);
		while ( (iterator<index->size() ) && (*index)[iterator]!=test_header )
			iterator++;

		// Make sure we actually found the item.
		ASSERT( iterator<index->size());
		// Remove it.
		index->remove(iterator);
	}

	if (do_add==true)
		index->insert(header); 
}

