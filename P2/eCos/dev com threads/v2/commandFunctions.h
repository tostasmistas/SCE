#ifndef __COMMANDFUNCTIONS_H
#define __COMMANDFUNCTIONS_H

#define NCOMMANDS  (sizeof(commands)/sizeof(struct command_d))
#define ARGVECSIZE 10
#define MAX_LINE   50

void cmd_ini(int argc, char **argv);

void cmd_sos(int argc, char **argv);

void cmd_sair(int argc, char **argv);

void cmd_cr(int argc, char** argv); 

void cmd_ar(int argc, char** argv);

void cmd_ctl(int argc, char** argv);

void cmd_cp(int argc, char** argv);

void cmd_mpm(int argc, char** argv);

void cmd_ca(int argc, char** argv); 

void cmd_dar(int argc, char** argv);

void cmd_dat(int argc, char** argv);

void cmd_dal(int argc, char** argv);

void cmd_aa(int argc, char** argv); 

void cmd_ir(int argc, char** argv);

void cmd_trc(int argc, char** argv);

void cmd_tri(int argc, char** argv);

void cmd_irl(int argc, char** argv);

void cmd_lr(int argc, char** argv);

void cmd_er(int argc, char** argv);

void cmd_cpt(int argc, char** argv);

void cmd_mpt(int argc, char** argv);

void cmd_lar(int argc, char** argv);

void cmd_lal(int argc, char** argv);

void cmd_lat(int argc, char** argv);

void cmd_iga(int argc, char** argv);

void cmd_ig(int argc, char** argv);
#endif
