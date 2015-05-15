#include "main.h"
#include "threads.h"
#include "threadCommunication_RX.h"

Cyg_ErrNo err;
cyg_io_handle_t serH;

/*-------------------------------------------------------------------------+
| function: receber mensagem (em hexadecimal) do PIC
+--------------------------------------------------------------------------*/
Cyg_ErrNo recv_hex (char *byteToRead, unsigned int lenght) {
	err = cyg_io_read(serH, byteToRead, &lenght);
	return err;
}

void threadCommunicationRX_func(cyg_addrword_t data) {

	unsigned char alreadySOM = 0;
	unsigned char alreadyEOM = 0;
	unsigned char byteRead = 0;
	unsigned char protocolo_OK = 1;
	unsigned char index = 0;
	unsigned char msgRecFromPIC[201] = {0}; //na posicao 0 armazenar o CMD e nas restantes 200 posicoes os registos
	unsigned int i = 0;

	recv_hex(&byteRead, 1); //esta thread esta sempre bloqueada a ler um byte

	//maquina de estados para verificar se a mensagem esta no protocolo correcto
	if(byteRead == EOM && alreadySOM == 1) { //verifica se EOM
		alreadyEOM = 1;
		alreadySOM = 0;
	}
	if(alreadySOM == 1) { //verifica se ja houve SOM
		if(index < 200){ //assumir que valor maximo de registos que se pode transferir e 25 (25*8 bytes = 200bytes)
			msgRecFromPIC[index] = byteRead;
			index++;
		}
	}
	if(byteRead == SOM) { //verifica se SOM
		alreadySOM = 1;
		index = 0;
		for(i = 0; i < 201; i++) {
			msgRecFromPIC[i] = 0;
		}
	}
	if(alreadyEOM == 1) {
		alreadyEOM = 0;
		protocolo_OK = 1;
	}

	if(protocolo_OK == 1) { //ha uma mensagem valida
		if(msgRecFromPIC[0] == 0xCD) { //verificar se a mensagem foi uma notificacao de memoria cheia
			while(cyg_mutex_lock(&escritaScreen_mutex) != true); //mutex para bloquear o recurso de escrita no ecra
			printf("aviso de memoria cheia\n"); //se sim entao escrever directamente no ecra o aviso de memoria cheia
			cyg_mutex_unlock(&escritaScreen_mutex); //desbloquear a escrita no ecra
		}
		else { //o codigo recebido nao foi de memoria cheia mas sim outro comando - agora ha que verificar qual a thread que efectuou um pedido: interface ou processamento
			if(msgRecFromPIC[0] == 0xCB || msgRecFromPIC[0] == 0xCC) { //pedido feito pelo processamento (transferencia de registos)
				while(cyg_mutex_lock(&localMemory_mutex) != true); //mutex para bloquear o recurso de escrita na memoria local
				for(i = 1; i < 200; i++) { 
					localMemory[indescrita][i-1] = msgRecFromPIC[i];
					if(i%8 == 0) {
						indescrita++;
					}
					if(indescrita == NRBUF) {
						indescrita = 0; // voltar a primeira "linha" da memoria
					}
				}
				cyg_mutex_unlock(&localMemory_mutex); //desbloquear a escrita na memoria local
				cyg_mbox_put(mbProc, (void*)CMD_OK); //colocar na mbox de processamento o OK de transferencia concluida
			}
			else { //pedido feito pela interface
				cyg_mbox_put(mbInter, (void*)msgRecFromPIC); //colocar pedido na mbox de interface
			}
		}
	}
}
