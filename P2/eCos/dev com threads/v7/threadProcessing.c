#include "main.h"
#include "threads.h"
#include "threadCommunication_TX.h"
#include "threadCommunication_RX.h"
#include "threadProcessing.h"
#include <cyg/kernel/kapi.h>

cyg_handle_t	processing_clockH, countH; // handles for the counters
cyg_handle_t 	alarmProcH; // handles for the alarms
cyg_alarm 		alarmProc;

int ptransf = 0;

/*-------------------------------------------------------------------------+
| function: rotina associada a alarme da thread de processamento
+--------------------------------------------------------------------------*/
void alarme_func(cyg_handle_t alarmH, cyg_addrword_t data){

	char msg_send[2];
	msg_send[0] = TRGC;
	msg_send[1]=  10; //transferir periodicamente 10 registos

	cyg_mutex_lock(&transferRegistos_mutex); //mutex para bloquear o recurso de escrita na variavel global
	transferRegistos = 2;
	cyg_mutex_unlock(&transferRegistos_mutex); //desbloquear a escrita na variavel global

	cyg_mbox_put(mbComTX, &msg_send);

	printf("enviei mensagem para a thread Comunicacao e acordei-a\n");
}

/*-------------------------------------------------------------------------+
| function: rotina associada a gestao de alarmes
+--------------------------------------------------------------------------*/
void gestaoalarmes(void){

	int i = 0;
		
	cyg_mutex_lock(&localMemory_mutex);
	for(i = 0; i < nr; i++){
		switch(localMemory[i][3]) {
			case 3:
				printf("def. alarme relogio: %d:%d:%d\n",
						localMemory[i][4], localMemory[i][5], localMemory[i][6]);
				break;
			case 4:
				printf("def. alarme temperatura: %d\n", localMemory[i][4]);
				break;
			case 5:
				printf("def. alarme luminosidade: %d\n",localMemory[i][4]);
				break;
			case 6:
				printf("activacao/desactivacao de alarmes: %d:%d:%d\n",
						localMemory[i][0], localMemory[i][1], localMemory[i][2]);
				break;
		}
	}
	cyg_mutex_unlock(&localMemory_mutex);
	
	char msg_send = CMD_OK;
	cyg_mbox_put(mbInter, &msg_send);

	printf("enviei mensagem para a thread Interface e acordei-a\n");
}

/*-------------------------------------------------------------------------+
| function: rotina associada a listagem de alarmes da memoria local
+--------------------------------------------------------------------------*/
void listalarmes(unsigned char* msg_rec, int type){

	char alarmesrel[3] = {0};
	unsigned char t1[3] = {0};
	unsigned char t2[3] = {0};
	unsigned char alarmetemp = 0;
	unsigned char alarmelum = 0;

	int i = 0;
	int j = 0;

	cyg_mutex_lock(&localMemory_mutex);
	switch(type) {
		case 1:
			printf("alarmes do relogio:\n");
			break;
		case 2:
			printf("alarmes de temperatura:\n");
			break;
		case 3:
			printf("alarmes de luminosidade:\n");
			break;
		default:
			break;
	}

	printf("t1: ");
	int nolowbound = 0;
	//verificar se ha instante de tempo t1
	for(i = 0; i <= 2; i++) {
		t1[i] = (char)msg_rec[i+1];
		printf("%d:", t1[i]);
		if(t1[i] == 255){
			nolowbound++; //se nao for especificado t1 entao nolowbound == 3
		}
	}

	printf("t2: ");
	int nohighbound = 0;
	//verificar se ha instante de tempo t2
	for(i = 3; i <= 5; i++) {
		t2[i-3] = (char)msg_rec[i+1];
		printf("%d:", t2[i-3]);
		if(t2[i-3] == 255){
			nohighbound++; //se nao for especificado t2 entao nohighbound == 3
		}
	}

	if(nolowbound == 0) { // e especificado t1
		printf("low bound\n");
		if(nohighbound == 3) { //listar alarmes de t1 ate ao fim
			printf("no high bound\n");
			for(i = 0; i < nr; i++) {
				if(localMemory[i][3] == 7 || localMemory[i][3] == 8 || localMemory[i][3] == 9 ){ //código alarmes
					int lowerbound = 0;
					for(j = 0; j < 3; j++) {
						alarmesrel[j] = localMemory[i][j];
						indleitura = i;
						if(alarmesrel[j] > t1[j]) {
							lowerbound++;
						}
					}
					alarmetemp = localMemory[i][4];
					alarmelum = localMemory[i][5];
					if(lowerbound > 2) { //h,m,s > h1 m1 s1
						switch(type) {
							case 1:
								printf("\thoras do evento: %d:%d:%d\n", alarmesrel[0], alarmesrel[1], alarmesrel[2]);
								printf("\ttemperatura: %d\n", alarmetemp);
								printf("\tluminosidade: %d\n", alarmelum);
								break;
							case 2:
								printf("\thoras do evento: %d:%d:%d\n", alarmesrel[0], alarmesrel[1], alarmesrel[2]);
								printf("\ttemperatura: %d\n", alarmetemp);
								printf("\tluminosidade: %d\n", alarmelum);
								break;
							case 3:
								printf("\thoras do evento: %d:%d:%d\n", alarmesrel[0], alarmesrel[1], alarmesrel[2]);
								printf("\ttemperatura: %d\n", alarmetemp);
								printf("\tluminosidade: %d\n", alarmelum);
								break;
							default:
								break;
						}
					}
				}
			}
		}
		else { //listar alarmes de t1 a t2
			printf("low bound and high bound\n");
			for(i = 0;i < nr; i++) {
				if(localMemory[i][3] == 7 || localMemory[i][3] == 8 || localMemory[i][3] == 9 ) { //código alarmes
					int bound = 0;
					for(j = 0; j < 3; j++) {
						alarmesrel[j] = localMemory[i][j];
						indleitura = i;
						if(alarmesrel[j] > t1[j] && alarmesrel[j] < t2[j]){
							bound++;
						}
					}
					
					alarmetemp = localMemory[i][4];
					alarmelum = localMemory[i][5];
					
					if(bound > 3) { //h:m:s > h1:m1:s1 && h:m:s < h2:m2:s2					
						switch(type) {
							case 1:
								printf("\thoras do evento: %d:%d:%d\n", alarmesrel[0], alarmesrel[1], alarmesrel[2]);
								printf("\ttemperatura: %d\n", alarmetemp);
								printf("\tluminosidade: %d\n", alarmelum);
								break;
							case 2:
								printf("\thoras do evento: %d:%d:%d\n", alarmesrel[0], alarmesrel[1], alarmesrel[2]);
								printf("\ttemperatura: %d\n", alarmetemp);
								printf("\tluminosidade: %d\n", alarmelum);
								break;
							case 3:
								printf("\thoras do evento: %d:%d:%d\n", alarmesrel[0], alarmesrel[1], alarmesrel[2]);
								printf("\ttemperatura: %d\n", alarmetemp);
								printf("\tluminosidade: %d\n", alarmelum);
								break;
							default:
								break;
						}
					}
				}
			}
		}
	}
	else { //nao e especificado t1 e entao pretende-se listar todos os alarmes
		for(j = 0; j < 3; j++){
			alarmesrel[j] = localMemory[i][j];
			indleitura=i;
		}
		alarmetemp=localMemory[i][4];
		alarmelum=localMemory[i][5];
		switch(type) {
			case 1:
				printf("\thoras do evento: %d:%d:%d\n", alarmesrel[0], alarmesrel[1], alarmesrel[2]);
				printf("\ttemperatura: %d\n", alarmetemp);
				printf("\tluminosidade: %d\n", alarmelum);
				break;
			case 2:
				printf("\thoras do evento: %d:%d:%d\n", alarmesrel[0], alarmesrel[1], alarmesrel[2]);
				printf("\ttemperatura: %d\n", alarmetemp);
				printf("\tluminosidade: %d\n", alarmelum);
				break;
			case 3:
				printf("\thoras do evento: %d:%d:%d\n", alarmesrel[0], alarmesrel[1], alarmesrel[2]);
				printf("\ttemperatura: %d\n", alarmetemp);
				printf("\tluminosidade: %d\n", alarmelum);
				break;
			default:
				break;
		}
	}

	cyg_mutex_unlock(&localMemory_mutex);
	char msg_send = CMD_OK;
	cyg_mbox_put(mbInter, &msg_send);
	printf("enviei mensagem para a thread Interface e acordei-a\n");
}

/*-------------------------------------------------------------------------+
| function: rotina associada a informacao geral da memoria local
+--------------------------------------------------------------------------*/
void informacaogeral(void){

	int i = 0;
	
	cyg_mutex_lock(&localMemory_mutex);
	for(i = 0; i < nr; i++){
		switch(localMemory[i][3]){
			case 1:
				indleitura = i;
				printf("inicio de execucao\n:");
				printf("\thoras do evento: %d:%d:%d\n", localMemory[i][0], localMemory[i][1], localMemory[i][2]);
				printf("\ttemperatura: %d\n", localMemory[i][4]);
				printf("\tluminosidade: %d\n", localMemory[i][5]);
				break;
			case 2:
				indleitura = i;
				printf("acerto do relogio\n:");
				printf("\thoras do evento: %d:%d:%d\n", localMemory[i][0], localMemory[i][1], localMemory[i][2]);
				printf("\tnova hora: %d:%d:%d\n", localMemory[i][4], localMemory[i][5], localMemory[i][6]);
				break;
			case 10:
				indleitura = i;
				printf("modificacao de periodo de monitorizacao\n:");
				printf("\thoras do evento: %d:%d:%d\n", localMemory[i][0], localMemory[i][1], localMemory[i][2]);
				printf("\tantigo valor: %d\n", localMemory[i][4]);
				printf("\tnovo valor: %d\n", localMemory[i][5]);
				break;
			case 11:
				indleitura = i;
				printf("notificacao de memoria cheia\n:");
				printf("\thoras do evento: %d:%d:%d\n", localMemory[i][0], localMemory[i][1], localMemory[i][2]);
				printf("\tNREG: %d\n", localMemory[i][4]);
				printf("\tnr: %d\n", localMemory[i][5]);
				printf("\tie: %d\n", localMemory[i][6]);
				printf("\til: %d\n", localMemory[i][7]);
				break;
			default:
				break;
		}
	}
	cyg_mutex_unlock(&localMemory_mutex);
				
	char msg_send = CMD_OK;
	cyg_mbox_put(mbInter, &msg_send);

	printf("enviei mensagem para a thread Interface e acordei-a\n");
}

/*-------------------------------------------------------------------------+
| function: rotina associada a thread de processamento
+--------------------------------------------------------------------------*/
void threadProcessing_func(cyg_addrword_t data) {

	processing_clockH = cyg_real_time_clock();
	cyg_clock_to_counter(processing_clockH, &countH); // contador associado ao alarme
	cyg_alarm_create(countH, alarme_func, (cyg_addrword_t) 0, &alarmProcH, &alarmProc);

	int ptransfnew = 0;

	for(;;){
		unsigned char *msg_rec;
		char msg_send;
		msg_rec = cyg_mbox_get(mbProc);
		switch(msg_rec[0]){
			case CPT: //consultar período de transferência - como contar o tempo?
				msg_send = ptransf;
				cyg_mbox_put(mbInter, &msg_send);
				break;
			case MPT: //modificar período de transferência
				ptransfnew = (int)msg_rec[1];
				printf("recebi mpt - novo ptransf = %d\n", ptransfnew);
				if(ptransfnew != 0 && ptransfnew != ptransf){
					ptransf = ptransfnew;
					printf("antes do Inicializacaoo\n");
					cyg_alarm_initialize(alarmProcH, cyg_current_time()+ptransf, ptransf);
					printf("depois de Inicializacaoo\n");
				}
				if(ptransfnew == 0){
					cyg_alarm_disable(alarmProcH);
				}
				char msg_send = CMD_OK;
				cyg_mbox_put(mbInter, &msg_send);
				break;
			case LAR: //listar alarmes relógio
				listalarmes(msg_rec, 1);
				break;
			case LAT: //listar alarmes temperatura
				listalarmes(msg_rec, 2);
				break;
			case LAL: //listar alarmes luminosidade
				listalarmes(msg_rec, 3);
				break;
			case IGA:
				gestaoalarmes();
				break;
			case IG:
				informacaogeral();
				break;
			default:
				break;
		}
	}
}
