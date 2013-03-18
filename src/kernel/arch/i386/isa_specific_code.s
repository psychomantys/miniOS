
.globl GDT_flush
GDT_flush:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %eax
	movl	%eax, %edx

	lgdt (%edx)

	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	ljmp  $0x8,$gdt_jmp_flush
gdt_jmp_flush:
	
	popl	%ebp
	ret





.globl IDT_load
IDT_load:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %eax
	movl	%eax, %edx

	lidt (%edx)

	popl	%ebp
	ret







.globl halt_machine
halt_machine:
	pushl	%ebp
	movl	%esp, %ebp


	.hang:
	hlt
	jmp  .hang


	popl	%ebp
	ret



.globl disable_interrupts
disable_interrupts:
	pushl	%ebp
	movl	%esp, %ebp

	cli

	popl	%ebp
	ret


.globl enable_interrupts
enable_interrupts:
	pushl	%ebp
	movl	%esp, %ebp

	sti

	popl	%ebp
	ret





.globl outb
outb:
	pushl	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %eax
	movw	(%eax), %dx
	movl	12(%ebp), %eax
	movb	(%eax), %al

	outb %al, %dx

	popl	%ebp
	ret




.globl inb
inb:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$20, %esp
	movl	8(%ebp), %eax
	movw	(%eax), %ax
	movw	%ax, -22(%ebp)
	movw	-22(%ebp), %dx

	inb %dx, %al

	addl	$20, %esp
	popl	%ebx
	popl	%ebp
	ret


.globl inw
inw:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$20, %esp
	movl	8(%ebp), %eax
	movw	(%eax), %ax
	movw	%ax, -22(%ebp)
	movw	-22(%ebp), %dx

	inw %dx, %ax

	addl	$20, %esp
	popl	%ebx
	popl	%ebp
	ret

