#ifndef  kernel_utils_INC
#define  kernel_utils_INC

#include	<assert.h>
#ifdef NDEBUG
	#define	ASSERT(x)
#else
	#define	ASSERT(x) assert(x)
//	#define	ASSERT(x) if( (x)==false ) PANIC(#x)
#endif

#define	PANIC(x) kprintf("Kernel Panicooo: %s (Line %d, File %s)\n",x,__LINE__,__FILE__); halt_machine();

#define declare_am(type, name) \
private: type _##name; \
public: \
void name(const type& var){\
	_##name = var;\
}\
\
const type & name(){\
	return _##name; \
}\

#define declare_a(type, name) \
private: type _##name; \
void name(const type& var){\
	_##name = var;\
}\
\
public: \
const type & name(){\
	return _##name; \
}\

#endif	/* ----- #ifndef kernel_utils_INC  ----- */

