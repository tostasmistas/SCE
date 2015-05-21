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
	cyg_mbox_put(mbComTX, &msg_send);

	printf("enviei mensagem para a thread communication e acordei-a\n");

	unsigned char *msg_rec;
	msg_rec = cyg_mbox_get(mbInter);
	if(msg_rec[1] == CMD_ERRO) {
		printf("erro na transmissao da mensagem\n");
	}
	else {
		printf("horas: %d\n", msg_rec[1]);
		printf("minutos: %d\n", msg_rec[2]);
		printf("segundos: %d\n", msg_rec[3]);
	}
}

/*-------------------------------------------------------------------------+
| function: cmd_ar - acertar relógio
+--------------------------------------------------------------------------*/
void cmd_ar(int argc, char** argv){

	char msg_send[4];
	msg_send[0] = ARLG;

	unsigned int i = 0;
	unsigned char erro = 0;
 	if (argc > 1) {
    	if(argc < 4) {
    		printf("erro: nao introduziu todos os parametros\n");
	  	}
		else{
			for (i = 0; i < (argc-1); i++) {
				unsigned int x; 
				sscanf(argv[i+1], "%d", &x); 
				msg_send[i+1] = (unsigned char)x;
			}
			if(msg_send[1] < 0 || msg_send[1] > 23) {
				erro = 1;
				printf("erro: o valor das horas deve estar entre 0 e 23\n");
			}
			if(msg_send[2] < 0 || msg_send[2] > 59) {
				erro = 1;
				printf("erro: o valor dos minutos deve estar entre 0 e 59\n");
			}
			if(msg_send[3] < 0 || msg_send[3] > 59) {
				erro = 1;
				printf("erro: o valor dos segundos deve estar entre 0 e 59\n");
			}
			if(erro != 1) {
				cyg_mbox_put(mbComTX, &msg_send);

				printf("enviei mensagem para a thread communication e acordei-a\n");

				unsigned char *msg_rec;
				msg_rec = cyg_mbox_get(mbInter);
				if(msg_rec[1] == CMD_ERRO) {
					printf("erro na transmissao da mensagem\n");
				}
				else {
					printf("relogio acertado\n");
				}
			}
		}
  	}
	else{
		printf("erro: nao introduziu parametros\n");
	}

}

/*-------------------------------------------------------------------------+
| function: cmd_ctl - consultar temperatura e luminosidade
+--------------------------------------------------------------------------*/
void cmd_ctl(int argc, char** argv) {

	char msg_send = CTEL;
	cyg_mbox_put(mbComTX, &msg_send);

	printf("enviei mensagem para a thread communication e acordei-a\n");

	unsigned char *msg_rec;
	msg_rec = cyg_mbox_get(mbInter);
	if(msg_rec[1] == CMD_ERRO) {
		printf("erro na transmissao da mensagem\n");
	}
	else {
		printf("temperatura: %d\n", (int)msg_rec[1]);
		printf("luminosidade: %d\n", (int)msg_rec[2]);
	}
}

/*-------------------------------------------------------------------------+
| function: cmd_cp - consultar parâmetros
+--------------------------------------------------------------------------*/
void cmd_cp(int argc, char** argv) {

	char msg_send = CPAR;
	cyg_mbox_put(mbComTX, &msg_send);


	printf("enviei mensagem para a thread communication e acordei-a\n");

	unsigned char *msg_rec;
	msg_rec = cyg_mbox_get(mbInter);
	if(msg_rec[1] == CMD_ERRO) {
		printf("erro na transmissao da mensagem\n");
	}
	else {
		printf("NREG: %d\n", (int)msg_rec[1]);
		printf("PMON: %d\n", (int)msg_rec[2]);
		printf("TSOM: %d\n", (int)msg_rec[3]);
	}
}

/*-------------------------------------------------------------------------+
| function: cmd_mpm - modificar período de monitorização
+--------------------------------------------------------------------------*/
void cmd_mpm(int argc, char** argv) {

	char msg_send[2];
	msg_send[0] = MPMN;

	unsigned char erro = 0;
	if(argc <= 1){
		printf("erro: nao introduziu todos os parametros\n");
	}
	else{
		unsigned int x; 
		sscanf(argv[1], "%d", &x); 
		msg_send[1] = (unsigned char)x;
		if(msg_send[1] < 0 || msg_send[1] > 99) {
			erro = 1;
			printf("erro: o valor de PMON deve estar entre 0 e 99\n");
		}
		if(erro != 1) {
			printf("%d\n", msg_send[1]);
			cyg_mbox_put(mbComTX, &msg_send);


			printf("enviei mensagem para a thread communication e acordei-a\n");

			unsigned char *msg_rec;
			msg_rec = cyg_mbox_get(mbInter);
			if(msg_rec[1] == CMD_ERRO) {
				printf("erro na transmissao da mensagem\n");
			}
			else {
				printf("perido de monoitorizacao modificado\n");
			}
		}
	}
}

/*-------------------------------------------------------------------------+
| function: cmd_ca - consultar alarmes
+--------------------------------------------------------------------------*/
void cmd_ca(int argc, char** argv) {

	char msg_send = CALA;
	cyg_mbox_put(mbComTX, &msg_send);

	printf("enviei mensagem para a thread communication e acordei-a\n");

	unsigned char *msg_rec;
	msg_rec = cyg_mbox_get(mbInter);
	if(msg_rec[1] == CMD_ERRO) {
		printf("erro na transmissao da mensagem\n");
	}
	else {
		printf("alarme das horas: %d\n", (int)msg_rec[1]);
		printf("alarme dos minutos: %d\n", (int)msg_rec[2]);
		printf("alarme dos segundos: %d\n", (int)msg_rec[3]);
		printf("alarme da temperatura: %d\n", (int)msg_rec[4]);
		printf("alarme da luminosidade: %d\n", (int)msg_rec[5]);
		printf("estado dos alarmes: %c\n", msg_rec[6]);
	}
}

/*-------------------------------------------------------------------------+
| function: cmd_dar - definição do alarme do relógio
+--------------------------------------------------------------------------*/
void cmd_dar(int argc, char** argv) {

	char msg_send[4];
	msg_send[0] = DALR;

	unsigned int i = 0;
	unsigned char erro = 0;
	if (argc > 1) {
    	if(argc < 4) {
    		printf("erro: nao introduziu todos os parametros\n");
	  	}
		else{
		  	for (i = 0; i < (argc-1); i++) {
				unsigned int x; 
				sscanf(argv[i+1], "%d", &x); 
				msg_send[i+1] = (unsigned char)x;
			}
			if(msg_send[1] < 0 || msg_send[1] > 23) {
				erro = 1;
				printf("erro: o valor do alarme das horas deve estar entre 0 e 23\n");
			}
			if(msg_send[2] < 0 || msg_send[2] > 59) {
				erro = 1;
				printf("erro: o valor  do alarme dos minutos deve estar entre 0 e 59\n");
			}
			if(msg_send[3] < 0 || msg_send[3] > 59) {
				erro = 1;
				printf("erro: o valor do alarme dos segundos deve estar entre 0 e 59\n");
			}
			if(erro != 1) {
				cyg_mbox_put(mbComTX, &msg_send);

				printf("enviei mensagem para a thread communication e acordei-a\n");

				unsigned char *msg_rec;
				msg_rec = cyg_mbox_get(mbInter);
				if(msg_rec[1] == CMD_ERRO) {
					printf("erro na transmissao da mensagem\n");
				}
				else {
					printf("alarme do relogio acertado\n");
				}
			}
		}
  	}
	else{
		printf("erro: nao introduziu parametros\n");
	}
}

/*-------------------------------------------------------------------------+
| function: cmd_dat - definir alarme temperatura
+--------------------------------------------------------------------------*/
void cmd_dat(int argc, char** argv) {

	char msg_send[2];
	msg_send[0] = DALT;

	unsigned char erro = 0;
	if(argc <= 1){
		printf("erro: nao introduziu todos os parametros\n");
	}
	else{
		unsigned int x; 
		sscanf(argv[1], "%d", &x); 
		msg_send[1] = (unsigned char)x;
		if(msg_send[1] < 0 || msg_send[1] > 50) {
			erro = 1;
			printf("erro: o valor do alarme da temperatura deve estar entre 0 e 50\n");
		}
		if(erro != 1) {
			cyg_mbox_put(mbComTX, &msg_send);

			printf("enviei mensagem para a thread communication e acordei-a\n");

			unsigned char *msg_rec;
			msg_rec = cyg_mbox_get(mbInter);
			if(msg_rec[1] == CMD_ERRO) {
				printf("erro na transmissao da mensagem\n");
			}
			else {
				printf("alarme da temperatura mudado\n");
			}
		}
	}
}

/*-------------------------------------------------------------------------+
| function: cmd_dal - definir alarme luminosidade
+--------------------------------------------------------------------------*/
void cmd_dal(int argc, char** argv) {

	char msg_send[2];
	msg_send[0] = DALL;

	unsigned erro = 0;
	if(argc <= 1) {
		printf("erro: nao introduziu todos os parametros\n");
	}
	else {
		unsigned int x; 
		sscanf(argv[1], "%d", &x); 
		msg_send[1] = (unsigned char)x;
		if(msg_send[1] < 0 || msg_send[1] > 5) {
			erro = 1;
			printf("erro: o valor do alarme da luminosidade deve estar entre 0 e 5\n");
		}
		if(erro == 0) {
			cyg_mbox_put(mbComTX, &msg_send);


			printf("enviei mensagem para a thread communication e acordei-a\n");

			unsigned char *msg_rec;
			msg_rec = cyg_mbox_get(mbInter);
			if(msg_rec[1] == CMD_ERRO) {
				printf("erro na transmissao da mensagem\n");
			}
			else {
				printf("alarme da luminosidade mudado\n");
			}
		}
	}
}

/*-------------------------------------------------------------------------+
| function: cmd_aa - activar/desactivar alarmes
+--------------------------------------------------------------------------*/
void cmd_aa(int argc, char** argv) {

	char msg_send = AALA;
	cyg_mbox_put(mbComTX, &msg_send);


	printf("enviei mensagem para a thread communication e acordei-a\n");

	unsigned char *msg_rec;
	msg_rec = cyg_mbox_get(mbInter);
	if(msg_rec[1] == CMD_ERRO) {
		printf("erro na transmissao da mensagem\n");
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
	cyg_mbox_put(mbComTX, &msg_send);


	printf("enviei mensagem para a thread communication e acordei-a\n");

	unsigned char *msg_rec;
	msg_rec = cyg_mbox_get(mbInter);
	if(msg_rec[1] == CMD_ERRO) {
		printf("erro na transmissao da mensagem\n");
	}
	else {
		printf("NREG: %d\n", (int)msg_rec[1]);
		printf("nr: %d\n", (int)msg_rec[2]);
		printf("ie: %d\n", (int)msg_rec[3]);
		printf("il: %d\n", (int)msg_rec[4]);
	}
}

/*-------------------------------------------------------------------------+
| function: cmd_trc - transfererir n registos a partir de il corrente
+--------------------------------------------------------------------------*/
void cmd_trc(int argc, char** argv) {

	char msg_send[2];
	msg_send[0] = TRGC;

	cyg_mutex_lock(&transferRegistos_mutex); //mutex para bloquear o recurso de escrita na variavel global
	transferRegistos = 1;
	cyg_mutex_unlock(&transferRegistos_mutex); //desbloquear a escrita na variavel global

	if (argc > 1) { 
		unsigned int x; 
		sscanf(argv[1], "%d", &x); 
		msg_send[1] = (unsigned char)x;

		cyg_mbox_put(mbComTX, &msg_send);

		printf("enviei mensagem para a thread communication e acordei-a\n");

		unsigned char *msg_rec;
		msg_rec = cyg_mbox_get(mbInter);
		if(msg_rec[0] == CMD_OK) {
			printf("transferencia concluida com sucesso\n");
		}
		else {
			printf("erro na transmissao da mensagem\n");
		}
	}
	else{
		printf("erro: nao introduziu parametros\n");
	}
}

/*-------------------------------------------------------------------------+
| function: cmd_tri - transfererir n registos a partir de il i
+--------------------------------------------------------------------------*/
void cmd_tri(int argc, char** argv) {

	char msg_send[3];
	msg_send[0] = TRGI;

	cyg_mutex_lock(&transferRegistos_mutex); //mutex para bloquear o recurso de escrita na variavel global
	transferRegistos = 1;
	cyg_mutex_unlock(&transferRegistos_mutex); //desbloquear a escrita na variavel global

	unsigned int i = 0;
	if (argc > 1) {
		if(argc < 3) {
			printf("erro: nao introduziu todos os parametros\n");
			}
		else {
		  	for (i = 0; i < (argc-1); i++) {
				unsigned int x; 
				sscanf(argv[i+1], "%d", &x); 
				msg_send[i+1] = (unsigned char)x;
			}

			cyg_mbox_put(mbComTX, &msg_send);

			printf("enviei mensagem para a thread communication e acordei-a\n");

			unsigned char *msg_rec;
			msg_rec = cyg_mbox_get(mbInter);
			if(msg_rec[1] == CMD_OK) {
				printf("transferencia concluida com sucesso\n");
			}
			else {
				printf("erro na transmissao da mensagem\n");
			}
		}
	}
	else {
		printf("erro: nao introduziu parametros\n");
	}

}

/*-------------------------------------------------------------------------+
| function: cmd_irl - informação registos locais(NRBUF,nr,iescrita,ileitura)
+--------------------------------------------------------------------------*/
void cmd_irl(int argc, char** argv) {

	printf("informacao dos registos locais:\n");
	printf("NRBUF = %d\n", NRBUF);
	printf("NR = %d\n", nr);
	printf("indice escrita = %d\n", indescrita);
	printf("indice leitura = %d\n", indleitura);

}

/*-------------------------------------------------------------------------+
| function: cmd_lr - listar n registos a partir do índice i
+--------------------------------------------------------------------------*/
void cmd_lr(int argc, char** argv) {

	char horas[3] = {0};
	char parametros[4] = {0};
	char codigoev = 0;
	unsigned int numreg = 0;
	unsigned int indice = 0;
	int i = 0;
	int j = 0;

	if(argc > 1){
		if(argc < 3){
			printf("erro: nao introduziu todos os parametros\n");
		}
		else{
			cyg_mutex_lock(&localMemory_mutex);
				sscanf(argv[1], "%d", &numreg); 
				sscanf(argv[2], "%d", &indice);
				for(i = indice; i < numreg; i++) {
					for(j = 0; j < 3; j++) {
						horas[j] = localMemory[i][j];
						indleitura = i;
					}
					codigoev = localMemory[i][3];
					for(j = 4; j < 8; j++){
						parametros[j-4] = localMemory[i][j];
					}
					printf("registo %d:\n", i);
					printf("\thoras do evento: %d:%d:%d\n", horas[0], horas[1], horas[2]);
					printf("\tcodigo do evento: %d\n", codigoev);	
					switch(codigoev) {
						case 1:
							printf("\ttemperatura: %d\n", parametros[0]);
							printf("\tluminosidade: %d\n", parametros[1]);
							break; 
						case 2:
							printf("\tnova hora: %d:%d:%d\n", parametros[0], parametros[1], parametros[2]);
							break;
						case 3:
							printf("\thora do alarme: %d:%d:%d\n", parametros[0], parametros[1], parametros[2]);
							break;
						case 4:
							printf("\tlimiar temperatura: %d\n", parametros[0]);
							break;
						case 5:
							printf("\tlimiar luminosidade: %d\n", parametros[0]);
							break;
						case 6:
							printf("\ttemperatura: %d\n", parametros[0]);
							printf("\tluminosidade: %d\n", parametros[1]);
							break;
						case 7:
							printf("\ttemperatura: %d\n", parametros[0]);
							printf("\tluminosidade: %d\n", parametros[1]);
							break;
						case 8:
							printf("\ttemperatura: %d\n", parametros[0]);
							printf("\tluminosidade: %d\n", parametros[1]);
							break;
						case 9:
							printf("\ttemperatura: %d\n", parametros[0]);
							printf("\tluminosidade: %d\n", parametros[1]);
							break;
						case 10:
							printf("\tantigo valor: %d\n", parametros[0]);
							printf("\tnovo valor: %d\n", parametros[1]);
							break;
						case 11:
							printf("\tNREG: %d\n", parametros[0]);
							printf("\tnr: %d\n", parametros[1]);
							printf("\tie: %d\n", parametros[2]);
							printf("\til: %d\n", parametros[3]);
							break;
						default:
							break;
						}
					}
			cyg_mutex_unlock(&localMemory_mutex);
		}
	}
	else {
		printf("erro: nao introduziu parametros\n");
	}
}

/*-------------------------------------------------------------------------+
| function: cmd_er - eliminar registos locais
+--------------------------------------------------------------------------*/
void cmd_er(int argc, char** argv) {

	int i;
	cyg_mutex_lock(&localMemory_mutex);
		for(i = 0;i < NRBUF; i++){
			localMemory[i] = 0;
		}
		indescrita = 0;
		indleitura = 0;
		nr = 0;
		printf("registos eliminados\n");
	cyg_mutex_unlock(&localMemory_mutex);
}

/*-------------------------------------------------------------------------+
| function: cmd_cpt - consultar período de transferência
+--------------------------------------------------------------------------*/
void cmd_cpt(int argc, char** argv) {

	char msg_send;
	msg_send = CPT;

	cyg_mbox_put(mbProc, &msg_send);

	printf("enviei mensagem para a thread processing e acordei-a\n");

	unsigned char *msg_rec;
	msg_rec = cyg_mbox_get(mbInter);
	if(msg_rec[0] == CMD_ERRO) {
		printf("erro na transmissao da mensagem\n");
	}
	else {
		printf("periodo de transferencia: %d\n", msg_rec[0]); //verificar se recebe array ou so um byte
	}
}

/*-------------------------------------------------------------------------+
| function: cmd_mpt - modificar período de transferência
+--------------------------------------------------------------------------*/
void cmd_mpt(int argc, char** argv) {

	char msg_send[2];
	msg_send[0] = MPT;

	if(argc > 1){
		unsigned int x; 
		sscanf(argv[1], "%d", &x); 
		msg_send[1] = (unsigned char)x;	

		cyg_mbox_put(mbProc, &msg_send);

		printf("enviei mensagem para a thread processing e acordei-a\n");

		unsigned char *msg_rec;
		msg_rec = cyg_mbox_get(mbInter);
		if(msg_rec[0] == CMD_ERRO) {
			printf("erro na transmissao da mensagem\n");
		}
		else {
			printf("periodo de transferencia modificado\n");
		}
	}
	else{
		printf("erro: nao introduziu todos os parametros\n");
	}
}

/*-------------------------------------------------------------------------+
| function: cmd_lar - listar alarmes de relógio
+--------------------------------------------------------------------------*/
void cmd_lar(int argc, char** argv) {

	unsigned char msg_send[7] = {255}; 
	unsigned int i = 0;
	for(i = 0; i < 7; i++) {
		msg_send[i] = 255;
	}
	msg_send[0] = LAR;

	if (argc > 1) { // sao especificados instantes de tempo
		for (i = 0; i < (argc-1); i++) {
			unsigned int x; 
			sscanf(argv[i+1], "%d", &x); 
			msg_send[i+1] = (unsigned char)x;
			//as restantes posicoes do array continuam a -1, para que depois se possa identificar que so se especificou t1
		}
	}

	// se nao forem especificados instantes de tempo as restantes posicoes do array tirando a de CMD permanecem a -1

	cyg_mbox_put(mbProc, &msg_send);

	printf("enviei mensagem para a thread processing e acordei-a\n");

	unsigned char *msg_rec;
	msg_rec = cyg_mbox_get(mbInter);
	if(msg_rec[1] == CMD_ERRO) {
		printf("erro na transmissao da mensagem\n");
	}
	else {
		printf("todos os registos pedidos do alarme do relogio foram listados\n");
	}
}

/*-------------------------------------------------------------------------+
| function: cmd_lat - listar alarmes de temperatura
+--------------------------------------------------------------------------*/
void cmd_lat(int argc, char** argv) {

	unsigned char msg_send[7] = {255}; 
	unsigned int i = 0;
	for(i = 0; i < 7; i++) {
		msg_send[i] = 255;
	}
	msg_send[0] = LAT;

	if (argc > 1) { // sao especificados instantes de tempo
		for (i = 0; i < (argc-1); i++) {
			unsigned int x; 
			sscanf(argv[i+1], "%d", &x); 
			msg_send[i+1] = (unsigned char)x;
			//as restantes posicoes do array continuam a -1, para que depois se possa identificar que so se especificou t1
		}
	}

	// se nao forem especificados instantes de tempo as restantes posicoes do array tirando a de CMD permanecem a -1

	cyg_mbox_put(mbProc, &msg_send);

	printf("enviei mensagem para a thread processing e acordei-a\n");

	unsigned char *msg_rec;
	msg_rec = cyg_mbox_get(mbInter);
	if(msg_rec[1] == CMD_ERRO) {
		printf("erro transmissão da mensagem\n");
	}
	else {
		printf("todos os registos pedidos do alarme da temperatura foram listados\n");
	}
}

/*-------------------------------------------------------------------------+
| function: cmd_lal - listar alarmes de luminosidade
+--------------------------------------------------------------------------*/
void cmd_lal(int argc, char** argv) {

	unsigned char msg_send[7] = {255}; 
	unsigned int i = 0;
	for(i = 0; i < 7; i++) {
		msg_send[i] = 255;
	}
	msg_send[0] = LAL;

	if (argc > 1) { // sao especificados instantes de tempo
		if(argc == 3) { // apenas se especifica o instante de tempo t1
			for (i = 0; i <= 2; i++) {
				{unsigned int x; sscanf(argv[i+1], "%d", &x); msg_send[i+1]=(unsigned char)x;}
				//as restantes posicoes do array continuam a -1, para que depois se possa identificar que so se especificou t1
			}
		}
		else if(argc == 6) { // especifica-se t1 e t2
			for (i = 0; i <= 6; i++) {
				{unsigned int x; sscanf(argv[i+1], "%d", &x); msg_send[i+1]=(unsigned char)x;}
			}
		}
	}

	cyg_mbox_put(mbProc, &msg_send);

	printf("enviei mensagem para a thread processing e acordei-a\n");

	unsigned char *msg_rec;
	msg_rec = cyg_mbox_get(mbInter);
	if(msg_rec[1] == CMD_ERRO) {
		printf("erro na transmissao da mensagem\n");
	}
	else {
		printf("todos os registos pedidos do alarme da luminosidade foram listados\n");
	}
}

/*-------------------------------------------------------------------------+
| function: cmd_iga - listar informação gestão alarmes
+--------------------------------------------------------------------------*/
void cmd_iga(int argc, char** argv) {

	char msg_send[1];
	msg_send[0] = IGA;

	cyg_mbox_put(mbProc, &msg_send);

	printf("enviei mensagem para a thread processing e acordei-a\n");

	unsigned char *msg_rec;
	msg_rec = cyg_mbox_get(mbInter);
	if(msg_rec[1] == CMD_ERRO) {
		printf("erro na transmissao da mensagem\n");
	}
	else {
		printf("todos os registos da gestao de alarme foram listados\n");
	}
}

/*-------------------------------------------------------------------------+
| function: cmd_ig - listar informação geral
+--------------------------------------------------------------------------*/
void cmd_ig(int argc, char** argv) {

	char msg_send[1];
	msg_send[0] = IG;

	cyg_mbox_put(mbProc, &msg_send);

	printf("enviei mensagem para a thread processing e acordei-a\n");

	unsigned char *msg_rec;
	msg_rec = cyg_mbox_get(mbInter);
	if(msg_rec[1] == CMD_ERRO) {
		printf("erro na transmissao da mensagem\n");
	}
	else {
		printf("todos os registos de informacao geral foram listados\n");
	}
}
