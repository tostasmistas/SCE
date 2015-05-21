#include "main.h"
#include "localMemory.h"

void localMemory_init(void) {

	int i = 0;
	int j = 0;

	localMemory = (char **)malloc(NRBUF * sizeof(char *));
  	//ponteiro para um array de [NRBUF] ponteiros

	for (i = 0; i < NRBUF; ++i){
      localMemory[i] = (char *)malloc(8 * sizeof(char));
      //pointer to a single array with 8 chars
	}

	for(i = 0; i < NRBUF; i++) {
		for(j = 0; j < 8; i++) {
			localMemory[i][j] = 255;
		}
	}
	
	indescrita = 0;
	indleitura = 0;
	nr = 0;
}
