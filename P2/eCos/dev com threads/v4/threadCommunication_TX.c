#include "main.h"
#include "threads.h"
#include "threadCommunication.h"

Cyg_ErrNo err;
cyg_io_handle_t serH;

/*-------------------------------------------------------------------------+
| function: enviar mensagem (em buffer hexadecimal) para o PIC
+--------------------------------------------------------------------------*/
Cyg_ErrNo send_buffer (char msgToSend[]) {
	unsigned int n = strlen(msgToSend) + 1;
	err = cyg_io_write(serH, msgToSend, &n);
	return err;
}

void threadCommunicationTX(cyg_addrword_t data) {

	unsigned char *msg_rec;
	char msgToSend3[3] = {0};
	char msgToSend4[4] = {0};
	char msgToSend6[6] = {0};
	char *msgMBox1 = malloc(sizeof(char)*1);
	char *msgMBox2 = malloc(sizeof(char)*2);
	char *msgMBox3 = malloc(sizeof(char)*3);
	char *msgMBox4 = malloc(sizeof(char)*4);
	char *msgMBox6 = malloc(sizeof(char)*6);
	unsigned char byteToRead;
	int i = 0;
	int erro = 0;

	msg_rec = cyg_mbox_get(mbComTX); // verificar qual a mensagem que a thread de interface enviou

	printf("recebi msg da thread interface!\n");

	switch(msg_rec[0]){
		case CRLG:
			printf("recebi CRLG da thread interface!\n");
			msgToSend3[0] = SOM;
			msgToSend3[1] = CRLG;
			msgToSend3[2] = EOM;
			if(send_buffer(msgToSend3) != 0) {
				erro = 1;
				printf("erro a enviar mensagem\n");
			}
			else {
				printf("vou receber a resposta\n");
				for(i = 0; i < 6; i++) { // receber byte a byte num total de 6 bytes
					if(recv_hex(&byteToRead, 1) != 0) { // erro na recepcao da mensagem
						erro = 1;
						break;
					}
					else { // nao houve erro a receber aquele byte da mensagem
						if(i == 0) {
							if(byteToRead != SOM) { // verificar se primeiro byte e SOM
								erro = 1;
								break;
							}
						}
						if(i == 1) {
							if(byteToRead != CRLG) { // verificar se segundo byte e CRLG
								erro = 1;
								break;
							}
						}
						if(i > 1 && i < 5) {
							msgMBox3[i-2] = (int)byteToRead;
						}
						if(i == 5) {
							if(byteToRead != EOM) { // verificar se ultimo byte e EOM
								erro = 1;
								break;
							}
						}
					}
				}
			}
			if(erro == 1) {
				printf("tive um erro");
				msgMBox1[0] = CMD_ERRO;
				cyg_mbox_put(mbInter, (void*)msgMBox1); // enviar aviso de erro
			}
			else {
				cyg_mbox_put(mbInter, (void*)msgMBox3); // enviar horas, minutos e segundos
			}
			break;
		case ARLG:
			printf("recebi ARLG da thread interface!\n");
			msgToSend6[0] = SOM;
			msgToSend6[1] = ARLG;
			msgToSend6[2] = msg_rec[1];
			msgToSend6[3] = msg_rec[2];
			msgToSend6[4] = msg_rec[3];
			msgToSend6[5] = EOM;
printf("novo relogio: %d %d %d\n", (int)msg_rec[1], msg_rec[2], msg_rec[3]);
			if(send_buffer(msgToSend6) != 0) {
				erro = 1;
				printf("erro a enviar mensagem\n");
			}
			else {
				printf("vou receber a resposta\n");
				for(i = 0; i < 4; i++) { // receber byte a byte num total de 4 bytes
					if(recv_hex(&byteToRead, 1) != 0) { // erro na recepcao da mensagem
						erro = 1;
						break;
					}
					else { // nao houve erro a receber a mensagem
						if(i == 0) {
							if(byteToRead != 0xFD) { // verificar se primeiro byte e SOM
								erro = 1;
								break;
							}
						}
						if(i == 1) {
							if(byteToRead != 0xC1) { // verificar se segundo byte e ARLG
								erro = 1;
								break;
							}
						}
						if(i == 2){
							msgMBox1[0] = byteToRead;
						}
						if(i == 3) {
							if(byteToRead != 0xFe) { // verificar se ultimo byte e EOM
								erro = 1;
								break;
							}
						}
					}
				}
			}
			if(erro == 1) {
				cyg_mbox_put(mbInter, (void*)-1); // enviar aviso de erro
			}
			else {
				cyg_mbox_put(mbInter, msgMBox1);
			}
			break;
		case CTEL:
			printf("recebi CTEL da thread interface!\n");
			msgToSend3[0] = SOM;
			msgToSend3[1] = CTEL;
			msgToSend3[2] = EOM;
			if(send_buffer(msgToSend3) != 0) {
				erro = 1;
				printf("erro a enviar mensagem\n");
			}
			else {
				printf("vou receber a resposta\n");
				for(i = 0; i < 5; i++) { // receber byte a byte num total de 5 bytes
					if(recv_hex(&byteToRead, 1) != 0) { // erro na recepcao da mensagem
						erro = 1;
						break;
					}
					else { // nao houve erro a receber a mensagem
						if(i > 1 && i < 4) {
							msgMBox2[i-2] = byteToRead;
						}
					}
				}
			}
			if(erro == 1) {
				cyg_mbox_put(mbInter, (void*)-1); // enviar aviso de erro
			}
			else {
				cyg_mbox_put(mbInter, msgMBox2); // enviar horas, minutos e segundos
			}
			break;
		case CPAR:
			printf("recebi CPAR da thread interface!\n");
			msgToSend3[0] = SOM;
			msgToSend3[1] = CPAR;
			msgToSend3[2] = EOM;
			if(send_buffer(msgToSend3) != 0) {
				erro = 1;
				printf("erro a enviar mensagem\n");
			}
			else {
				printf("vou receber a resposta\n");
				for(i = 0; i < 6; i++) { // receber byte a byte num total de 6 bytes
					if(recv_hex(&byteToRead, 1) != 0) { // erro na recepcao da mensagem
						erro = 1;
						break;
					}
					else { // nao houve erro a receber a mensagem
						if(i > 1 && i < 5) {
							msgMBox3[i-2] = byteToRead;
						}
					}
				}
			}
			if(erro == 1) {
				cyg_mbox_put(mbInter, (void*)-1); // enviar aviso de erro
			}
			else {
				/*msgMBox[0] = 5;
				msgMBox[1] = 6;
				msgMBox[2] = 7;*/
				cyg_mbox_put(mbInter, msgMBox3); // enviar horas, minutos e segundos
			}
			break;
		case MPMN:
			printf("recebi MPMN da thread interface!\n");
			msgToSend4[0] = SOM;
			msgToSend4[1] = MPMN;
			msgToSend4[2] = msg_rec[1];
			msgToSend4[3] = EOM;
			if(send_buffer(msgToSend4) != 0) {
				erro = 1;
				printf("erro a enviar mensagem\n");
			}
			else {
				printf("vou receber a resposta\n");
				for(i = 0; i < 4; i++) { // receber byte a byte num total de 4 bytes
					if(recv_hex(&byteToRead, 1) != 0) { // erro na recepcao da mensagem
						erro = 1;
						break;
					}
					else { // nao houve erro a receber a mensagem
						if(i==2){
							msgMBox1[0]=byteToRead;
						}
					}
				}
			}
			if(erro == 1) {
				cyg_mbox_put(mbInter, (void*)-1); // enviar aviso de erro
			}
			else {
				cyg_mbox_put(mbInter, msgMBox1); // enviar horas, minutos e segundos
			}
			break;
		case CALA:
			printf("recebi CALA da thread interface!\n");
			msgToSend3[0] = SOM;
			msgToSend3[1] = CALA;
			msgToSend3[2] = EOM;
			if(send_buffer(msgToSend3) != 0) {
				erro = 1;
				printf("erro a enviar mensagem\n");
			}
			else {
				printf("vou receber a resposta\n");
				for(i = 0; i < 9; i++) { // receber byte a byte num total de 6 bytes
					if(recv_hex(&byteToRead, 1) != 0) { // erro na recepcao da mensagem
						erro = 1;
						break;
					}
					else { // nao houve erro a receber a mensagem
						if(i > 1 && i < 8) {
							msgMBox6[i-2] = byteToRead;
						}
					}
				}
			}
			if(erro == 1) {
				cyg_mbox_put(mbInter, (void*)-1); // enviar aviso de erro
			}
			else {
				/*msgMBox[0] = 5;
				msgMBox[1] = 6;
				msgMBox[2] = 7;*/
				cyg_mbox_put(mbInter, msgMBox6); // enviar horas, minutos e segundos
			}
			break;
		case DALR:
			printf("recebi DALR da thread interface!\n");
			msgToSend6[0] = SOM;
			msgToSend6[1] = DALR;
			msgToSend6[2] = msg_rec[1];
			msgToSend6[3] = msg_rec[2];
			msgToSend6[4] = msg_rec[3];
			msgToSend6[5] = EOM;
printf("novo alarme relogio: %d %d %d\n", (int)msg_rec[1], msg_rec[2], msg_rec[3]);
			if(send_buffer(msgToSend6) != 0) {
				erro = 1;
				printf("erro a enviar mensagem\n");
			}
			else {
				printf("vou receber a resposta\n");
				for(i = 0; i < 4; i++) { // receber byte a byte num total de 4 bytes
					if(recv_hex(&byteToRead, 1) != 0) { // erro na recepcao da mensagem
						erro = 1;
						break;
					}
					else { // nao houve erro a receber a mensagem
						if(i==2){
							msgMBox1[0]=byteToRead;
						}
					}
				}
			}
			if(erro == 1) {
				cyg_mbox_put(mbInter, (void*)-1); // enviar aviso de erro
			}
			else {
				cyg_mbox_put(mbInter, msgMBox1); // enviar horas, minutos e segundos
			}
			break;
		case DALT:
			printf("recebi DALT da thread interface!\n");
			msgToSend4[0] = SOM;
			msgToSend4[1] = DALT;
			msgToSend4[2] = msg_rec[1];
			msgToSend4[3] = EOM;
printf("novo alarme temperatura: %d", (int)msg_rec[1]);
			if(send_buffer(msgToSend4) != 0) {
				erro = 1;
				printf("erro a enviar mensagem\n");
			}
			else {
				printf("vou receber a resposta\n");
				for(i = 0; i < 4; i++) { // receber byte a byte num total de 4 bytes
					if(recv_hex(&byteToRead, 1) != 0) { // erro na recepcao da mensagem
						erro = 1;
						break;
					}
					else { // nao houve erro a receber a mensagem
						if(i==2){
							msgMBox1[0]=byteToRead;
						}
					}
				}
			}
			if(erro == 1) {
				cyg_mbox_put(mbInter, (void*)-1); // enviar aviso de erro
			}
			else {
				cyg_mbox_put(mbInter, msgMBox1); // enviar horas, minutos e segundos
			}
			break;
		case DALL:
			printf("recebi DALL da thread interface!\n");
			msgToSend4[0] = SOM;
			msgToSend4[1] = DALL;
			msgToSend4[2] = msg_rec[1];
			msgToSend4[3] = EOM;
			if(send_buffer(msgToSend4) != 0) {
				erro = 1;
				printf("erro a enviar mensagem\n");
			}
			else {
				printf("vou receber a resposta\n");
				for(i = 0; i < 4; i++) { // receber byte a byte num total de 4 bytes
					if(recv_hex(&byteToRead, 1) != 0) { // erro na recepcao da mensagem
						erro = 1;
						break;
					}
					else { // nao houve erro a receber a mensagem
						if(i==2){
							msgMBox1[0]=byteToRead;
						}
					}
				}
			}
			if(erro == 1) {
				cyg_mbox_put(mbInter, (void*)-1); // enviar aviso de erro
			}
			else {
				cyg_mbox_put(mbInter, msgMBox1); // enviar horas, minutos e segundos
			}
			break;
		case AALA:
			printf("recebi AALA da thread interface!\n");
			msgToSend3[0] = SOM;
			msgToSend3[1] = AALA;
			msgToSend3[2] = EOM;
			if(send_buffer(msgToSend3) != 0) {
				erro = 1;
				printf("erro a enviar mensagem\n");
			}
			else {
				printf("vou receber a resposta\n");
				for(i = 0; i < 4; i++) { // receber byte a byte num total de 4 bytes
					if(recv_hex(&byteToRead, 1) != 0) { // erro na recepcao da mensagem
						erro = 1;
						break;
					}
					else { // nao houve erro a receber a mensagem
						if(i==2){
							msgMBox1[0]=byteToRead;
						}
					}
				}
			}
			if(erro == 1) {
				cyg_mbox_put(mbInter, (void*)-1); // enviar aviso de erro
			}
			else {
				cyg_mbox_put(mbInter, msgMBox1); // enviar horas, minutos e segundos
			}
			break;
		case IREG:
			printf("recebi IREG da thread interface!\n");
			msgToSend3[0] = SOM;
			msgToSend3[1] = IREG;
			msgToSend3[2] = EOM;
			if(send_buffer(msgToSend3) != 0) {
				erro = 1;
				printf("erro a enviar mensagem\n");
			}
			else {
				printf("vou receber a resposta\n");
				for(i = 0; i < 7; i++) { // receber byte a byte num total de 6 bytes
					if(recv_hex(&byteToRead, 1) != 0) { // erro na recepcao da mensagem
						erro = 1;
						break;
					}
					else { // nao houve erro a receber a mensagem
						if(i > 1 && i < 6) {
							msgMBox4[i-2] = byteToRead;
						}
					}
				}
			}
			if(erro == 1) {
				cyg_mbox_put(mbInter, (void*)-1); // enviar aviso de erro
			}
			else {
				/*msgMBox[0] = 5;
				msgMBox[1] = 6;cr
				msgMBox[2] = 7;*/
				cyg_mbox_put(mbInter, msgMBox4); // enviar horas, minutos e segundos
			}
			break;
		default:
			break;
	}
}
