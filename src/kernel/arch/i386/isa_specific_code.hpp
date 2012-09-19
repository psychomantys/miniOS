/*
 * =====================================================================================
 *
 *       Filename:  isa_specific_code.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/18/2012 02:31:18 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef	_ISA_SPECIFIC_CODE__INC
#define	_ISA_SPECIFIC_CODE__INC

// Some nice typedefs, to standardise sizes across platforms.
// These typedefs are written for 32-bit X86.
typedef	unsigned	char	u8int;
typedef	unsigned	short	u16int;
typedef	unsigned	int	u32int;

typedef	char	s8int;
typedef	short	s16int;
typedef	int	s32int;

//#include	<cstdint>

typedef	u32int	dword_t;
typedef	u16int	word_t;

#endif	/* ----- #ifndef _ISA_SPECIFIC_CODE__INC  ----- */

