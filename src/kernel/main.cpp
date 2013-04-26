#include	<kernel/monitor.hpp>
#include	<kernel/start.hpp>
#include	<kernel/gdt.hpp>
#include	<kernel/idt.hpp>
#include	<kernel/irq.hpp>
#include	<kernel/paging.hpp>
#include	<kernel/kheap.hpp>
#include	<kernel/timer.hpp>
#include	<kernel/keyboard.hpp>
#include	<kernel/kpp/queue.hpp>
#include	<kernel/kpp/bitset.hpp>
#include	<kernel/kpp/ordered_array.hpp>
#include	<kernel/boot/multiboot.hpp>
#include	<kernel/fs/initrd.hpp>
#include	<kernel/task.hpp>

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

KMalloc auxxx( end_malloc_addr() );

t x1(1);

int main(){
	GDT gdt;
	IDT idt;

	gdt.install();
	idt.install();

	IRQ irq( idt );
	irq.install();

	Timer pit( irq );
	Keyboard kb( pit, irq );
	pit.install();
	kb.install();

	char *a1 = new char[8];

	Paging paging(0x1000000, idt, KHEAP_START, KHEAP_INITIAL_SIZE);
	paging.install();

	KHeap kheap(paging);
	kheap.install(KHEAP_START, KHEAP_START+KHEAP_INITIAL_SIZE, 0xCFFFF000, false, false);

	Multitask multitask(paging);
	multitask.install();

	if( multiboot_magic!=MULTIBOOT_BOOTLOADER_MAGIC ){
		kprintf("multiboot Magic number wrong! (%p)\n", multiboot_addr);
		halt_machine();
	}


	enable_interrupts();
//	kb.getch();

/*	
	uint32_t *ptr = (uint32_t*)0xA0000000;
	uint32_t *ptr = (uint32_t*)0x22672c;
	uint32_t do_page_fault = *ptr;
*/

	char *b = new char[8];
	char *c = new char[8];

	kprintf("a: %p\nb: %p\nc: %p\ninitial_esp: %p\n",a1,b,c,initial_esp);

	delete a1;
	delete b;
	delete c;


	uint32_t initrd_location=*((uint32_t*)(multiboot_addr->mods_addr));

	kprintf("initrd_begin=%p mb_addr=%p\n",initrd_location,multiboot_addr);

//	int div_error=0/0;
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

//	x1.print();
//	x1.print();
	for( int i=0 ; not s.is_empty() ; ++i ){
		x[i]=s.pop() ;
	}
/*
*/
	Ordered_array<int> ar(10);
	for( int i=0 ; i<10 ; ++i ){
		ar.insert(i);
	}
	for( int i=0 ; i<10 ; ++i ){
//		t taux(i);
		kprintf("%d ",ar[i]);
	}
	kprintf("   size()=%d\n",ar.size());

	kprintf(x);
	pit.wait(20);

	x[0]=kb.getch();
	kprintf(x);

	t x2(2);
	t x3(3);
//	x1.print();
	x3.print();

	t *a=new t[2];
	int *p_t2=new int[40];
	
//	x1.print();
	x3.print();

	kprintf("Vai um delete?\n");

	kprintf("%p 1\n",x);
	delete []x;
	kprintf("%p p_t2\n",p_t2);
	delete []p_t2;
	kprintf("%p 3\n",a);
	delete []a;

	kprintf("Foi passou delete!!\n");

//	x1.print();
	x2.print();

	t tfim(15);
	tfim.print();
	
	kprintf("Vai fork?!!!!\n");
	int ret = multitask.fork();

	disable_interrupts();
	kprintf("Fork?!!!! %d\n",ret);

	if( multiboot_addr->mods_count>0 ){
		kprintf("Have initrd!!\n");
		fs_root=initialise_initrd(initrd_location);
//		uint32_t initrd_location=*((uint32_t*)(multiboot_addr->mods_addr));
//		uint32_t initrd_end=*((uint32_t*)(multiboot_addr->mods_addr+4));


		// list the contents of /
		int i = 0;
		struct dirent *node = 0;
		while ( (node = readdir_fs(fs_root, i))!=0) {
			kprintf("Found file %s",node->name);
			fs_node_t *fsnode = finddir_fs(fs_root, node->name);

			if( (fsnode->flags&0x7)==FS_DIRECTORY ){
				kprintf("\n\t(directory)\n");
			}else{
				kprintf("\n\t contents: \"");
				char buf[256];
				uint32_t sz=read_fs(fsnode, 0, 256, (uint8_t*)(buf));
				uint32_t j;
				for( j=0 ; j<sz ; j++){
					kprintf("%c",buf[j]);
				}
				kprintf("\"\n");
			}
			++i;
		} 

	}else{
		kprintf("No initrd.\n");
	}

	kprintf("Final do main kernel!!!!!\n");
	enable_interrupts();
	return 0x00000042;
}

