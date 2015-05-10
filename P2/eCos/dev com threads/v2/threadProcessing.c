#include "main.h"
#include "threads.h"
#include "threadCommunication.h"
#include "threadProcessing.h"

unsigned char *msg_rec;
int bound=0;
int lowerbound=0;
int i=0;
int j=0;
int noupbound=0;
int nolowbound=0;
int ptransf;
char alarmetemp=0;
char alarmesrel[3];
char alarmelum=0;


/*-------------------------------------------------------------------------+
| function: rotina associada a thread de processamento
+--------------------------------------------------------------------------*/
void threadProcessing_func(cyg_addrword_t data) {
	msg_rec = cyg_mbox_get(mbIntProc);
	switch(msg_rec[0]){
	//consultar período de transferência
	//como contar o tempo?
		case CPT:
			char msg_send = (char)ptransf;
			cyg_mbox_put(mbProcInt, &msg_send);
			break;
	//modificar período de transferência
		case MPT:
			ptransf=(int)msg_rec[1];
			break;
	//listar alarmes relógio
		case LAR:
			listaalarmes(msg_rec,1);
		break;
	//listar alarmes temperatura
		case LAT:
			listaalarmes(msg_rec,2);
		break;
	//listar alarmes luminosidade
		case LAL:
			listaalarmes(msg_rec,3);
		break;
		case IGA:
			gestaoalarmes();
		break;
		case IG:
			//Não percebo o que é para fazer aqui????
			//infogeral();
		break;
	}
		
}

void gestaoalarmes(){
	for(i=0; i<nr; i++){
		switch(localmemory[i][3]){
			case 3:
				printf("Def. Alarme Relógio - %c:%c:%c",localmemory[i][4],localmemory[i][5],localmemory[6]);
			break;
			case 4:
				printf("Def. Alarme Temperatura - %c",localmemory[i][4]);
			break;
			case 5:
				printf("Def. Alarme Luminosidade - %c",localmemory[i][4]);
			break;
			case 6:
				printf("Activação/Desactivação de Alarmes - %c:%c:%c",localmemory[i][0], localmemory[i][1], localmemory[i][2]);
			break;
		}
	}
}

void listalarmes(char* msg_rec, int type){
	switch(type){
		case(1):
			printf("Alarmes Relógio");
		break;
		case(2):
			printf("Alarmes Temperatura");
		break;
		case(3):
			printf("Alarmes Luminosidade");
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
					if(localmemory[i][3]==7){ //código alarmes relógio
					lowerbound=0;
						for(j=0; j<3; j++){
							alarmesrel[j]=localmemory[i];
							
							if(alarmesrel[j]>t1[j]){
								lowerbound++;
							}
						}
						if(lowerbound>3){ //h,m,s > h1 m1 s1
							alarmetemp=localmemory[4];
							alarmelum=localmemory[5];
							printf("Alarme Temperatura- Horas<%c:%c:%c> Temperatura<%c> Luminosidade <%c>",i,alarmesrel[0],alarmesrel[1],alarmesrel[2], alarmetemp, alarmelum);
						}
					}
				}
			}else{
				if(nolowbound==2){
					nolowbound=0;
					for(i=0; i<nr ; i++){
						if(localmemory[i][3]==7){ //código alarmes relógio
							for(j=0; j<3; j++){
								alarmesrel[j]=localmemory[i];
							}
									alarmetemp=localmemory[4];
									alarmelum=localmemory[5];
									printf("Alarme Temperatura- Horas<%c:%c:%c> Temperatura<%c> Luminosidade <%c>",i,alarmesrel[0],alarmesrel[1],alarmesrel[2], alarmetemp, alarmelum);
							}
						}
					}else{
						for(i=0; i<nr ; i++){
							if(localmemory[i][3]==7){ //código alarmes relógio
								bound=0;
								for(j=0; j<3; j++){
									alarmesrel[j]=localmemory[i];
									if(alarmesrel[j]>t1[j] && alarmesrel[j]<t2[j]){
										bound++;
									}
								}
								if(bound>3){ 
									alarmetemp=localmemory[4];
									alarmelum=localmemory[5];
									printf("Alarme Temperatura- Horas<%c:%c:%c> Temperatura<%c> Luminosidade <%c>",i,alarmesrel[0],alarmesrel[1],alarmesrel[2], alarmetemp, alarmelum);
								}
							}
						}
					}
			}		

}