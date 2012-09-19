/*
 * =====================================================================================
 *
 *       Filename:  common.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/13/2012 08:46:03 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef COMMON_H
#define COMMON_H

#include	<isa_specific_code.hpp>

void outb(u16int port, u8int value);
u8int inb(u16int port);
u16int inw(u16int port);

#endif



