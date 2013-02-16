#ifndef  kernel_utils_INC
#define  kernel_utils_INC

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

#endif	/* ----- #ifndef kernel_utils_INC  ----- */

