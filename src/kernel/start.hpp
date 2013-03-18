
#ifndef	_CPP_CTOR_DTOR_IMPL__INC
#define	_CPP_CTOR_DTOR_IMPL__INC

#include	<stdint.h>

extern uint32_t initial_esp;

extern "C" void _at_global_begin();

extern "C" void _at_global_end();

extern "C" void start();

#endif	/* ----- #ifndef _CPP_CTOR_DTOR_IMPL__INC  ----- */

