#include "main.h"
#include "threads.h"
#include "threadInterface.h"
#include "threadCommunication_RX.h"
#include "threadCommunication_TX.h"
#include "threadProcessing.h"

char stack[NTHREADS][STKSIZE]; // space for two 4K stacks
cyg_thread 			thread_space[NTHREADS]; // space for two thread objects
cyg_mbox 				dataComTX, dataProc, dataInter;

void cyg_threads_init(void) {

	cyg_mutex_init(&localMemory_mutex); // criar mutex de proteccao de escrita na memoria local
	cyg_mutex_init(&transferRegistos_mutex); // criar mutex de proteccao de escrita na variavel global que indica quem pediu a transferencia de registos
	cyg_mutex_init(&escritaScreen_mutex); //criar mutex de proteccao de escrita no ecra

	//primeiro argumento <-> prioridade da thread

	// thread de comunicacao RX (recebe dados do PIC)
	cyg_thread_create(0, threadCommunicationRX_func, (cyg_addrword_t) 0,
					  				"thread communication RX", (void*) stack[0], STKSIZE,
					  				&threadCommunicationRX, &thread_space[0]);

	// thread de comunicacao TX (transmite dados para o PIC)
	cyg_thread_create(1, threadCommunicationTX_func, (cyg_addrword_t) 1,
					  				"thread communication TX", (void*) stack[1], STKSIZE,
					  				&threadCommunicationTX, &thread_space[1]);

	// thread de interface
	cyg_thread_create(2, threadInterface_func, (cyg_addrword_t) 2,
					  				"thread interface", (void*) stack[2], STKSIZE,
					  				&threadInterface, &thread_space[2]);

	// thread de processamento
	cyg_thread_create(1, threadProcessing_func, (cyg_addrword_t) 3,
					  				"thread processing", (void*) stack[3], STKSIZE,
					  				&threadProcessing, &thread_space[3]);

	//mbox para enviar dados para a comunicacao TX
	cyg_mbox_create(&mbComTX, &dataComTX);

	//mbox para enviar dados para o processamento
	cyg_mbox_create(&mbProc, &dataProc);

	//mbox para enviar dados para a interface
	cyg_mbox_create(&mbInter, &dataInter);

	processing_clockH = cyg_real_time_clock();
	cyg_clock_to_counter(processing_clockH, &countH); // contador associado ao alarme
	cyg_alarm_create(countH, alarme_func, (cyg_addrword_t) 0, &alarmProcH, &alarmProc);

	cyg_thread_resume(threadInterface);
	cyg_thread_resume(threadCommunicationTX);
	cyg_thread_resume(threadProcessing);
	cyg_thread_resume(threadCommunicationRX);
}
