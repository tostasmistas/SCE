#include "main.h"
#include "threads.h"
#include "threadCommunication_RX.h"

cyg_io_handle_t serH;

/*-------------------------------------------------------------------------+
| function: receber mensagem (em hexadecimal) do PIC
+--------------------------------------------------------------------------*/
void recv_hex (char *byteToRead, unsigned int lenght) {
	cyg_io_read(serH, byteToRead, &lenght);
}

/*-------------------------------------------------------------------------+
| function: rotina associada a thread de comunicacao RX
+--------------------------------------------------------------------------*/
void threadCommunicationRX_func(cyg_addrword_t data) {

	unsigned char alreadyEOM = 0;
	unsigned char alreadySOM = 0;
	unsigned char byteRead = 0;
	unsigned char protocolo_OK = 0;
	unsigned char index = 0;
	unsigned int i = 0;
	unsigned char msgRecFromPIC[97] = {255}; //na posicao 0 armazenar o CMD e nas restantes 200 posicoes os registos
	
	for (;;) {
		recv_hex(&byteRead, 1); //esta thread esta sempre bloqueada a ler um byte

		//maquina de estados para verificar se a mensagem esta no protocolo correcto
		if(byteRead == EOM && alreadySOM == 1) { //verifica se EOM
			alreadyEOM = 1;
			alreadySOM = 0;
		}
		if(alreadySOM == 1) { //verifica se ja houve SOM
			if(index < 97){ //assumir que valor maximo de registos que se pode transferir e 12 (12*8 bytes = 96bytes)
				msgRecFromPIC[index] = byteRead;
				index++;
			}
		}
		if(byteRead == SOM) { //verifica se SOM
			alreadySOM = 1;
			index = 0;
			for(i = 0; i < 97; i++) {
				msgRecFromPIC[i] = 255;
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
			if(msgRecFromPIC[0] == NMCH) { //verificar se a mensagem foi uma notificacao de memoria cheia
				cyg_mutex_lock(&escritaScreen_mutex); //mutex para bloquear o recurso de escrita no ecra
				printf("aviso de memoria cheia\n"); //se sim entao escrever directamente no ecra o aviso de memoria cheia
				cyg_mutex_unlock(&escritaScreen_mutex); //desbloquear a escrita no ecra
			}
			else { //o codigo recebido nao foi de memoria cheia mas sim outro comando
				if(msgRecFromPIC[0] == TRGC || msgRecFromPIC[0] == TRGI) { //pedido de transferencia de registos
					cyg_mutex_lock(&transferRegistos_mutex); //mutex para bloquear o recurso de escrita na variavel global
					if(transferRegistos == 1) {	//pedido feito pela interface
						transferRegistos = 0;
						cyg_mutex_unlock(&transferRegistos_mutex); //desbloquear a escrita na variavel global
						printf("pedido de transfer registos feito pela interface\n");
						cyg_mutex_lock(&localMemory_mutex); //mutex para bloquear o recurso de escrita na memoria local
						printf("bloqueei\n");
						for(i = 1; i < 97; i++) {
							if(msgRecFromPIC[i] == 255) {
								break;	
							}
							localMemory[indescrita][(i%8)-1] = msgRecFromPIC[i];
							printf("%d ", msgRecFromPIC[i]);
							if(i%8 == 0) {
								indescrita++;
								nr++;
								printf("\n");
							}
							if(nr >= NRBUF){
								nr = NRBUF;
							}
							if(indescrita == NRBUF) {
								indescrita = 0; // voltar a primeira "linha" da memoria
							}
						}
						cyg_mutex_unlock(&localMemory_mutex); //desbloquear a escrita na memoria local
						printf("consegui transferir registos\n");
						char msg_send = CMD_OK;
						cyg_mbox_put(mbInter, &msg_send); //colocar na mbox de processamento o OK de transferencia concluida
					}
					else if(transferRegistos == 2) {	//pedido feito pelo processamento (transferencia periodica de registos fixos)
						transferRegistos = 0;
						cyg_mutex_unlock(&localMemory_mutex); //desbloquear a escrita na variavel global
						printf("pedido de transfer registos feito pelo processamento\n");
						cyg_mutex_lock(&localMemory_mutex); //mutex para bloquear o recurso de escrita na memoria local
						for(i = 1; i < 97; i++) {
							if(msgRecFromPIC[i] == 255) {
								break;	
							}
							localMemory[indescrita][(i%8)-1] = msgRecFromPIC[i];
							if(i%8 == 0) { //cada linha da memoria local e um registo - 8 bytes
								indescrita++;
								nr++;
							}
							if(nr >= NRBUF){
								nr = NRBUF;
							}
							if(indescrita == NRBUF) {
								indescrita = 0; // voltar a primeira "linha" da memoria
							}
						}
						cyg_mutex_unlock(&localMemory_mutex); //desbloquear a escrita na memoria local
						char msg_send = CMD_OK;
						cyg_mbox_put(mbProc, &msg_send); //colocar na mbox de processamento o OK de transferencia concluida
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
