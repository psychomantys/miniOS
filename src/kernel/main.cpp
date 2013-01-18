#include	<kernel/monitor.hpp>
#include	<kernel/gdt.hpp>
#include	<kernel/idt.hpp>
#include	<kernel/irq.hpp>
#include	<kernel/timer.hpp>
#include	<kernel/keyboard.hpp>
#include	<kernel/kpp/queue.hpp>
#include	<string.h>
#include	<stdlib.h>

class t{
	public:
		const char *id;

		t( const char *id="10000" ) : id(id) {
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
IRQ irq( idt );
Timer pit( irq );
Keyboard kb( pit, irq );

//int main(struct multiboot *mboot_ptr){
int main(){
	//stl::string s("OPAAA GANGNAM STYLE!!")

	gdt.install();
	idt.install();

	irq.install();
	pit.install();
	kb.install();

	enable_interrupts();
//	int div_error=0/0;
	t x4("4");
	t x5("5");
	kprintf("Psycho Mantys\n");
	pit.wait(20);
	kprintf("22222\n");
//	char *x=(char*)malloc(10);
	char *x=new char[10];
//	DQueue<char> s;
	SQueue<char> s(10);

	s.push('4');
	s.push('2');
	s.push('!');
	s.push('\n');
	s.push('\0');

	for( int i=0 ; not s.is_empty() ; ++i ){
		x[i]=s.pop() ;
	}

	kprintf(x);
	pit.wait(20);

	x[0]=kb.getch();
	kprintf(x);

	t *a=new t[2];

	delete []x;
	delete []a;

	x1.print();
	x4.print();

	return 0x00000042;
}

