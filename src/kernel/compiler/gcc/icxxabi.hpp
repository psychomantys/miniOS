/*
 * =====================================================================================
 *
 *       Filename:  icxxabi.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/18/2012 02:26:06 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef	_ICXXABI_HPP__INC
#define	_ICXXABI_HPP__INC

#define	ATEXIT_MAX_FUNCS	128

#ifdef	__cplusplus
extern "C" {
#endif

void __cxa_atbegin();

int __cxa_atexit(void (*function)(void *), void *objptr, void *dso);

void __cxa_finalize(void *function='\0');

#ifdef	__cplusplus
};
#endif

#endif   /* ----- #ifndef _ICXXABI_HPP__INC  ----- */

