#include "main.h"
#include "threads.h"
#include "localMemory.h"

/* variaveis de threads */
cyg_handle_t 	threadInterface, threadCommunicationRX, threadCommunicationTX, threadProcessing; // handles for the threads
cyg_handle_t 	mbInter, mbInter, mbComTX, mbProc; // handles for the mailboxs
cyg_mutex_t 	localMemory_mutex, transferRegistos_mutex, escritaScreen_mutex;

/* variaveis da memória local */
char **localMemory; //memória local
int nr; //numero de registos
int indescrita; //indice de escrita
int indleitura; //indíce de leitura

int transferRegistos = 0; //1: interface 
						  //2: processamento

int main(void) {

	localMemory_init(); // inicialização da memoria local

	cyg_mutex_init(&localMemory_mutex); // criar mutex de proteccao de escrita na memoria local
	cyg_mutex_init(&transferRegistos_mutex); // criar mutex de proteccao de escrita na variavel global que indica quem pediu a transferencia de registos
	cyg_mutex_init(&escritaScreen_mutex); //criar mutex de proteccao de escrita no ecra
	cyg_threads_init(); // criar threads, definir o seu estado inicial e criar mboxs

	return 0;
}
