#ifndef __THREADCOMMUNICATIONRX_H
#define __THREADCOMMUNICATIONRX_H

void recv_hex (char *byteToRead, unsigned int lenght);

void threadCommunicationRX_func(cyg_addrword_t data);

#endif
