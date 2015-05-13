#ifndef __THREAD_PROCESSING_H
#define __THREAD_PROCESSING_H




cyg_handle_t processing_clockH, countH;
cyg_alarm alarmH;


void alarmecomunicacao(void);
void gestaoalarmes(void);
void listalarmes(unsigned char* msg_rec, int type);
void informacaogeral(void);
void threadProcessing_func(cyg_addrword_t data);

#endif
