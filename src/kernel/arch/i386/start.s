/*
.globl code
.globl bss
.globl end
.globl rodata
*/
.set MBOOT_PAGE_ALIGN, 1<<0
.set MBOOT_MEM_INFO, 1<<1
.set MBOOT_HEADER_MAGIC, 0x1BADB002
.set MBOOT_HEADER_FLAGS, MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
.set MBOOT_CHECKSUM, -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

.globl mboot
.globl start

mboot:
	.long	MBOOT_HEADER_MAGIC
	.long	MBOOT_HEADER_FLAGS
	.long	MBOOT_CHECKSUM

	.long	mboot
	.long	code
	.long	bss
	.long	end
	.long	rodata
	.long	start

multiboot_magic:
	.globl	multiboot_magic
	.zero	4

multiboot_addr:
	.globl multiboot_addr
	.zero	4

start:

	movl  %eax, multiboot_magic
	movl  %ebx, multiboot_addr


	call disable_interrupts

	call _at_global_begin
	call main
	call _at_global_end

	call halt_machine

