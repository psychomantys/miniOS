#ifndef  task_INC
#define  task_INC

#include	<stdint.h>
#include	<isa_specific_code.hpp>
#include	<kernel/paging.hpp>
#include	<kernel/start.hpp>

// This structure defines a 'task' - a process.
typedef struct task{
	int id;                // Process ID.
	uint32_t esp, ebp;       // Stack and base pointers.
	uint32_t eip;            // Instruction pointer.
	page_directory_t *page_directory; // Page directory.
	struct task *next;     // The next task in a linked list.
} task_t;

class Multitask{
	private:
		// The currently running task.
		volatile task_t *current_task;

		// The start of the task linked list.
		volatile task_t *ready_queue;

		// Some externs are needed to access members in paging.c...
//		extern page_directory_t *kernel_directory;
//		extern page_directory_t *current_directory;
//		extern void alloc_frame(page_t*,int,int);
		Paging &paging;

		// The next available process ID.
		uint32_t next_pid;

	public:
		Multitask( Paging p ) :
			paging(p),
			next_pid(1)
		{ }

		// Initialises the tasking system.
		void initialise_tasking();

		// Called by the timer hook, this changes the running process.
		void task_switch();

		// Forks the current process, spawning a new one with a different
		// memory space.
		int fork();

		// Causes the current process' stack to be forcibly moved to a new location.
		void move_stack(void *new_stack_start, uint32_t size);

		// Returns the pid of the current process.
		int getpid();
		void switch_task();
};

#endif   /* ----- #ifndef task_INC  ----- */

