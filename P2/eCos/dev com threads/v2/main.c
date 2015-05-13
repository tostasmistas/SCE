#include "main.h"
#include "threads.h"

cyg_handle_t 	threadInterface, threadCommunication, threadProcessing; // handles for the threads
cyg_handle_t 	mbInter, mbCom, mbProcInt,mbLoc, mbProc, mbIntProc; // handles for the mailboxs
cyg_mutex_t 	mem_lock;
//Memória Local
char **localmemory; //memória local
int nr; //numero de registos
int indescrita; //indice de escrita
int indleitura; //indíce de leitura

int main(void) {
	cyg_mutex_init(&mem_lock);
	int i;
	localmemory = (char *)malloc(NRBUF * sizeof(char *)); 
  //ponteiro para um array de [NRBUF] ponteiros

  for (i = 0; i < NRBUF; ++i){
      localmemory[i] = (char *)malloc(8 * sizeof(char)); 
      //pointer to a single array with 8 chars
	}
	indescrita=0;
	indleitura=0;
	nr=0;
	
	
	cyg_threads_init(); // criar threads, definir o seu estado inicial e criar mboxs

	//while(1);
	
	return 0;
}
