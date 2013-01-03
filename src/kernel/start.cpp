
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
	".long	start");
#endif

#include	<compiler_specific_code.hpp>
#include	<isa_specific_code.hpp>

extern void (main)();

extern "C" void start(){
//	http://www.gnu.org/software/grub/manual/multiboot/multiboot.html#kernel_002ec
	disable_interrupts();

	_at_global_begin();
	main();
	_at_global_end();

	halt_machine();
}

extern "C" {
	void _at_global_begin(){
		__cxa_atbegin();
	}
	void _at_global_end(){
		__cxa_finalize();
	}
}

