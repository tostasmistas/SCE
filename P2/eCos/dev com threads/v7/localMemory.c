#include "main.h"
#include "localMemory.h"

void localMemory_init(void) {

	int i;
	localMemory = (char **)malloc(NRBUF * sizeof(char *));
  	//ponteiro para um array de [NRBUF] ponteiros

	for (i = 0; i < NRBUF; ++i){
      localMemory[i] = (char *)malloc(8 * sizeof(char));
      //pointer to a single array with 8 chars
	}
	indescrita = 0;
	indleitura = 0;
	nr = 0;
}


	
