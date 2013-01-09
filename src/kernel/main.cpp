#include	<kernel/monitor.hpp>
#include	<kernel/gdt.hpp>
#include	<kernel/idt.hpp>
#include	<kernel/irq.hpp>
#include	<kernel/timer.hpp>
#include	<kernel/keyboard.hpp>
#include	<string.h>
#include	<stdlib.h>

//VGA tela;

class t{
	public:
		const char *id;

		t( const char *id ) : id(id) {
			kprintf("CTOR t ");
			kprintf(id);
			kprintf("\n");
		}

		void print(){
			char n[50]="print method t->print() (id)==> ";
			kprintf(strcat(n,id));
			kprintf("\n");
		}

		~t(){
			kprintf("DTOR t ");
			kprintf(id);
			kprintf("\n");
		}
};

// Não funciona com o IDT
// acho que tem que mudar as areas de memoria do kernel
t x1("1");
t x2("2");
t x3("3");

GDT gdt;
IDT idt;
IRQ irq;
Timer pit;
Keyboard kb;

//int main(struct multiboot *mboot_ptr){
int main(){
	//stl::string s("OPAAA GANGNAM STYLE!!")

	gdt.install();
	idt.install();

	irq.install( idt );
	pit.install( irq );
	kb.install( irq );

	enable_interrupts();
//	int x=0/0;
	t x4("4");
	t x5("5");
	kprintf("Psycho Mantys\n");
	pit.wait(200);
	kprintf("22222\n");
	//int *x=(int*)malloc(10);
	x1.print();
	x4.print();

	return 0x00000042;
}

