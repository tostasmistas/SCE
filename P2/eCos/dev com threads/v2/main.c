#include "main.h"
#include "threads.h"

cyg_handle_t 	threadInterface, threadCommunication; // handles for the threads
cyg_handle_t 	mbInter, mbCom; // handles for the mailboxs

int main(void) {

	cyg_threads_init(); // criar threads, definir o seu estado inicial e criar mboxs

	//while(1);
	
	return 0;
}
