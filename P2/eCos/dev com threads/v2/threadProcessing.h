#ifndef __THREAD_PROCESSING_H
#define __THREAD_PROCESSING_H

unsigned char *msg_rec;
int bound=0;
int lowerbound=0;
int i=0;
int j=0;
int noupbound=0;
int nolowbound=0;
int ptransf=0;
int ptransfnew=0;
char alarmetemp=0;
char alarmesrel[3];
char alarmelum=0;

cyg_handle_t processing_clockH, countH;
cyg_alarm alarmH;

void threadProcessing_func(cyg_addrword_t data);
void alarmecomunicacao();
void gestaoalarmes();
void listalarmes(char* msg_rec, int type);
#endif