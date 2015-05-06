#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <cyg/io/io.h>

#include "commandFunctions.h"

Cyg_ErrNo err;
cyg_io_handle_t serH;

/*-------------------------------------------------------------------------+
| Variable and constants definition
+--------------------------------------------------------------------------*/
const char TitleMsg[] = "\n interface PIC<->PC\n";
const char InvalMsg[] = "\n invalid command!";

struct 	command_d {
	void  (*cmd_fnct)(void);
	char*	cmd_name;
	char*	cmd_help;
} const commands[] = {
//interação com tarefa de comunicação
	{cmd_sos, "sos", "	: help"},
	{cmd_cr, "cr", "	: consultar relogio"},
	/*{cmd_ar, "ar", "<h> <m> <s> acertar relógio"},
	{cmd_ctl, "ctl","consultar temperatura e luminosidade"},
	{cmd_cp, "cp","consultar parâmetros (NREG,PMON,TSOM)"},
	{cmd_mpm,  "mpm","<p> modificar período de monitorização(segundos- 0 desactiva)"},
	{cmd_ca,  "ca","consultar alarmes(relógio,temp.,lumi.,activos/inactivos)"},
	{cmd_dar,  "dar","<h> <m> <s> definir alarme relógio"},
	{cmd_dat, "dat","<t> definir alarme temperatura"},
	{cmd_dal, "dal","<l> definir alarme luminosidade"},
	{cmd_aa, "aa","activar/desactivar alarmes"},
	{cmd_ir, "ir","informação sobre registos (NREG, nr, iescrita, ileitura)"},
	{cmd_trc, "trc","<n> transferir n registos (ind. leit. corrente)"},
	{cmd_tri, "tri","<n> <i> transferir n registos a partir do índice i"},

//informação, listagem e eliminação de registos existentes na memória local
	{cmd_irl, "irl","informação registos locais (NRBUF, nr, iescrita, ileitura)"},
	{cmd_lr, "lr","<n> <i> listar n registos (mem. local) a partir do índice i"},
	{cmd_er, "er","eliminar registos locais"},

//interação com tarefa de processamento
	{cmd_cpt, "cpt","consultar período de transferência"},
	{cmd_mpt, "mpt","<p> modificar período de transferência (minutos - 0 desactiva)"},
	{cmd_lar, "lar","<h1> <m1> <s1> <h2> <m2> <s2> listar alarmes relógio entre t1 e t2"},
	{cmd_lat, "lat","<h1> <m1> <s1> <h2> <m2> <s2> listar alarmes temperatura entre t1 e t2"},
	{cmd_lal, "lal","<h1> <m1> <s1> <h2> <m2> <s2> listar alarmes luminosidade entre t1 e t2"},
	{cmd_iga, "iga", "informação gestão alarmes (definição e activação)"},
	{cmd_ig, "ig","<p>	informação geral (início, relógio, memória, período monit.)"},*/
	{cmd_sair, "sair","	: sair"}
};

/*-------------------------------------------------------------------------+
| Function: getline        (called from monitor)
+--------------------------------------------------------------------------*/
int my_getline (char** argv, int argvsize) {
  static char line[MAX_LINE];
  char *p;
  int argc;

  fgets(line, MAX_LINE, stdin);

  /* Break command line into an o.s. like argument vector,
     i.e. compliant with the (int argc, char **argv) specification --------*/

  for (argc=0,p=line; (*line != '\0') && (argc < argvsize); p=NULL,argc++) {
    p = strtok(p, " \t\n");
    argv[argc] = p;
    if (p == NULL) return argc;
  }
  argv[argc] = p;
  return argc;
}

/*-------------------------------------------------------------------------+
| function: monitor        (called from main)
+--------------------------------------------------------------------------*/
void monitor (void) {
	static char *argv[ARGVECSIZE+1], *p;
	int argc, i;

	printf("%s type sos for help\n", TitleMsg);
	for (;;) {
		printf("\ncmd> ");
		/* Reading and parsing command line  ----------------------------------*/
		if ((argc = my_getline(argv, ARGVECSIZE)) > 0) {
	  		for (p=argv[0]; *p != '\0'; *p=tolower(*p), p++);
	  		for (i = 0; i < NCOMMANDS; i++)
				if (strcmp(argv[0], commands[i].cmd_name) == 0)
	  				break;
	  /* Executing commands -----------------------------------------------*/
			if (i < NCOMMANDS)
				commands[i].cmd_fnct();
	  		else
				printf("%s", InvalMsg);
		} /* if my_getline */
	} /* forever */
}

/*-------------------------------------------------------------------------+
| function: cmd_ini - inicializar dispositivo
+--------------------------------------------------------------------------*/
void cmd_ini(int argc, char **argv) {
	printf("io_lookup\n");
	if ((argc > 1) && (argv[1][0] = '1'))
	err = cyg_io_lookup("/dev/ser1", &serH);
	else err = cyg_io_lookup("/dev/ser0", &serH);
	printf("lookup err=%x\n", err);
}

/*-------------------------------------------------------------------------+
| function: enviar mensagem (hexadecimal)
+--------------------------------------------------------------------------*/
void send_hex (unsigned char hex_code, unsigned int lenght) {
	err = cyg_io_write(serH, &hex_code, &lenght);
}

/*-------------------------------------------------------------------------+
| function: receber mensagem (hexadecimal)
+--------------------------------------------------------------------------*/
void recv_hex (unsigned char buf_read[], unsigned int lenght) {
	err = cyg_io_read(serH, buf_read, &lenght);
}

/*-------------------------------------------------------------------------+
| function: cmd_sos - listar todos os comandos
+--------------------------------------------------------------------------*/
void cmd_sos(void) {
  int i;

  printf("%s\n", TitleMsg);
  for (i=0; i<NCOMMANDS; i++)
    printf("%s %s\n", commands[i].cmd_name, commands[i].cmd_help);
}

/*-------------------------------------------------------------------------+
| Function: cmd_sair - termina a aplicacao
+--------------------------------------------------------------------------*/
void cmd_sair(void) {
	exit(0);
}

/*-------------------------------------------------------------------------+
| function: cmd_cr - consultar relogio
+--------------------------------------------------------------------------*/
void cmd_cr(void) {
	unsigned char buf_read[6] = {0};

	send_hex(SOM, 1);
	send_hex(CRLG, 1);
	send_hex(EOM, 1);
	recv_hex(buf_read, 6); // receber 6 bytes
	printf("horas: %d\n", (int)buf_read[2]);
	printf("minutos: %d\n", (int)buf_read[3]);
	printf("segundos: %d\n", (int)buf_read[4]);
}
