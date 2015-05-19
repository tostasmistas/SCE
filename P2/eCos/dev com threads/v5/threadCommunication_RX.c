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

	unsigned char alreadyEOM = 0;
	unsigned char alreadySOM = 0;
	unsigned char byteRead = 0;
	unsigned char protocolo_OK = 0;
	unsigned char index = 0;
	unsigned char msgRecFromPIC[97] = {0}; //na posicao 0 armazenar o CMD e nas restantes 200 posicoes os registos
	unsigned int i = 0;

	for (;;) {
		recv_hex(&byteRead, 1); //esta thread esta sempre bloqueada a ler um byte

		//maquina de estados para verificar se a mensagem esta no protocolo correcto
		if(byteRead == EOM && alreadySOM == 1) { //verifica se EOM
			alreadyEOM = 1;
			alreadySOM = 0;
		}
		if(alreadySOM == 1) { //verifica se ja houve SOM
			if(index < 97){ //assumir que valor maximo de registos que se pode transferir e 12 (12*8 bytes = 96bytes)
				printf("%d\n", byteRead);
				msgRecFromPIC[index] = byteRead;
				index++;
			}
		}
		if(byteRead == SOM) { //verifica se SOM
			alreadySOM = 1;
			index = 0;
			for(i = 0; i < 97; i++) {
				msgRecFromPIC[i] = 0;
			}
		}
		if(alreadyEOM == 1) {
			alreadyEOM = 0;
			protocolo_OK = 1;
		}

		if(protocolo_OK == 1) { //ha uma mensagem valida
			byteRead = 0;
			protocolo_OK = 0;
			index = 0;
			i = 0;
			if(msgRecFromPIC[0] == 0xCD) { //verificar se a mensagem foi uma notificacao de memoria cheia
				cyg_mutex_lock(&escritaScreen_mutex); //mutex para bloquear o recurso de escrita no ecra
				printf("aviso de memoria cheia\n"); //se sim entao escrever directamente no ecra o aviso de memoria cheia
				cyg_mutex_unlock(&escritaScreen_mutex); //desbloquear a escrita no ecra
			}
			else { //o codigo recebido nao foi de memoria cheia mas sim outro comando - agora ha que verificar qual a thread que efectuou um pedido: interface ou processamento
				if(msgRecFromPIC[0] == 0xCB || msgRecFromPIC[0] == 0xCC) { //pedido de transferencia de registos
					if(transferRegistos == 1) {	//pedido feito pela interface
						cyg_mutex_lock(&localMemory_mutex); //mutex para bloquear o recurso de escrita na variavel global
						transferRegistos = 0;	
						cyg_mutex_unlock(&localMemory_mutex); //desbloquear a escrita na variavel global
						printf("pedido feito pela interface\n");
						cyg_mutex_lock(&localMemory_mutex); //mutex para bloquear o recurso de escrita na memoria local
						for(i = 1; i < sizeof(msgRecFromPIC); i++) { 
							localMemory[indescrita][(i%8)-1] = msgRecFromPIC[i];
							if(i%8 == 0) {
								indescrita++;
								nr++;
							}
							if(nr >= NRBUF){
								nr=NRBUF;
							}
							if(indescrita == NRBUF) {
								indescrita = 0; // voltar a primeira "linha" da memoria
							}
						}
						cyg_mutex_unlock(&localMemory_mutex); //desbloquear a escrita na memoria local
						cyg_mbox_put(mbInter, (void*)CMD_OK); //colocar na mbox de processamento o OK de transferencia concluida
					}
					else if(transferRegistos == 2) {	//pedido feito pelo processamento (transferencia periodica de registos fixos)
						cyg_mutex_lock(&localMemory_mutex); //mutex para bloquear o recurso de escrita na variavel global
						transferRegistos = 0;	
						cyg_mutex_unlock(&localMemory_mutex); //desbloquear a escrita na variavel global
						printf("pedido feito pelo processamento\n");
						cyg_mutex_lock(&localMemory_mutex); //mutex para bloquear o recurso de escrita na memoria local
						for(i = 1; i < sizeof(msgRecFromPIC); i++) { 
							localMemory[indescrita][(i%8)-1] = msgRecFromPIC[i];
							if(i%8 == 0) {
								indescrita++;
								nr++;
							}
							if(nr >= NRBUF){
								nr=NRBUF;
							}
							if(indescrita == NRBUF) {
								indescrita = 0; // voltar a primeira "linha" da memoria
							}
						}
						cyg_mutex_unlock(&localMemory_mutex); //desbloquear a escrita na memoria local
						cyg_mbox_put(mbProc, (void*)CMD_OK); //colocar na mbox de processamento o OK de transferencia concluida
					}
				}			
				else { //pedido feito pela interface (que tambem pode ser transferencia de 
					printf("pedido feito pela interface\n");
					cyg_mbox_put(mbInter, (void*)msgRecFromPIC); //colocar pedido na mbox de interface
				}
			}
		}
	}
}
