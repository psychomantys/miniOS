
#include	<kernel/start.hpp>
#include	<kernel/boot/multiboot.hpp>

#ifdef	MINIOS_IMPL_C_MULTIBOOT

/*
 * Don't work :/
 *
 * Maybe next generation can make this piece of code work...
 */

#define	MULTIBOOT_HEADER_FLAGS	( MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO )
#define	MULTIBOOT_CHECKSUM	- ( MULTIBOOT_HEADER_MAGIC+MULTIBOOT_HEADER_FLAGS )
extern volatile uint32_t	code;
extern volatile uint32_t	bss;
extern volatile uint32_t	end;
extern volatile uint32_t	rodata;

const struct multiboot_header kernel_header = {
	MULTIBOOT_HEADER_MAGIC,
	MULTIBOOT_HEADER_FLAGS,
	MULTIBOOT_CHECKSUM,
	&kernel_header,
	code,
	bss,
	end,
	rodata,
	start
};

#else
/*
asm(".globl code\n\t"
".globl bss\n\t"
".globl end\n\t"
".globl rodata\n\t"

".set MBOOT_PAGE_ALIGN, 1<<0\n\t"
".set MBOOT_MEM_INFO, 1<<1\n\t"
".set MBOOT_HEADER_MAGIC, 0x1BADB002\n\t"
".set MBOOT_HEADER_FLAGS, MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO\n\t"
".set MBOOT_CHECKSUM, -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)\n"

"mboot:\n\t"
	".long	MBOOT_HEADER_MAGIC\n\t"
	".long	MBOOT_HEADER_FLAGS\n\t"
	".long	MBOOT_CHECKSUM\n\t"

	".long	mboot\n\t"
	".long	code\n\t"
	".long	bss\n\t"
	".long	end\n\t"
	".long	rodata\n\t"
	".long	start"); */
#endif

#include	<compiler_specific_code.hpp>
#include	<isa_specific_code.hpp>
#include	<kernel/debug.hpp>
#include	<kernel/kmalloc.hpp>

extern void (main)();

/*
volatile uint32_t multiboot_magic;
volatile uint32_t multiboot_addr;

extern "C" void start(){
//	get_register_eax(multiboot_magic);
//	get_register_ebx(multiboot_addr);
//	http://www.gnu.org/software/grub/manual/multiboot/multiboot.html#kernel_002ec

//	volatile struct multiboot_header *mboot_ptr;
//	uint32_t xx;
	asm volatile ("movl %%eax, %0\n\t" : "=r"(multiboot_magic) );
	asm volatile ("movl %%ebx, %0\n\t" : "=r"(multiboot_addr) );
	disable_interrupts();


	kprintf("multiboot_magic___=%d  \n", multiboot_magic );
	kprintf("multiboot_addr____=%d  \n", multiboot_addr );
//	for(;;);
//	kprintf("mboot_ptr=%d %d asasas",10,10);
//	kprintf("\n");
//	kprintf("mboot->bss_end_addr=%p\n",mboot_ptr->bss_end_addr);
//	kprintf("\n");
//	kprintf("mboot->load_end_addr=%p\n",mboot_ptr->load_end_addr);
//	kprintf("\n");


	_at_global_begin();
	main( multiboot_magic, multiboot_addr );
	_at_global_end();

	halt_machine();
}
*/


extern "C" {
	void _at_global_begin(){
		end_malloc_addr=multiboot_addr->bss_end_addr;
		__cxa_atbegin();
	}
	void _at_global_end(){
		kdebug(3,"begin end!\n");
		__cxa_finalize();
		kdebug(3,"end end!\n");
	}
}

