#include	<kernel/monitor.hpp>
#include	<kernel/gdt.hpp>
#include	<string.h>
#include	<stdlib.h>

VGA tela;

struct t{
	const char *id;

	t( const char *id ) : id(id) {
		tela.write("CTOR t ");
		tela.write(id);
		tela.write("\n");
	}
	void print(){
		char n[40]="print method t->print() (id)==> ";
		tela.write(strcat(n,id));
		tela.write("\n");
	}

	~t(){
		tela.write("DTOR t ");
		tela.write(id);
		tela.write("\n");
	}
};

t x1("1");
t x2("2");
t x3("3");

//int main(struct multiboot *mboot_ptr){
int main(){
//	stl::string s("OPAAA GANGNAM STYLE!!")
	GDT gdt;
	gdt.install();

	tela.clear();
	t x4("4");
	tela.settextcolor(VGA::light_green, VGA::black);
	t x5("5");
	tela.write("Psycho Mantys\n");
	x1.print();
	x4.print();

	return 0x00000042;
}

