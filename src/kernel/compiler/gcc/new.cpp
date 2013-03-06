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
#include	<kernel/monitor.hpp>
#include	<kernel/kmalloc.hpp>

void *operator new[](unsigned int s){
	void *p=(void*)(kmalloc(s));
	if( ! p ){
//		kprintf("Bad alloc %d\n",s);
	}	
//	kprintf("p=%p\n",p);
	return p;
}

void *operator new(unsigned int s){
	void *p=(void*)(kmalloc(s));
	if( ! p ){
//		kprintf("Bad alloc %d\n",s);
	}	
	return p;
}

void operator delete[](void* f){
//	kfree(f);
}

void operator delete(void* f){
//	kfree(f);
}

