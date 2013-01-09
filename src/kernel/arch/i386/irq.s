
.global irq0
.global irq1
.global irq2
.global irq3
.global irq4
.global irq5
.global irq6
.global irq7
.global irq8
.global irq9
.global irq10
.global irq11
.global irq12
.global irq13
.global irq14
.global irq15

irq0:
	cli
	push $0
	push $32
	jmp irq_common_stub

irq1:
	cli
	push $0
	push $33
	jmp irq_common_stub

irq2:
	cli
	push $0
	push $34
	jmp irq_common_stub

irq3:
	cli
	push $0
	push $35
	jmp irq_common_stub

irq4:
	cli
	push $0
	push $36
	jmp irq_common_stub

irq5:
	cli
	push $0
	push $37
	jmp irq_common_stub

irq6:
	cli
	push $0
	push $38
	jmp irq_common_stub


irq7:
	cli
	push $0
	push $39
	jmp irq_common_stub


irq8:
	cli
	push $0
	push $40
	jmp irq_common_stub


irq9:
	cli
	push $0
	push $41
	jmp irq_common_stub


irq10:
	cli
	push $0
	push $42
	jmp irq_common_stub


irq11:
	cli
	push $0
	push $43
	jmp irq_common_stub


irq12:
	cli
	push $0
	push $44
	jmp irq_common_stub

irq13:
	cli
	push $0
	push $45
	jmp irq_common_stub


irq14:
	cli
	push $0
	push $46
	jmp irq_common_stub

irq15:
	cli
	push $0
	push $47
	jmp irq_common_stub

#extern _irq_handler
.global irq_common_stub

# This is a stub that we have created for IRQ based ISRs. This calls
# '_irq_handler' in our C code. We need to create this in an 'irq.c'
irq_common_stub: 
    pusha
    push %ds
    push %es
    push %fs
    push %gs

    mov $0x10,%ax
    mov %ax,%ds
    mov %ax,%es
    mov %ax,%fs
    mov %ax,%gs
    mov %esp,%eax

    push %eax
    mov $irq_handler, %eax
    call *%eax
    popl %eax

    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa
    add $8,%esp
    iret


