/*
 * =====================================================================================
 *
 *       Filename:  plataform_specific_code.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/18/2012 02:29:46 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef	_PLATAFORM_SPECIFIC_CODE__INC
#define	_PLATAFORM_SPECIFIC_CODE__INC

/* This defines what the stack looks like after an ISR was running */
struct regs{
	unsigned int gs, fs, es, ds;      /* pushed the segs last */
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
	unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
	unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};

#endif	/* ----- #ifndef _PLATAFORM_SPECIFIC_CODE__INC  ----- */

