/*
 * =====================================================================================
 *
 *       Filename:  common.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/13/2012 08:46:31 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */


#include "common.h"

// Write a byte out to the specified port.
void outb(u16int port, u8int value){
	asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

u8int inb(u16int port){
	u8int ret;
	asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

u16int inw(u16int port){
	u16int ret;
	asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}



