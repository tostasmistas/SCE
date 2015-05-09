#ifndef __COMMANDFUNCTIONS_H
#define __COMMANDFUNCTIONS_H

#define NCOMMANDS  (sizeof(commands)/sizeof(struct command_d))
#define ARGVECSIZE 10
#define MAX_LINE   50

void cmd_ini(int argc, char **argv);

void cmd_sos(void);

void cmd_sair(void);

void cmd_cr(void);

#endif
