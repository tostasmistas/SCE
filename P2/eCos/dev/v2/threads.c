#include "main.h"
#include "threads.h"
#include "threadInterface.h"
#include "threadCommunication.h"

char stack[NTHREADS][STKSIZE]; // space for two 4K stacks
cyg_thread 			thread_space[NTHREADS]; // space for two thread objects
cyg_handle_t 		threadInterface, threadCommunication; // handles for the threads
cyg_thread_entry_t	threadInterface_func, threadCommunication_func; // variables fot the procedure which is the thread

void cyg_threads_init(void) {

	cyg_thread_create(4, threadInterface_func, (cyg_addrword_t) 0, 
					  "thread interface", (void*) stack[0], STKSIZE,
					  &threadInterface, &thread_space[0]);

	cyg_thread_create(4, threadCommunication_func, (cyg_addrword_t) 1, 
					  "thread communication", (void*) stack[1], STKSIZE,
					  &threadCommunication, &thread_space[1]);

	cyg_thread_resume(threadInterface);

	cyg_thread_resume(threadCommunication);
}
