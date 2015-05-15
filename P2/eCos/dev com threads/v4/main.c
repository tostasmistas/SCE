#include "main.h"
#include "threads.h"

cyg_handle_t 	threadInterface, threadCommunicationRX, threadCommunicationTX, threadProcessing; // handles for the threads
cyg_handle_t 	mbInter, mbInter, mbComTX, mbProc; // handles for the mailboxs
cyg_handle_t 	alarmProc; // handles for the alarms
cyg_mutex_t 	localMemory_mutex, escritaScreen_mutex;

/* variaveis da memória local */
char **localMemory; //memória local
int nr; //numero de registos
int indescrita; //indice de escrita
int indleitura; //indíce de leitura

int main(void) {

	int i;
	localMemory = (char **)malloc(NRBUF * sizeof(char **));
  	//ponteiro para um array de [NRBUF] ponteiros

	for (i = 0; i < NRBUF; ++i){
      localMemory[i] = (char *)malloc(8 * sizeof(char*));
      //pointer to a single array with 8 chars
	}
	indescrita = 0;
	indleitura = 0;
	nr = 0;

	cyg_mutex_init(&localMemory_mutex); // criar mutex de proteccao de escriat na memoria local
	cyg_mutex_init(&escritaScreen_mutex); //criar mutex de proteccao de escrita no ecra
	cyg_threads_init(); // criar threads, definir o seu estado inicial e criar mboxs

	return 0;
}
