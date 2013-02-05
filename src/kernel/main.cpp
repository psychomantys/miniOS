#include	<kernel/monitor.hpp>
#include	<kernel/gdt.hpp>
#include	<kernel/idt.hpp>
#include	<kernel/irq.hpp>
#include	<kernel/timer.hpp>
#include	<kernel/keyboard.hpp>
#include	<kernel/kpp/queue.hpp>
#include	<kernel/boot/multiboot.hpp>

#include	<string.h>
#include	<stdlib.h>

class t{
	public:
		const int id;

		t( const int id=10000 ) : id(id) {
			kprintf("CTOR t %d\n",id);
		}

		void print(){
			char n[40]="print method t->print() (id)==> ";
			kprintf("%s %d\n",n,id);
		}

		~t(){
			kprintf("DTOR t %d\n",id);
		}
};

// Não funciona com o IDT
// acho que tem que mudar as areas de memoria do kernel
t x1(1);
t x2(2);
t x3(3);

GDT gdt;
IDT idt;
IRQ irq( idt );
Timer pit( irq );
Keyboard kb( pit, irq );


//static uint32_t bss;
int main(){

	kprintf("multiboot_addr=%p \n", multiboot_addr);

	kprintf("multiboot_magic=%p\n",multiboot_magic);
	kprintf("multiboot_addr->bss_end_addr=%p\n",multiboot_addr->bss_end_addr);
	kprintf("multiboot_addr=%p\n",multiboot_addr);
	gdt.install();
	idt.install();

	irq.install();
	pit.install();
	kb.install();

	enable_interrupts();

//	int div_error=0/0;
	t x4(4);
	t x5(5);
////	kprintf("Psycho Mantys\n");
//	pit.wait(20);
////	kprintf("22222\n");
//	char *x=(char*)malloc(10);
	char *x;
	kprintf("x=%p    \n",x);
	x=new char[10];
	kprintf("x=%p    \n",x);
//	DQueue<char> s;
	x[0]='A';
	x[1]='A';
	x[2]='A';
	x[3]='A';
	x[4]='\0';
	kprintf("%s\n",x);
	SQueue<char> s(10);

	s.push('4');
	s.push('2');
	s.push('!');
	s.push('\n');
	s.push('\0');

	x1.print();
	x4.print();
	for( int i=0 ; not s.is_empty() ; ++i ){
		x[i]=s.pop() ;
	}

	kprintf(x);
	pit.wait(20);

	x[0]=kb.getch();
	kprintf(x);

	x1.print();
	x4.print();

	t *a=new t[2];
	
	x1.print();
	x4.print();

	delete []x;
	delete []a;

	x1.print();
	x4.print();

	return 0x00000042;
}

