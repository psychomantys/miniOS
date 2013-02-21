#include	<kernel/monitor.hpp>
#include	<kernel/gdt.hpp>
#include	<kernel/idt.hpp>
#include	<kernel/irq.hpp>
#include	<kernel/paging.hpp>
#include	<kernel/timer.hpp>
#include	<kernel/keyboard.hpp>
#include	<kernel/kpp/queue.hpp>
#include	<kernel/kpp/bitset.hpp>
#include	<kernel/kpp/ordered_array.hpp>
#include	<kernel/boot/multiboot.hpp>

#include	<string.h>

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

t x1(1);

GDT gdt;
IDT idt;
IRQ irq( idt );
Timer pit( irq );
Keyboard kb( pit, irq );
Paging paging(0x2000000, idt);


//static uint32_t bss;
int main(){

	if( multiboot_magic!=MULTIBOOT_BOOTLOADER_MAGIC ){
		kprintf("multiboot Magic number wrong! (%p)\n", multiboot_addr);
		halt_machine();
	}

	gdt.install();
	idt.install();

	irq.install();
	pit.install();
	kb.install();

//	paging.install();

	enable_interrupts();

	uint32_t *ptr = (uint32_t*)0xA0000000;
	uint32_t do_page_fault = *ptr;
/*
*/

//	int div_error=0/0;
	t x4(4);
	t x5(5);
	kprintf("Psycho Mantys\n");
	pit.wait(20);
	char *x=new char[10];
	kprintf("x=%p\n",x);
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

	Ordered_array<int> ar(10);
	for( int i=0 ; i<10 ; ++i ){
		ar.insert(i);
	}
	for( int i=0 ; i<10 ; ++i ){
		kprintf("%d ",ar[i]);
	}
	kprintf("   size()=%d\n",ar.size());

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

