
#include <common.h>
#include	"monitor.h"

// TODO:
// Pouco portavel. Mudar u32int para um typedef do tamanho da palavra da plataforma

typedef void (*function_pointer)(void);

extern "C" void _atbegin(){
	extern function_pointer start_ctors[];
	extern function_pointer end_ctors[];

	monitor_clear();
	monitor_write("_atbegin()\n");
	u32int qtd_ctors=(end_ctors - start_ctors);

//	for(u32int i=0 ; i<qtd_ctors ; ++i){
//		function_pointer ctor=start_ctors[i];
//		ctor();
//	}
}

extern "C" void _atexit(){
	extern function_pointer start_dtors[];
	extern function_pointer end_dtors[];

	u32int qtd_dtors=(end_dtors - start_dtors);

//	for(u32int i=qtd_dtors-1 ; i>=qtd_dtors ; ++i){
//		function_pointer dtor=start_dtors[i];
//		dtor();
//	}
}

int __cxa_atexit( void (*function)(void *), void *arg, void *dso_handle ){
		  (*function)((void*)0);
}


