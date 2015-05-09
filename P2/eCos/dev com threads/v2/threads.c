#include "main.h"
#include "threads.h"
#include "threadInterface.h"
#include "threadCommunication.h"

char stack[NTHREADS][STKSIZE]; // space for two 4K stacks
cyg_thread 			thread_space[NTHREADS]; // space for two thread objects
cyg_mbox 			interToCom, comToInter;

void cyg_threads_init(void) {
	//primeiro argumento <-> prioridade da thread
	cyg_thread_create(0, threadCommunication_func, (cyg_addrword_t) 1, 
					  "thread communication", (void*) stack[1], STKSIZE,
					  &threadCommunication, &thread_space[1]);

	
	cyg_thread_create(0, threadInterface_func, (cyg_addrword_t) 0, 
					  "thread interface", (void*) stack[0], STKSIZE,
					  &threadInterface, &thread_space[0]);

	//mbox para enviar dados da interface para a comunicacao <-> caixa de correio da comunicacao
	cyg_mbox_create(&mbCom, &interToCom);

	//mbox para enviar dados da comunicacao para a interface <-> caixa de correio da interface
  	cyg_mbox_create(&mbInter, &comToInter);
	
	cyg_thread_resume(threadInterface);
	cyg_thread_resume(threadCommunication); 
}
