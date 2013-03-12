/*
 * =====================================================================================
 *
 *       Filename:  new.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/09/2013 11:06:11 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include	<stdlib.h>
#include	<kernel/debug.hpp>
#include	<kernel/kmalloc.hpp>

void *operator new[](unsigned int s){
	void *p=(void*)(kmalloc(s));
	if( ! p ){
		kdebug(2,"Bad alloc %d\n",s);
	}	
//	kprintf("p=%p\n",p);
	return p;
}

void *operator new(unsigned int s){
	void *p=(void*)(kmalloc(s));
	if( ! p ){
		kdebug(2,"Bad alloc %d\n",s);
	}	
	return p;
}

void operator delete[](void* f){
	kdebug(9,"void operator delete[](void* f) f=%p\n",f);
	kfree((uint32_t)(f));
	kdebug(9,"end void operator delete[](void* f) f=%p\n",f);
}

void operator delete(void* f){
	kfree((uint32_t)(f));
}

