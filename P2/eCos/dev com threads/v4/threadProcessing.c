#include "main.h"
#include "threads.h"
#include "threadCommunication_TX.h"
#include "threadCommunication_RX.h"
#include "threadProcessing.h"
#include <cyg/kernel/kapi.h>

cyg_handle_t	processing_clockH, countH;
cyg_alarm 		alarm;

void alarmecomunicacao(void){
	char msg_send[8];
	msg_send[0]=TRGC;
		
			cyg_mbox_put(mbComTX, &msg_send);
			
			printf("enviei mensagem para a thread Comunicacao e acordei-a\n");
			cyg_thread_resume(threadCommunicationTX);
}

void gestaoalarmes(void){
	int i;
	char msg_send[2];
	msg_send[0]=1;
		while(cyg_mutex_lock(&localMemory_mutex)!=true);
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
				cyg_thread_resume(threadInterface);
				printf("enviei mensagem para a thread Interface e acordei-a\n");
		
}

void listalarmes(unsigned char* msg_rec, int type){
	char msg_send[2];
	int i=0;
	int j=0;

	char alarmetemp=0;
	char alarmesrel[3];
	char alarmelum=0;

	int noupbound=0;
	int nolowbound=0;
	int bound=0;
	int lowerbound=0;
	char t1[4];
	char t2[4];
	msg_send[0]=1;
	
	while(cyg_mutex_lock(&localMemory_mutex)!=true);
		switch(type){
			case(1):
				printf("Alarmes Relogio\n");
			break;
			case(2):
				printf("Alarmes Temperatura\n");
			break;
			case(3):
				printf("Alarmes Luminosidade\n");
			break;
		}
		
		for(i=0; i<3; i++){
					t1[i]=msg_rec[i+1];
					if(t1[i]==0){
						nolowbound++;
					}
				}
				for(i=3; i<6; i++){
				t2[i-3]=msg_rec[i+1];
					if(t2[i-3]==0){
						noupbound++;
					}
				}
				if(noupbound==2){
					noupbound=0;
					for(i=0; i<nr ; i++){
						if(localMemory[i][3]==7){ //código alarmes relógio
						lowerbound=0;
							for(j=0; j<3; j++){
								alarmesrel[j]=localMemory[i][j];
								indleitura=i;
								if(alarmesrel[j]>t1[j]){
									lowerbound++;
								}
							}
							if(lowerbound>3){ //h,m,s > h1 m1 s1
								alarmetemp=localMemory[i][4];
								alarmelum=localMemory[i][5];
								printf("Horas<%c:%c:%c> Temperatura<%c> Luminosidade <%c>\n",alarmesrel[0],alarmesrel[1],alarmesrel[2], alarmetemp, alarmelum);
							}
						}
					}
				}else{
					if(nolowbound==2){
						nolowbound=0;
						for(i=0; i<nr ; i++){
							if(localMemory[i][3]==7){ //código alarmes relógio
								for(j=0; j<3; j++){
									alarmesrel[j]=localMemory[i][j];
									indleitura=i;
								}
										alarmetemp=localMemory[i][4];
										alarmelum=localMemory[i][5];
										printf("Horas<%c:%c:%c> Temperatura<%c> Luminosidade <%c>\n",alarmesrel[0],alarmesrel[1],alarmesrel[2], alarmetemp, alarmelum);

								}
							}
						}else{
							for(i=0; i<nr ; i++){
								if(localMemory[i][3]==7){ //código alarmes relógio
									bound=0;
									for(j=0; j<3; j++){
										alarmesrel[j]=localMemory[i][j];
										indleitura=i;
										if(alarmesrel[j]>t1[j] && alarmesrel[j]<t2[j]){
											bound++;
										}
									}
									if(bound>3){ 
										alarmetemp=localMemory[i][4];
										alarmelum=localMemory[i][5];
										printf("Horas<%c:%c:%c> Temperatura<%c> Luminosidade <%c>\n",alarmesrel[0],alarmesrel[1],alarmesrel[2], alarmetemp, alarmelum);
									}
								}
							}
						}
				}		
			
			//Enviar OK para Interface
			cyg_mutex_unlock(&localMemory_mutex);
			cyg_mbox_put(mbInter, &msg_send);
			printf("enviei mensagem para a thread Interface e acordei-a\n");
	
}

void informacaogeral(void){
	char msg_send[2];
	int i=0;
	msg_send[0]=1;
	while(cyg_mutex_lock(&localMemory_mutex)!=true);
		for(i=0; i<nr; i++){
			switch(localMemory[i][3]){
			case 1:
				printf("Inicialização: %c:%c:%c - Temp:%c Lum:%c\n", localMemory[i][0],localMemory[i][1],localMemory[i][2],localMemory[i][4],localMemory[i][5]);
				ileitura=i;
			break;
			case 2:
				printf("Nova Hora: %c:%c:%c\n" ,localMemory[i][0],localMemory[i][1],localMemory[i][2]);
				ileitura=i;
			break;
			case 10:
				ileitura=i;
				printf("Período de Monitorização passou de %c para %c. Hora-%c:%c:%c\n",localMemory[i][4],localMemory[i][5],localMemory[i][0],localMemory[i][1],localMemory[i][2]);
			break;
			case 11:
				ileitura=i;
				printf("Memória Cheia: NREG=%c nr=%c ie=%c il=%c. Hora-%c:%c:%c\n",localMemory[i][4],localMemory[i][5],localMemory[i][6],localMemory[i][7],localMemory[i][0],localMemory[i][1],localMemory[i][2]);
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
	int ptransf=0;
	int ptransfnew=0;
	msg_rec = cyg_mbox_get(mbProc);
	switch(msg_rec[0]){
	//consultar período de transferência
	//como contar o tempo?
		case CPT:
			msg_send = (char)ptransf;
			//cyg_mbox_put(mbProcInt, &msg_send);
			break;
	//modificar período de transferência
		case MPT:
			ptransfnew=(int)msg_rec[1];
			if(ptransfnew!=0 && ptransfnew!=ptransf){
				ptransf=ptransfnew;
				cyg_alarm_disable(countH);
				cyg_alarm_initialize(countH,cyg_current_time()+ptransf,ptransf);
			}
			if(ptransfnew==0){
				cyg_alarm_disable(alarmProc);
			}
			break;
	//listar alarmes relógio
		case LAR:
			listalarmes(msg_rec,1);
		break;
	//listar alarmes temperatura
		case LAT:
			listalarmes(msg_rec,2);
		break;
	//listar alarmes luminosidade
		case LAL:
			listalarmes(msg_rec,3);
		break;
		case IGA:
			gestaoalarmes();
		break;
		case IG:
			informacaogeral();
		break;
	}
		
}


