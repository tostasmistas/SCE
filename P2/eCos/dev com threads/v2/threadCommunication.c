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

/*-------------------------------------------------------------------------+
| function: receber mensagem (em hexadecimal) do PIC
+--------------------------------------------------------------------------*/
Cyg_ErrNo recv_hex (char *byteToRead, unsigned int lenght) {
	err = cyg_io_read(serH, byteToRead, &lenght);
	return err;
}

void threadCommunication_func(cyg_addrword_t data) {

	unsigned char *msg_rec;
	char byteToRead;
	int i = 0;
	int erro = 0;

	msg_rec = cyg_mbox_get(mbCom);

	printf("recebi msg da thread interface!\n");

	switch(msg_rec[0]){
		case CRLG:
			printf("recebi CRLG da thread interface!\n");
			char msgToSend[3] = {0};
			char msgMBox[3] = {0}; // array para armazenar horas, minutos e segundos
			msgToSend[0] = SOM;
			msgToSend[1] = CRLG;
			msgToSend[2] = EOM;
			if(send_buffer(msgToSend) != 0) {
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
							msgMBox[i-2] = byteToRead;
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
				cyg_mbox_put(mbInter, msgMBox); // enviar horas, minutos e segundos
			}
			break;
		case ARLG:
			break;
		default:
			break;
	}
}
