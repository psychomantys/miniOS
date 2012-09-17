// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
// Made for JamesM's tutorials

#include	"monitor.h"
/*
struct t{
	t(){
		monitor_write("CTOR t\n");
	}

	void print(){
		monitor_write("print method t\n");
	}
};

t x;
*/
int main(struct multiboot *mboot_ptr){
//	monitor_clear();
	monitor_write("Psycho Mantys");
//	x.print();
	return 0x00000042;
}

