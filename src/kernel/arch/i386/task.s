
.global read_eip
read_eip:
	mov (%esp),%eax
	ret

/*
	pop %eax
	jmp *%eax
*/


.global copy_page_physical
copy_page_physical: 
	pushl %ebx            # According to __cdecl, we must preserve the contents of EBX.
	pushf                 # push EFLAGS, so we can pop it and reenable interrupts
# later, if they were enabled anyway.
	cli                   # Disable interrupts, so we aren't interrupted.
# Load these in BEFORE we disable paging!
	movl 12(%esp),%ebx    # Source address
	movl 16(%esp),%ecx    # Destination address

	movl %cr0, %edx       # Get the control register...
	andl $0x7fffffff,%edx # and...
	movl %edx, %cr0       # Disable paging.

	movl $1024,%edx       # 1024*4bytes = 4096 bytes

copy_page_physical.loop: 
	movl (%ebx),%eax      # Get the word at the source address
	movl %eax,(%ecx)      # Store it at the dest address
	addl $4,%ebx          # Source address += sizeof(word)
	addl $4,%ecx          # Dest address += sizeof(word)
	decl %edx             # One less word to do
	jnz copy_page_physical.loop

	movl %cr0, %edx       # Get the control register again
	orl $0x80000000,%edx  # and...
	movl %edx, %cr0       # Enable paging.

	popf                  # Pop EFLAGS back.
	popl %ebx             # Get the original value of EBX back.
	ret

