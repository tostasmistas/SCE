#include "main.h"
#include "threads.h"
#include "localMemory.h"

/* variaveis de threads */
cyg_handle_t 	threadInterface, threadCommunicationRX, threadCommunicationTX, threadProcessing; // handles for the threads
cyg_handle_t 	mbInter, mbInter, mbComTX, mbProc; // handles for the mailboxs
cyg_handle_t	processing_clockH, countH; // handles for the counters
cyg_handle_t 	alarmProcH; // handles for the alarms
cyg_alarm 		alarmProc;
cyg_mutex_t 	localMemory_mutex, transferRegistos_mutex, escritaScreen_mutex;

/* variaveis da memória local */
char **localMemory; //memória local
int nr; //numero de registos
int indescrita; //indice de escrita
int indleitura; //indíce de leitura

int transferRegistos = 0; //1: interface & 2: processamento

int main(void) {

	localMemory_init(); // inicialização da memoria local
	cyg_threads_init(); // criar threads, definir o seu estado inicial, criar mboxs, alarmes, mutexs

	return 0;
}
