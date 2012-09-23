#include	<monitor.h>
#include	<string.h>
#include	<stdlib.h>

struct t{
	const char *id;

	t( const char *id ) : id(id) {
		monitor_write("CTOR t ");
		monitor_write(id);
		monitor_write("\n");
	}
	void print(){
		char n[40]="print method t->print() (id)==> ";
		monitor_write(strcat(n,id));
		monitor_write("\n");
	}

	~t(){
		monitor_write("DTOR t ");
		monitor_write(id);
		monitor_write("\n");
	}
};

t x1("1");
t x2("2");
t x3("3");

int main(struct multiboot *mboot_ptr){
//	monitor_clear();
	t x4("4");
	monitor_write("Psycho Mantys\n");
	x1.print();
	x4.print();
	return 0x00000042;
}

