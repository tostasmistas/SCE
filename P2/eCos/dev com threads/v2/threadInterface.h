#ifndef __THREADINTERFACE_H
#define __THREADINTERFACE_H

void cmd_sos(void);

int my_getline(char** argv, int argvsize);

void monitor(void);

void threadInterface_func(cyg_addrword_t data);

#endif
