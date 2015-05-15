#ifndef __THREADCOMMUNICATION_H
#define __THREADCOMMUNICATION_H

Cyg_ErrNo send_buffer (char msgToSend[]);

Cyg_ErrNo recv_hex (char *byteToRead, unsigned int lenght);

void threadCommunication_func(cyg_addrword_t data);

#endif
