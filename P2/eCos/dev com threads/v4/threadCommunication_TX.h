#ifndef __THREADCOMMUNICATIONTX_H
#define __THREADCOMMUNICATIONTX_H

Cyg_ErrNo send_buffer (char msgToSend[]);

void threadCommunicationTX_func(cyg_addrword_t data);

#endif
