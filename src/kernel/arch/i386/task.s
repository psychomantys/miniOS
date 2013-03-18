
.global read_eip
read_eip:
	pop %eax
	jmp *%eax

/*
.globl copy_page_physical
copy_page_physical:
	pushl	%ebp          ; According to __cdecl, we must preserve the contents of EBX.
	pushf                 ; push EFLAGS, so we can pop it and reenable interrupts
                              ; later, if they were enabled anyway.
	cli                   ; Disable interrupts, so we aren't interrupted.
                              ; Load these in BEFORE we disable paging!
	movl 12(%esp), %ebx   ; Source address
	movl 16(%esp), %ecx   ; Destination address

	movl %cr0, %edx       ; Get the control register...
	andl $0x7fffffff, %edx  ; and...
	mov %edx, %cr0        ; Disable paging.

	mov $1024, %edx       ; 1024*4bytes = 4096 bytes to copy

.loop:
	mov (%ebx), %eax      ; Get the word at the source address
	mov %eax, (%ecx)      ; Store it at the dest address
	add $4, %ebx          ; Source address += sizeof(word)
	add $4, %ecx          ; Dest address += sizeof(word)
	dec %edx              ; One less word to do
	jnz .loop

	mov %cr0, %edx        ; Get the control register again
	or $0x80000000, %edx  ; and...
	mov %edx, %cr0        ; Enable paging.

	popf                  ; Pop EFLAGS back.
	pop %ebx              ; Get the original value of EBX back.
ret

.globl copy_page_physical
copy_page_physical:
	push ebx              ; According to __cdecl, we must preserve the contents of EBX.
	pushf                 ; push EFLAGS, so we can pop it and reenable interrupts
                         ; later, if they were enabled anyway.
	cli                   ; Disable interrupts, so we aren't interrupted.
                         ; Load these in BEFORE we disable paging!
	mov ebx, [esp+12]     ; Source address
	mov ecx, [esp+16]     ; Destination address

	mov edx, cr0          ; Get the control register...
	and edx, 0x7fffffff   ; and...
	mov cr0, edx          ; Disable paging.

	mov edx, 1024         ; 1024*4bytes = 4096 bytes to copy

.loop:
	mov eax, [ebx]        ; Get the word at the source address
	mov [ecx], eax        ; Store it at the dest address
	add ebx, 4            ; Source address += sizeof(word)
	add ecx, 4            ; Dest address += sizeof(word)
	dec edx               ; One less word to do
	jnz .loop

	mov edx, cr0          ; Get the control register again
	or  edx, 0x80000000   ; and...
	mov cr0, edx          ; Enable paging.

	popf                  ; Pop EFLAGS back.
	pop ebx               ; Get the original value of EBX back.
ret
*/

