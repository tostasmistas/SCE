#include "main.h"
#include "threads.h"
#include "threadCommunication_TX.h"
#include "threadCommunication_RX.h"
#include "threadProcessing.h"
#include <cyg/kernel/kapi.h>

cyg_handle_t	processing_clockH, countH;
cyg_alarm 		alarm;
int ptransf = 0;

/*-------------------------------------------------------------------------+
| function: rotina associada a alarme da thread de processamento
+--------------------------------------------------------------------------*/
void alarmecomunicacao(cyg_handle_t alamarH, cyg_addrword_t data){

	char msg_send[2];
	msg_send[0] = TRGC;
	msg_send[1]=  10; //transferir periodicamente 10 registos

	cyg_mutex_lock(&localMemory_mutex); //mutex para bloquear o recurso de escrita na variavel global
	transferRegistos = 2;
	cyg_mutex_unlock(&localMemory_mutex); //desbloquear a escrita na variavel global

	cyg_mbox_put(mbComTX, &msg_send);

	printf("enviei mensagem para a thread Comunicacao e acordei-a\n");
}

/*-------------------------------------------------------------------------+
| function: rotina associada a gestao de alarmes
+--------------------------------------------------------------------------*/
void gestaoalarmes(void){

	int i;
	char msg_send[1];
	msg_send[0] = 1;
		cyg_mutex_lock(&localMemory_mutex);
		for(i=0; i<nr; i++){
			switch(localMemory[i][3]){
				case 3:
					printf("Def. Alarme Relogio - %c:%c:%c\n",localMemory[i][4],localMemory[i][5],localMemory[i][6]);
				break;
				case 4:
					printf("Def. Alarme Temperatura - %c\n",localMemory[i][4]);
				break;
				case 5:
					printf("Def. Alarme Luminosidade - %c\n",localMemory[i][4]);
				break;
				case 6:
					printf("Activacao/Desactivacao de Alarmes - %c:%c:%c\n",localMemory[i][0], localMemory[i][1], localMemory[i][2]);
				break;
			}
		}
		//Enviar OK para Interface
				cyg_mutex_unlock(&localMemory_mutex);
				cyg_mbox_put(mbInter, &msg_send);

				printf("enviei mensagem para a thread Interface e acordei-a\n");

}

/*-------------------------------------------------------------------------+
| function: rotina associada a listagem de alarmes da memoria local
+--------------------------------------------------------------------------*/
void listalarmes(unsigned char* msg_rec, int type){

	char alarmesrel[3] = {0};
	char t1[4] = {0};
	char t2[4] = {0};
	unsigned char alarmetemp = 0;
	unsigned char alarmelum = 0;

	int i = 0;
	int j = 0;

	char msg_send[1] = {0};
	msg_send[0] = 1;

	cyg_mutex_lock(&localMemory_mutex);
	switch(type) {
		case(1):
			printf("alarmes do relogio:\n");
			break;
		case(2):
			printf("alarmes de temperatura:\n");
			break;
		case(3):
			printf("alarmes de luminosidade:\n");
			break;
		default:
			break;
	}

	int nolowbound = 0;
	//verificar se ha instante de tempo t1
	for(i = 0; i <= 2; i++) {
		t1[i] = msg_rec[i+1];
		if(t1[i] == (-1)){
			nolowbound++; //se nao for especificado t1 entao nolowbound == 3
		}
	}

	int nohighbound = 0;
	//verificar se ha instante de tempo t2
	for(i = 3; i <= 5; i++) {
		t2[i-3] = msg_rec[i+1];
		if(t2[i-3] == (-1)){
			nohighbound++; //se nao for especificado t2 entao nohighbound == 3
		}
	}

	if(nolowbound == 0) { // e especificado t1
		if(nohighbound == 3) { //listar alarmes de t1 ate ao fim
			for(i = 0; i < nr; i++) {
				if(localMemory[i][3] == 7) { //código alarmes relógio
					int lowerbound = 0;
					for(j = 0; j < 3; j++) {
						alarmesrel[j] = localMemory[i][j];
						indleitura = i;
						if(alarmesrel[j] > t1[j]) {
							lowerbound++;
						}
					}
					if(lowerbound > 2) { //h,m,s > h1 m1 s1
						alarmetemp = localMemory[i][4];
						alarmelum = localMemory[i][5];
						printf("horas<%c:%c:%c> Temperatura<%c> Luminosidade <%c>\n",alarmesrel[0],alarmesrel[1],alarmesrel[2], alarmetemp, alarmelum);
					}
				}
			}
		}
		else { //listar alarmes de t1 a t2
			for(i = 0;i < nr; i++) {
				if(localMemory[i][3] == 7) { //código alarmes relógio
					int bound = 0;
					for(j = 0; j < 3; j++) {
						alarmesrel[j] = localMemory[i][j];
						indleitura = i;
						if(alarmesrel[j] > t1[j] && alarmesrel[j] < t2[j]){
							bound++;
						}
					}
					if(bound > 3) { //h:m:s > h1:m1:s1 && h:m:s < h2:m2:s2
						alarmetemp = localMemory[i][4];
						alarmelum = localMemory[i][5];
						printf("Horas<%c:%c:%c> Temperatura<%c> Luminosidade <%c>\n",alarmesrel[0],alarmesrel[1],alarmesrel[2], alarmetemp, alarmelum);
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
		printf("Horas<%c:%c:%c> Temperatura<%c> Luminosidade <%c>\n",alarmesrel[0],alarmesrel[1],alarmesrel[2], alarmetemp, alarmelum);
	}

	//Enviar OK para Interface
	cyg_mutex_unlock(&localMemory_mutex);
	cyg_mbox_put(mbInter, &msg_send);
	printf("enviei mensagem para a thread Interface e acordei-a\n");
}

/*-------------------------------------------------------------------------+
| function: rotina associada a informacao geral da memoria localS
+--------------------------------------------------------------------------*/
void informacaogeral(void){

	char msg_send[1];
	int i = 0;
	msg_send[0] = 1;

	cyg_mutex_lock(&localMemory_mutex);
		for(i = 0; i < nr; i++){
			switch(localMemory[i][3]){
				case 1:
					printf("Inicializacaoo: %c:%c:%c - Temp:%c Lum:%c\n", localMemory[i][0],localMemory[i][1],localMemory[i][2],localMemory[i][4],localMemory[i][5]);
					indleitura=i;
					break;
				case 2:
					printf("Nova Hora: %c:%c:%c\n" ,localMemory[i][0],localMemory[i][1],localMemory[i][2]);
					indleitura=i;
					break;
				case 10:
					indleitura=i;
					printf("Periodo de Monitorizacao passou de %c para %c. Hora-%c:%c:%c\n",localMemory[i][4],localMemory[i][5],localMemory[i][0],localMemory[i][1],localMemory[i][2]);
					break;
				case 11:
					indleitura=i;
					printf("Memoria Cheia: NREG=%c nr=%c ie=%c il=%c. Hora-%c:%c:%c\n",localMemory[i][4],localMemory[i][5],localMemory[i][6],localMemory[i][7],localMemory[i][0],localMemory[i][1],localMemory[i][2]);
					break;
				default:
					break;
			}
		}

		//Enviar OK para Interface
				cyg_mutex_unlock(&localMemory_mutex);
				cyg_mbox_put(mbInter, &msg_send);

				printf("enviei mensagem para a thread Interface e acordei-a\n");

}

/*-------------------------------------------------------------------------+
| function: rotina associada a thread de processamento
+--------------------------------------------------------------------------*/
void threadProcessing_func(cyg_addrword_t data) {

	processing_clockH = cyg_real_time_clock();
	cyg_clock_to_counter(processing_clockH, &countH); // contador associado ao alarme
	cyg_alarm_create(countH, &alarmecomunicacao, 0, &alarmProc, &alarm);

	char msg_send;
	unsigned char *msg_rec;

	int ptransfnew = 0;

	for(;;){
		msg_rec = cyg_mbox_get(mbProc);
		switch(msg_rec[0]){
			case CPT: //consultar período de transferência - como contar o tempo?
				msg_send = ptransf;
				cyg_mbox_put(mbInter, &msg_send);
				break;
			case MPT: //modificar período de transferência
				ptransfnew = (int)msg_rec[1];
				if(ptransfnew != 0 && ptransfnew != ptransf){
					ptransf = ptransfnew;
					cyg_alarm_disable(countH);
					cyg_alarm_initialize(countH, cyg_current_time()+ptransf, ptransf);
				}
				if(ptransfnew == 0){
					cyg_alarm_disable(alarmProc);
				}
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
			case TRGC:
				printf("%d\n", (int)msg_rec[0]);
				printf("n de registos: %d\n", (int)msg_rec[1]);
				printf("Recebi TRGC\n");
				cyg_mbox_put(mbComTX, &msg_rec);
				break;
			case TRGI:
				printf("Recebi TRGI\n");
				cyg_mbox_put(mbComTX, &msg_rec);
				break;
			default:
				break;
		}
	}
}
