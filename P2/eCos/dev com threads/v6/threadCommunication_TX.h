#ifndef __THREADCOMMUNICATIONTX_H
#define __THREADCOMMUNICATIONTX_H

void send_buffer (char msgToSend[], unsigned int length);

void threadCommunicationTX_func(cyg_addrword_t data);

#endif
