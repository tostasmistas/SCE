#ifndef __THREAD_PROCESSING_H
#define __THREAD_PROCESSING_H

void threadProcessing_func(cyg_addrword_t data);
void gestaoalarmes();
void listalarmes(char* msg_rec, int type);
#endif