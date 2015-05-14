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

	unsigned char *msg_rec;
	msg_rec = cyg_mbox_get(mbInter);
	if(msg_rec[0] == CMD_ERRO) {
		printf("erro transmissão da mensagem\n");
	}
	else {
		printf("horas: %d\n", msg_rec[0]);
		printf("minutos: %d\n", msg_rec[1]);
		printf("segundos: %d\n", msg_rec[2]);
	}
}
/*-------------------------------------------------------------------------+
| function: cmd_ar - acertar relógio
+--------------------------------------------------------------------------*/
void cmd_ar(int argc, char** argv){

	char msg_send[4];
 	unsigned int i = 0;

	msg_send[0] = ARLG;
 	if (argc > 1) {
    	if(argc < 4) {
    		printf("erro: não introduziu todos os parâmetros");
	  	}
		else{
			for (i = 0; i < (argc-1); i++) {
				{unsigned int x; sscanf(argv[i+1], "%d", &x); msg_send[i+1]=(unsigned char)x;}
			}			
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
  	}
	else{
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

/*-------------------------------------------------------------------------+
| function: cmd_irl - informação registos locais(NRBUF,nr,iescrita,ileitura)
+--------------------------------------------------------------------------*/
void cmd_irl(int argc, char** argv) {

	printf("Informação Registos Locais\n");
	printf("NRBUF=%d\n",NRBUF);
	printf("NR=%d\n",nr);
	printf("Índice Escrita=%d\n",indescrita);
	printf("Índice Leitura=%d\n",indleitura);

}

/*-------------------------------------------------------------------------+
| function: cmd_lr - listar n registos a partir do índice i
+--------------------------------------------------------------------------*/
void cmd_lr(int argc, char** argv) {

	char horas[4];
	char parametros[6];
	char codigoev;
	int i = 0;
	int j = 0;
	
	if(argc>1){
		if(argc<3){
			printf("Não introduziu todos os parâmetros.\n");
			}else{
				if(cyg_mutex_lock(&mem_lock)==true){
				//numreg=argc[1];
				//indice=argc[2];
				for(i=(int)argv[2];i<(int)argv[1];i++){
					for(j=0; j<3;j++){
						horas[j]=localmemory[i][j];
					}
					codigoev=localmemory[i][3];
					for(j=4;j<8;j++){
						parametros[j-4]=localmemory[i][j];
					}
					printf("Registo[%d]: Horas-%c:%c:%c Código-%c Parâmetros-%c,%c,%c,%c\n",i,horas[0],horas[1],horas[2],codigoev,parametros[4],parametros[5],parametros[6],parametros[7]);
				}
				cyg_mutex_unlock(&mem_lock);
				}

			}
	}
}

/*-------------------------------------------------------------------------+
| function: cmd_er - eliminar registos locais
+--------------------------------------------------------------------------*/
void cmd_er(int argc, char** argv) {
	int i;
	if(cyg_mutex_lock(&mem_lock)==true){
		for(i=0;i<NRBUF;i++){
			localmemory[i]=0;
		}

		indescrita=0;
		indleitura=0;
		nr=0;
		printf("Registos eliminados\n");
	}
	cyg_mutex_unlock(&mem_lock);
}

/*-------------------------------------------------------------------------+
| function: cmd_cpt - consultar período de transferência
+--------------------------------------------------------------------------*/
void cmd_cpt(int argc, char** argv) {

	char msg_send[2];
	msg_send[0]=CPT;

	//Enviar mensagem para tarefa de Processamento

			cyg_mbox_put(mbIntProc, &msg_send);
			printf("enviei mensagem para a thread Processing e acordei-a\n");

			char *msg_rec;
			msg_rec = cyg_mbox_get(mbProcInt);
			if(msg_rec[0] == -1) {
				printf("erro transmissão da mensagem\n");
			}
			else {
				printf("Período de Transferência: %c\n", msg_rec[0]);
			}

}

/*-------------------------------------------------------------------------+
| function: cmd_mpt - modificar período de transferência
+--------------------------------------------------------------------------*/
void cmd_mpt(int argc, char** argv) {

	char msg_send[3];
	msg_send[0]=MPT;

	if(argc>1){
		msg_send[1]=(int)argv[1];
		
		//Enviar mensagem para tarefa de Processamento
			cyg_mbox_put(mbIntProc, &msg_send);
			printf("enviei mensagem para a thread Processing e acordei-a\n");

	}else{
		printf("Não introduziu todos os parâmetros.\n");
	}
}

/*-------------------------------------------------------------------------+
| function: cmd_lar - listar alarmes de relógio
+--------------------------------------------------------------------------*/
void cmd_lar(int argc, char** argv) {
	char msg_send[8];
	msg_send[0]=LAR;
	int i = 0;
	int j = 0;
		for(i=1; i<argc; i++){
			msg_send[i]=(int)argv[i];
			j=i;
		}
		while(j<argc){
			msg_send[j]=0;
			j++;
		}

		//Enviar mensagem para tarefa de Processamento
			cyg_mbox_put(mbIntProc, &msg_send);
			printf("enviei mensagem para a thread Processing e acordei-a\n");

			char *msg_rec;
			msg_rec = cyg_mbox_get(mbProcInt);
			if(msg_rec[0] == -1) {
				printf("erro transmissão da mensagem\n");
			}
			else {
				printf("Todos os registos listados");
			}
}

/*-------------------------------------------------------------------------+
| function: cmd_lat - listar alarmes de temperatura
+--------------------------------------------------------------------------*/
void cmd_lat(int argc, char** argv) {
	char msg_send[8];
	msg_send[0]=LAT;
	int i = 0;
	int j = 0;
		for(i=1; i<argc; i++){
			msg_send[i]=(int)argv[i];
			j=i;
		}
		while(j<argc){
			msg_send[j]=0;
			j++;
		}
		//Enviar mensagem para tarefa de Processamento
			cyg_mbox_put(mbIntProc, &msg_send);
			printf("enviei mensagem para a thread Processing e acordei-a\n");

			char *msg_rec;
			msg_rec = cyg_mbox_get(mbProcInt);
			if(msg_rec[0] == -1) {
				printf("erro transmissão da mensagem\n");
			}
			else {
				printf("Todos os registos listados");
			}
}

/*-------------------------------------------------------------------------+
| function: cmd_lal - listar alarmes de luminosidade
+--------------------------------------------------------------------------*/
void cmd_lal(int argc, char** argv) {
	char msg_send[8];
	msg_send[0]=LAL;
	int i = 0;
	int j = 0;
		for(i=1; i<argc; i++){
			msg_send[i]=(int)argv[i];
			j=i;
		}
		while(j<argc){
			msg_send[j]=0;
			j++;
		}
		//Enviar mensagem para tarefa de Processamento
			cyg_mbox_put(mbIntProc, &msg_send);
			printf("enviei mensagem para a thread Processing e acordei-a\n");

			char *msg_rec;
			msg_rec = cyg_mbox_get(mbProcInt);
			if(msg_rec[0] == -1) {
				printf("erro transmissão da mensagem\n");
			}
			else {
				printf("Todos os registos listados");
			}

}

/*-------------------------------------------------------------------------+
| function: cmd_iga - listar informação gestão alarmes
+--------------------------------------------------------------------------*/
void cmd_iga(int argc, char** argv) {
	char msg_send[2];
	msg_send[0]=IGA;

		//Enviar mensagem para tarefa de Processamento
			cyg_mbox_put(mbIntProc, &msg_send);
			printf("enviei mensagem para a thread Processing e acordei-a\n");

			char *msg_rec;
			msg_rec = cyg_mbox_get(mbProcInt);
			if(msg_rec[0] == -1) {
				printf("erro transmissão da mensagem\n");
			}
			else {
				printf("Todos os registos listados");
			}
}

/*-------------------------------------------------------------------------+
| function: cmd_ig - listar informação geral
+--------------------------------------------------------------------------*/
void cmd_ig(int argc, char** argv) {
	char msg_send[2];
	msg_send[0]=IG;

		//Enviar mensagem para tarefa de Processamento
			cyg_mbox_put(mbIntProc, &msg_send);
			printf("enviei mensagem para a thread Processing e acordei-a\n");

			char *msg_rec;
			msg_rec = cyg_mbox_get(mbProcInt);
			if(msg_rec[0] == -1) {
				printf("erro transmissão da mensagem\n");
			}
			else {
				printf("Todos os registos listados");
			}
}
