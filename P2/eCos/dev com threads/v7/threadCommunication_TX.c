#include "main.h"
#include "threads.h"
#include "threadCommunication_TX.h"

cyg_io_handle_t serH;

/*-------------------------------------------------------------------------+
| function: enviar mensagem (em buffer hexadecimal) para o PIC
+--------------------------------------------------------------------------*/
void send_buffer (char msgToSend[], unsigned int length) {
	cyg_io_write(serH, msgToSend, &length);
}

/*-------------------------------------------------------------------------+
| function: rotina associada a thread de comunicacao TX
+--------------------------------------------------------------------------*/
void threadCommunicationTX_func(cyg_addrword_t data) {

	unsigned char *msg_rec;
	char msgToSend3[3] = {0};
	char msgToSend4[4] = {0};
	char msgToSend5[5] = {0};
	char msgToSend6[6] = {0};
	int erro = 0;

	for (;;) {
		erro = 0;
		msg_rec = cyg_mbox_get(mbComTX); // verificar qual a mensagem que a thread de interface enviou

		printf("recebi msg da thread interface!\n");

		switch(msg_rec[0]){
			case CRLG:
				printf("recebi CRLG da thread interface!\n");
				msgToSend3[0] = SOM;
				msgToSend3[1] = CRLG;
				msgToSend3[2] = EOM;
				send_buffer(msgToSend3, 3);
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
				send_buffer(msgToSend6, 6);
				break;
			case CTEL:
				printf("recebi CTEL da thread interface!\n");
				msgToSend3[0] = SOM;
				msgToSend3[1] = CTEL;
				msgToSend3[2] = EOM;
				send_buffer(msgToSend3, 3);
				break;
			case CPAR:
				printf("recebi CPAR da thread interface!\n");
				msgToSend3[0] = SOM;
				msgToSend3[1] = CPAR;
				msgToSend3[2] = EOM;
				send_buffer(msgToSend3, 3);
				break;
			case MPMN:
				printf("recebi MPMN da thread interface!\n");
				msgToSend4[0] = SOM;
				msgToSend4[1] = MPMN;
				msgToSend4[2] = msg_rec[1];
				msgToSend4[3] = EOM;
				printf("novo pm: %d\n", msgToSend4[2]);
				send_buffer(msgToSend4, 4);
				break;
			case CALA:
				printf("recebi CALA da thread interface!\n");
				msgToSend3[0] = SOM;
				msgToSend3[1] = CALA;
				msgToSend3[2] = EOM;
				send_buffer(msgToSend3, 3);
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
				send_buffer(msgToSend6, 6);
				break;
			case DALT:
				printf("recebi DALT da thread interface!\n");
				msgToSend4[0] = SOM;
				msgToSend4[1] = DALT;
				msgToSend4[2] = msg_rec[1];
				msgToSend4[3] = EOM;
				printf("novo alarme temperatura: %d", (int)msg_rec[1]);
				send_buffer(msgToSend4, 4);
				break;
			case DALL:
				printf("recebi DALL da thread interface!\n");
				msgToSend4[0] = SOM;
				msgToSend4[1] = DALL;
				msgToSend4[2] = msg_rec[1];
				msgToSend4[3] = EOM;
				send_buffer(msgToSend4, 4);
				break;
			case AALA:
				printf("recebi AALA da thread interface!\n");
				msgToSend3[0] = SOM;
				msgToSend3[1] = AALA;
				msgToSend3[2] = EOM;
				send_buffer(msgToSend3, 3);
				break;
			case IREG:
				printf("recebi IREG da thread interface!\n");
				msgToSend3[0] = SOM;
				msgToSend3[1] = IREG;
				msgToSend3[2] = EOM;
				send_buffer(msgToSend3, 3);
				break;
			case TRGC:
				printf("recebi TRGC da thread interface!\n");
				msgToSend4[0] = SOM;
				msgToSend4[1] = TRGC;
				msgToSend4[2] = msg_rec[1];
				msgToSend4[3] = EOM;
				send_buffer(msgToSend4, 4);
				break;
			case TRGI:
				printf("recebi TRGI da thread interface!\n");
				msgToSend5[0] = SOM;
				msgToSend5[1] = TRGI;
				msgToSend5[2] = msg_rec[1];
				msgToSend5[3] = msg_rec[2];
				msgToSend5[4] = EOM;
				send_buffer(msgToSend5, 5);
				break;
			default:
				break;
		}
	}
}
