#include "main.h"
#include "threads.h"
#include "threadInterface.h"
#include "threadCommunication.h"
#include "threadProcessing.h"

char stack[NTHREADS][STKSIZE]; // space for two 4K stacks
cyg_thread 			thread_space[NTHREADS]; // space for two thread objects
cyg_mbox 				interToComTX, procToComTX, interToProc, comRXToProc, comRXToInter, interToMemLoc;

void cyg_threads_init(void) {
	//primeiro argumento <-> prioridade da thread

	// thread de comunicacao RX (recebe dados do PIC)
	cyg_thread_create(0, threadCommunicationRX_func, (cyg_addrword_t) 0,
					  				"thread communication RX", (void*) stack[0], STKSIZE,
					  				&threadCommunicationRX, &thread_space[0]);

	// thread de comunicacao TX (transmite dados para o PIC)
	cyg_thread_create(0, threadCommunicationTX_func, (cyg_addrword_t) 1,
					  				"thread communication TX", (void*) stack[1], STKSIZE,
					  				&threadCommunicationTX, &thread_space[1]);

	// thread de interface
	cyg_thread_create(0, threadInterface_func, (cyg_addrword_t) 2,
					  				"thread interface", (void*) stack[2], STKSIZE,
					  				&threadInterface, &thread_space[2]);

	// thread de processamento
	cyg_thread_create(0, threadProcessing_func, (cyg_addrword_t) 3,
					  				"thread processing", (void*) stack[3], STKSIZE,
					  				&threadProcessing, &thread_space[3]);

	//mbox para enviar dados da interface para a comunicacao TX <-> caixa de correio da comunicacao TX
	cyg_mbox_create(&mbComTX, &interToComTX);

	//mbox para enviar dados do processamento para a comunicacao TX <-> caixa de correio da comunicacao TX
	cyg_mbox_create(&mbComTX, &procToComTX);

	//mbox para enviar dados da interface para o processamento <-> caixa de correio do processamento
	cyg_mbox_create(&mbProc, &interToProc);

	//mbox para enviar dados da comunicação RX para o processamento <-> caixa de correio do processamento
	cyg_mbox_create(&mbProc, &comRXToProc);

	//mbox para enviar dados da comunicacao RX para a interface <-> caixa de correio da interface
	cyg_mbox_create(&mbInter, &comRXToInter);

	//mbox para enviar dados da memoria local para a interface <-> caixa de correio da interface
	cyg_mbox_create(&mbInter, &interToMemLoc);

	cyg_thread_resume(threadInterface);
	cyg_thread_resume(threadCommunicationRX);
	cyg_thread_resume(threadCommunicationTX);
	cyg_thread_resume(threadProcessing);
}
