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
void cmd_sair(int argc, char** argv) {
	exit(0);
}

/*-------------------------------------------------------------------------+
| function: cmd_cr - consultar relogio
+--------------------------------------------------------------------------*/
void cmd_cr(int argc, char** argv) {

	char msg_send = CRLG;
	cyg_mbox_put(mbCom, &msg_send);
	cyg_thread_resume(threadCommunication);

	printf("enviei mensagem para a thread communication e acordei-a\n");

	char *msg_rec;
	msg_rec = cyg_mbox_get(mbInter);
	if(msg_rec[0] == -1) {
		printf("erro transmissão da mensagem\n");
	}
	else {
		printf("horas: %d\n", (int)msg_rec[0]);
		printf("minutos: %d\n", (int)msg_rec[1]);
		printf("segundos: %d\n", (int)msg_rec[2]);
	}
}
/*-------------------------------------------------------------------------+
| function: cmd_ar - acertar relógio
+--------------------------------------------------------------------------*/
void cmd_ar(int argc, char** argv){

	char msg_send[4];
 	unsigned int i;

	msg_send[0]=ARLG;
 	if (argc > 1) {
    	if(argc < 4) {
    		printf("Erro: Não introduziu todos os parâmetros.");
	  	}else{
			for (i=0; i<(argc-1); i++)
				{unsigned int x; sscanf(argv[i+1], "%d", &x); msg_send[i+1]=(unsigned char)x;}
    //Enviar mensagem para tarefa de Comunicação
    		printf("horas: %d\n", (int)msg_send[1]);
			printf("minutos: %d\n", (int)msg_send[2]);
			printf("segundos: %d\n", (int)msg_send[3]);
			cyg_mbox_put(mbCom, &msg_send);	
			cyg_thread_resume(threadCommunication);
			printf("enviei mensagem para a thread communication e acordei-a\n");

			char *msg_rec;
			msg_rec = cyg_mbox_get(mbInter);
			if(msg_rec[0] == -1) {
				printf("erro transmissão da mensagem\n");
			}
			else {
				printf("horas mudadas\n");
			}
		}
  	}else{
		printf("Erro: Não introduziu parâmetros.");
	}

}
/*-------------------------------------------------------------------------+
| function: cmd_ctl - consultar temperatura e luminosidade
+--------------------------------------------------------------------------*/
void cmd_ctl(int argc, char** argv) {

	char msg_send = CTEL;
	cyg_mbox_put(mbCom, &msg_send);
	cyg_thread_resume(threadCommunication);
	
	printf("enviei mensagem para a thread communication e acordei-a\n");

	char *msg_rec;
	msg_rec = cyg_mbox_get(mbInter);
	if(msg_rec[0] == -1) {
		printf("erro transmissão da mensagem\n");
	}
	else {
		printf("temperatura: %d\n", (int)msg_rec[0]);
		printf("luminosidade: %d\n", (int)msg_rec[1]);
	}
}
/*-------------------------------------------------------------------------+
| function: cmd_cp - consultar parâmetros
+--------------------------------------------------------------------------*/
void cmd_cp(int argc, char** argv) {

	char msg_send = CPAR;
	cyg_mbox_put(mbCom, &msg_send);
	cyg_thread_resume(threadCommunication);

	printf("enviei mensagem para a thread communication e acordei-a\n");

	char *msg_rec;
	msg_rec = cyg_mbox_get(mbInter);
	if(msg_rec[0] == -1) {
		printf("erro transmissão da mensagem\n");
	}
	else {
		printf("NREG: %d\n", (int)msg_rec[0]);
		printf("PMON: %d\n", (int)msg_rec[1]);
		printf("TSOM: %d\n", (int)msg_rec[2]);
	}
}
/*-------------------------------------------------------------------------+
| function: cmd_mpm - modificar período de monitorização
+--------------------------------------------------------------------------*/
void cmd_mpm(int argc, char** argv) {

	char msg_send[2];
	msg_send[0]=MPMN;
	if(argc<=1){
		printf("Erro: Não introduziu parâmetros.");
	}else{
		{unsigned int x; sscanf(argv[1], "%x", &x); msg_send[1]=(unsigned char)x;}
		cyg_mbox_put(mbCom, &msg_send);		
		cyg_thread_resume(threadCommunication);
		printf("enviei mensagem para a thread communication e acordei-a\n");
		char *msg_rec;
		msg_rec = cyg_mbox_get(mbInter);
		if(msg_rec[0] == -1) {
			printf("erro transmissão da mensagem\n");
		}
		else {
			printf("período de monitorização modificado\n");
		}
	}
}
/*-------------------------------------------------------------------------+
| function: cmd_ca - consultar alarmes
+--------------------------------------------------------------------------*/
void cmd_ca(int argc, char** argv) {

	char msg_send = CALA;
	cyg_mbox_put(mbCom, &msg_send);
	
	cyg_thread_resume(threadCommunication);

	printf("enviei mensagem para a thread communication e acordei-a\n");

	char *msg_rec;
	msg_rec = cyg_mbox_get(mbInter);
	if(msg_rec[0] == -1) {
		printf("erro transmissão da mensagem\n");
	}
	else {
		printf("alarme horas: %d\n", (int)msg_rec[0]);
		printf("alarme minutos: %d\n", (int)msg_rec[1]);
		printf("alarme segundos: %d\n", (int)msg_rec[2]);
		printf("alarme temperatura: %d\n", (int)msg_rec[3]);
		printf("alarme luminosidade: %d\n", (int)msg_rec[4]);
		printf("estado alarmes: %c\n", msg_rec[5]);
	}
}
/*-------------------------------------------------------------------------+
| function: cmd_dar - definição do alarme do relógio
+--------------------------------------------------------------------------*/
void cmd_dar(int argc, char** argv) {

	char msg_send[4];
 	unsigned int i;

 	
	msg_send[0]=DALR;
	if (argc > 1) {
    	if(argc < 4) {
    		printf("Erro: Não introduziu todos os parâmetros.");
	  	}else{
		  	for (i=0; i<(argc-1); i++)
				{unsigned int x; sscanf(argv[i+1], "%x", &x); msg_send[i+1]=(unsigned char)x;}
    //Enviar mensagem para tarefa de Comunicação
    
			cyg_mbox_put(mbCom, &msg_send);
			
			cyg_thread_resume(threadCommunication);
			printf("enviei mensagem para a thread communication e acordei-a\n");

			char *msg_rec;
			msg_rec = cyg_mbox_get(mbInter);
			if(msg_rec[0] == -1) {
				printf("erro transmissão da mensagem\n");
			}
			else {
				printf("alarme horas mudado\n");
			}
		}
  	}else{
		printf("Erro: Não introduziu parâmetros.");
	}
}
/*-------------------------------------------------------------------------+
| function: cmd_dat - definir alarme temperatura
+--------------------------------------------------------------------------*/
void cmd_dat(int argc, char** argv) {

	char msg_send[2];
	msg_send[0]=DALT;
	if(argc<=1){
		printf("Erro: Não introduziu parâmetros.");
	}else{
		{unsigned int x; sscanf(argv[1], "%x", &x); msg_send[1]=(unsigned char)x;}
		cyg_mbox_put(mbCom, &msg_send);
		
	cyg_thread_resume(threadCommunication);
		printf("enviei mensagem para a thread communication e acordei-a\n");
		char *msg_rec;
		msg_rec = cyg_mbox_get(mbInter);
		if(msg_rec[0] == -1) {
			printf("erro transmissão da mensagem\n");
		}
		else {
			printf("alarme temperatura mudado\n");
		}
	}
}
/*-------------------------------------------------------------------------+
| function: cmd_dal - definir alarme luminosidade
+--------------------------------------------------------------------------*/
void cmd_dal(int argc, char** argv) {

	char msg_send[2];
	msg_send[0]=DALL;
	if(argc<=1){
		printf("Erro: Não introduziu parâmetros.");
	}else{
		{unsigned int x; sscanf(argv[1], "%x", &x); msg_send[1]=(unsigned char)x;}
		cyg_mbox_put(mbCom, &msg_send);
		
	cyg_thread_resume(threadCommunication);
		printf("enviei mensagem para a thread communication e acordei-a\n");
		char *msg_rec;
		msg_rec = cyg_mbox_get(mbInter);
		if(msg_rec[0] == -1) {
			printf("erro transmissão da mensagem\n");
		}
		else {
			printf("alarme temperatura mudado\n");
		}
	}
}
/*-------------------------------------------------------------------------+
| function: cmd_aa - activar/desactivar alarmes
+--------------------------------------------------------------------------*/
void cmd_aa(int argc, char** argv) {

	char msg_send = AALA;
	cyg_mbox_put(mbCom, &msg_send);
	
	cyg_thread_resume(threadCommunication);

	printf("enviei mensagem para a thread communication e acordei-a\n");

	char *msg_rec;
	msg_rec = cyg_mbox_get(mbInter);
	if(msg_rec[0] == -1) {
		printf("erro transmissão da mensagem\n");
	}
	else {
		printf("alarmes desactivados/activados\n");
	}
}
/*-------------------------------------------------------------------------+
| function: cmd_ir - informação sobre registos
+--------------------------------------------------------------------------*/
void cmd_ir(int argc, char** argv) {

	char msg_send = IREG;
	cyg_mbox_put(mbCom, &msg_send);
	
	cyg_thread_resume(threadCommunication);

	printf("enviei mensagem para a thread communication e acordei-a\n");

	char *msg_rec;
	msg_rec = cyg_mbox_get(mbInter);
	if(msg_rec[0] == -1) {
		printf("erro transmissão da mensagem\n");
	}
	else {
		printf("NREG: %d\n", (int)msg_rec[0]);
		printf("nr: %d\n", (int)msg_rec[1]);
		printf("ie: %d\n", (int)msg_rec[2]);
		printf("il: %d\n", (int)msg_rec[3]);
	}
}
/*-------------------------------------------------------------------------+
| function: cmd_trc - transfererir n registos a partir de il corrente
+--------------------------------------------------------------------------*/
void cmd_trc(int argc, char** argv) {

	char msg_send[2];
 	unsigned int i;

 	
	msg_send[0]=TRGC;
	if (argc > 1) {
    	if(argc < 2) {
    		printf("Erro: Não introduziu todos os parâmetros.");
	  	}else{
		  	for (i=0; i<(argc-1); i++)
				{unsigned int x; sscanf(argv[i+1], "%x", &x); msg_send[i+1]=(unsigned char)x;}
    //Enviar mensagem para tarefa de Comunicação
    
			cyg_mbox_put(mbCom, &msg_send);
			printf("enviei mensagem para a thread communication e acordei-a\n");

			char *msg_rec;
			msg_rec = cyg_mbox_get(mbInter);
			if(msg_rec[0] == -1) {
				printf("erro transmissão da mensagem\n");
			}
			else {
				for (i=0; i<strlen(msg_rec);i++){
					if(i%8==0 && i!=0){
						printf("\n");
					}
					printf("%d ", (int)msg_rec[i]);
				}
			}
		}
  	}else{
		printf("Erro: Não introduziu parâmetros.");
	}
}
/*-------------------------------------------------------------------------+
| function: cmd_tri - transfererir n registos a partir de il i
+--------------------------------------------------------------------------*/
void cmd_tri(int argc, char** argv) {

	char msg_send[3];
 	unsigned int i;

 	
	msg_send[0]=TRGI;
	if (argc > 1) {
    	if(argc < 3) {
    		printf("Erro: Não introduziu todos os parâmetros.");
	  	}else{
		  	for (i=0; i<(argc-1); i++)
				{unsigned int x; sscanf(argv[i+1], "%x", &x); msg_send[i+1]=(unsigned char)x;}
    //Enviar mensagem para tarefa de Comunicação
    
			cyg_mbox_put(mbCom, &msg_send);
			printf("enviei mensagem para a thread communication e acordei-a\n");

			char *msg_rec;
			msg_rec = cyg_mbox_get(mbInter);
			if(msg_rec[0] == -1) {
				printf("erro transmissão da mensagem\n");
			}
			else {
				for (i=0; i<strlen(msg_rec);i++){
					if(i%8==0 && i!=0){
						printf("\n");
					}
					printf("%d ", (int)msg_rec[i]);
				}
			}
		}
  	}else{
		printf("Erro: Não introduziu parâmetros.");
	}
}
