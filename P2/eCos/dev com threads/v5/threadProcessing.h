#ifndef __THREAD_PROCESSING_H
#define __THREAD_PROCESSING_H

void alarmecomunicacao(cyg_handle_t alamarH, cyg_addrword_t data);

void gestaoalarmes(void);

void listalarmes(unsigned char* msg_rec, int type);

void informacaogeral(void);

void threadProcessing_func(cyg_addrword_t data);

#endif
