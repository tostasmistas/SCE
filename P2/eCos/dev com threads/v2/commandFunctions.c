#include "main.h"
#include "threads.h"
#include "commandFunctions.h"

Cyg_ErrNo err;
cyg_io_handle_t serH;

/*-------------------------------------------------------------------------+
| function: cmd_ini - inicializar dispositivo
+--------------------------------------------------------------------------*/
void cmd_ini(int argc, char **argv) {
	printf("io_lookup\n");
	if ((argc > 1) && (argv[1][0] = '1'))
	err = cyg_io_lookup("/dev/ser1", &serH);
	else err = cyg_io_lookup("/dev/ser0", &serH);
	printf("lookup err=%x\n", err);
}

/*-------------------------------------------------------------------------+
| Function: cmd_sair - termina a aplicacao
+--------------------------------------------------------------------------*/
void cmd_sair(void) {
	exit(0);
}

/*-------------------------------------------------------------------------+
| function: cmd_cr - consultar relogio
+--------------------------------------------------------------------------*/
void cmd_cr(void) {

	char msg_send = CRLG;
	cyg_mbox_put(mbCom, &msg_send);

	printf("enviei mensagem para a thread communication e acordei-a\n");

	char *msg_rec;
	msg_rec = cyg_mbox_get(mbInter);
	if(msg_rec[0] == -1) {
		printf("erro transmissão da mensagem\n");
	}
	else {
		printf("horas: %d\n", msg_rec[0]);
		printf("minutos: %d\n", msg_rec[1]);
		printf("segundos: %d\n", msg_rec[2]);
	}
}
