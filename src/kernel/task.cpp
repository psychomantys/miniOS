/*
 * =====================================================================================
 *
 *       Filename:  task.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/13/2013 07:44:24 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include	<kernel/task.hpp>

void Multitask::initialise_tasking(){
	// Rather important stuff happening, no interrupts please!
	// asm volatile("cli");
	disable_interrupts();

	// Relocate the stack so we know where it is.
	move_stack((void*)0xE0000000, 0x2000);

	// Initialise the first task (kernel task)
	current_task = ready_queue = (task_t*)kmalloc(sizeof(task_t));
	current_task->id = next_pid++;
	current_task->esp = current_task->ebp = 0;
	current_task->eip = 0;
	current_task->page_directory = paging.current_directory;
	current_task->next = 0;

	// Reenable interrupts.
	enable_interrupts();
//	asm volatile("sti");
}

void Multitask::move_stack(void *new_stack_start, uint32_t size){
	uint32_t i;
	// Allocate some space for the new stack.
	for( i = (uint32_t)new_stack_start;
			i >= ((uint32_t)new_stack_start-size);
			i -= 0x1000)
	{
		// General-purpose stack is in user-mode.
		paging.alloc_frame( paging.get_page(i, 1, paging.current_directory), false /* User mode */, true /* Is writable */ );
	}

	// Flush the TLB by reading and writing the page directory address again.
	uint32_t pd_addr;
	asm volatile("mov %%cr3, %0" : "=r" (pd_addr));
	asm volatile("mov %0, %%cr3" : : "r" (pd_addr));

	// Old ESP and EBP, read from registers.
	uint32_t old_stack_pointer; asm volatile("mov %%esp, %0" : "=r" (old_stack_pointer));
	uint32_t old_base_pointer;  asm volatile("mov %%ebp, %0" : "=r" (old_base_pointer));

	// Offset to add to old stack addresses to get a new stack address.
	uint32_t offset            = (uint32_t)new_stack_start - initial_esp;

	// New ESP and EBP.
	uint32_t new_stack_pointer = old_stack_pointer + offset;
	uint32_t new_base_pointer  = old_base_pointer  + offset;

	// Copy the stack.
	memcpy((void*)new_stack_pointer, (void*)old_stack_pointer, initial_esp-old_stack_pointer);

	// Backtrace through the original stack, copying new values into
	// the new stack.  
	for(i = (uint32_t)new_stack_start; i > (uint32_t)new_stack_start-size; i -= 4)
	{
		uint32_t tmp = * (uint32_t*)i;
		// If the value of tmp is inside the range of the old stack, assume it is a base pointer
		// and remap it. This will unfortunately remap ANY value in this range, whether they are
		// base pointers or not.
		if (( old_stack_pointer < tmp) && (tmp < initial_esp))
		{
			tmp = tmp + offset;
			uint32_t *tmp2 = (uint32_t*)i;
			*tmp2 = tmp;
		}
	}

	// Change stacks.
	asm volatile("mov %0, %%esp" : : "r" (new_stack_pointer));
	asm volatile("mov %0, %%ebp" : : "r" (new_base_pointer));
}

void Multitask::switch_task(){
	// If we haven't initialised tasking yet, just return.
	if (!current_task)
		return;

	// Read esp, ebp now for saving later on.
	uint32_t esp, ebp, eip;
	asm volatile("mov %%esp, %0" : "=r"(esp));
	asm volatile("mov %%ebp, %0" : "=r"(ebp));

	// Read the instruction pointer. We do some cunning logic here:
	// One of two things could have happened when this function exits - 
	//   (a) We called the function and it returned the EIP as requested.
	//   (b) We have just switched tasks, and because the saved EIP is essentially
	//       the instruction after read_eip(), it will seem as if read_eip has just
	//       returned.
	// In the second case we need to return immediately. To detect it we put a dummy
	// value in EAX further down at the end of this function. As C returns values in EAX,
	// it will look like the return value is this dummy value! (0x12345).
	eip = read_eip();

	// Have we just switched tasks?
	if (eip == 0x12345)
		return;

	// No, we didn't switch tasks. Let's save some register values and switch.
	current_task->eip = eip;
	current_task->esp = esp;
	current_task->ebp = ebp;

	// Get the next task to run.
	current_task = current_task->next;
	// If we fell off the end of the linked list start again at the beginning.
	if (!current_task) current_task = ready_queue;

	eip = current_task->eip;
	esp = current_task->esp;
	ebp = current_task->ebp;

	// Make sure the memory manager knows we've changed page directory.
	paging.current_directory = current_task->page_directory;
	// Here we:
	// * Stop interrupts so we don't get interrupted.
	// * Temporarily puts the new EIP location in ECX.
	// * Loads the stack and base pointers from the new task struct.
	// * Changes page directory to the physical address (physicalAddr) of the new directory.
	// * Puts a dummy value (0x12345) in EAX so that above we can recognise that we've just
	//   switched task.
	// * Restarts interrupts. The STI instruction has a delay - it doesn't take effect until after
	//   the next instruction.
	// * Jumps to the location in ECX (remember we put the new EIP in there).
	asm volatile("         \
			cli;                 \
			mov %0, %%ecx;       \
			mov %1, %%esp;       \
			mov %2, %%ebp;       \
			mov %3, %%cr3;       \
			mov $0x12345, %%eax; \
			sti;                 \
			jmp *%%ecx           "
			: : "r"(eip), "r"(esp), "r"(ebp), "r"(paging.current_directory->physicalAddr));
}

int Multitask::fork(){
	// We are modifying kernel structures, and so cannot
	asm volatile("cli");

	// Take a pointer to this process' task struct for later reference.
	task_t *parent_task = (task_t*)current_task;

	// Clone the address space.
	page_directory_t *directory = paging.clone_directory(paging.current_directory);

	// Create a new process.
	task_t *new_task = (task_t*)kmalloc(sizeof(task_t));

	new_task->id = next_pid++;
	new_task->esp = new_task->ebp = 0;
	new_task->eip = 0;
	new_task->page_directory = directory;
	new_task->next = 0;

	// Add it to the end of the ready queue.
	task_t *tmp_task = (task_t*)ready_queue;
	while (tmp_task->next)
		tmp_task = tmp_task->next;
	tmp_task->next = new_task;

	// This will be the entry point for the new process.
	uint32_t eip = read_eip();

	// We could be the parent or the child here - check.
	if (current_task == parent_task){
		// We are the parent, so set up the esp/ebp/eip for our child.
		uint32_t esp; asm volatile("mov %%esp, %0" : "=r"(esp));
		uint32_t ebp; asm volatile("mov %%ebp, %0" : "=r"(ebp));
		new_task->esp = esp;
		new_task->ebp = ebp;
		new_task->eip = eip;
		asm volatile("sti");

		return new_task->id;
	}else{
		// We are the child.
		return 0;
	}
}


/*
void copy_page_physical( uint32_t dest, uint32_t src){
	disable_interrupts();

	register uint32_t cr0;
	get_register_cr0(cr0);
	cr0 &= 0x7fffffff;
	set_register_cr0(cr0);

	memset(dest, src, 4*1024 );

	get_register_cr0(cr0);
	cr0 |= 0x80000000;
	set_register_cr0(cr0);
}
*/
/*
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



