/***************************************************************************
| File: interface.c
***************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*-------------------------------------------------------------------------+
| Cabeçalhos das Funções de Comando
! Faltam funções de Processamento e de Informação sobre os Registos
+--------------------------------------------------------------------------*/
extern void cmd_cr (int, char** );
extern void cmd_ar (int, char** );
extern void cmd_ctl (int, char** );
extern void cmd_cp (int, char** );
extern void cmd_mpm (int, char** );
extern void cmd_ca (int, char** );
extern void cmd_dar (int, char** );
extern void cmd_dat (int, char** );
extern void cmd_dal (int, char** );
extern void cmd_aa (int, char** );
extern void cmd_ir (int, char** );
extern void cmd_trc (int, char** );
extern void cmd_tri (int, char** );
       void cmd_sos  (int, char** );

/*-------------------------------------------------------------------------+
| Definição das Constantes de Comando
+--------------------------------------------------------------------------*/
const char TitleMsg[] = "\n Application Control Monitor\n";
const char InvalMsg[] = "\nInvalid command!";

struct 	command_d {
  void  (*cmd_fnct)(int, char**);
  char*	cmd_name;
  char*	cmd_help;
} const commands[] = {
//Interação com tarefa de comunicação
  {cmd_sos,  "sos","                 					help"},
  {cmd_cr,  "cr","                 	 					consultar relógio"},
  {cmd_ar, 	"ar","<h> <m> <s>        					acertar relógio"},
  {cmd_ctl,  "ctl","             	 					consultar temperatura e luminosidade"},
  {cmd_cp,  "cp","		             					consultar parâmetros (NREG,PMON,TSOM)"},
  {cmd_mpm,  "mpm","<p>	  			 					modificar período de monitorização(segundos- 0 desactiva)"},
  {cmd_ca,  "ca","			         					consultar alarmes(relógio,temp.,lumi.,activos/inactivos)"},
  {cmd_dar,  "dar","<h> <m> <s>      					definir alarme relógio"},
  {cmd_dat, "dat","<t>  			 					definir alarme temperatura"},
  {cmd_dal, "dal","<l>   			 					definir alarme luminosidade"},
  {cmd_aa, "aa","				     					activar/desactivar alarmes"},
  {cmd_ir, "ir","					 					informação sobre registos (NREG, nr, iescrita, ileitura)"},
  {cmd_trc, "trc","<n> 			     					transferir n registos (ind. leit. corrente)"},
  {cmd_tri, "tri","<n> <i>  		 					transferir n registos a partir do índice i"},

//Informação, listagem e eliminação de registos existentes na memória local
  {cmd_irl, "irl","				     					informação registos locais (NRBUF, nr, iescrita, ileitura)"},
  {cmd_lr, "lr","<n> <i>								listar n registos (mem. local) a partir do índice i"},
  {cmd_er, "er","			         					eliminar registos locais"},

//Interação com tarefa de processamento
  {cmd_cpt, "cpt","				     					consultar período de transferência"},
  {cmd_mpt, "mpt","<p>			    					modificar período de transferência (minutos - 0 desactiva)"},
  {cmd_lar, "lar","<h1> <m1> <s1> <h2> <m2> <s2>		listar alarmes relógio entre t1 e t2"},
  {cmd_lat, "lat","<h1> <m1> <s1> <h2> <m2> <s2>		listar alarmes temperatura entre t1 e t2"},
  {cmd_lal, "lal","<h1> <m1> <s1> <h2> <m2> <s2>		listar alarmes luminosidade entre t1 e t2"},
  {cmd_iga, "iga","				     					informação gestão alarmes (definição e activação)"},
  {cmd_ig, "mpt","<p>			 						informação geral (início, relógio, memória, período monit.)"}
};

#define NCOMMANDS  (sizeof(commands)/sizeof(struct command_d))
#define ARGVECSIZE 10
#define MAX_LINE   50

/*-------------------------------------------------------------------------+
| Function: cmd_sos - provides a rudimentary help
+--------------------------------------------------------------------------*/
void cmd_sos (int argc, char **argv)
{
  int i;

  printf("%s\n", TitleMsg);
  for (i=0; i<NCOMMANDS; i++)
    printf("%s %s\n", commands[i].cmd_name, commands[i].cmd_help);
}

/*-------------------------------------------------------------------------+
| Function: getline        (called from monitor)
+--------------------------------------------------------------------------*/
int my_getline (char** argv, int argvsize)
{
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
| Function: monitor        (called from main)
+--------------------------------------------------------------------------*/
void monitor (void)
{
  static char *argv[ARGVECSIZE+1], *p;
  int argc, i;
  char *m;

  printf("%s Type sos for help\n", TitleMsg);
  for (;;) {
    printf("\nCmd> ");
    /* Reading and parsing command line  ----------------------------------*/
    if ((argc = my_getline(argv, ARGVECSIZE)) > 0) {
      for (p=argv[0]; *p != '\0'; *p=tolower(*p), p++);
      for (i = 0; i < NCOMMANDS; i++)
	if (strcmp(argv[0], commands[i].cmd_name) == 0)
	  break;
      /* Executing commands -----------------------------------------------*/
      if (i < NCOMMANDS)
	commands[i].cmd_fnct (argc, argv);
      else
	printf("%s", InvalMsg);
    } /* if my_getline */
  } /* forever */

  //Como fazer com que esteja sempre a verificar se chegou uma mensagem?  Prioridades??
  //Mensagens Placa que retornaram Dados/Notificação de Memória Cheia
  m=cyg_mbox_get(pictopc);
  codigo=m[indicecodigo]
    switch(codigo){
      case('CRLG')
        //Imprimir horas, minutos, segundos;
      break;
      case('CTEL')
        //Imprimir T e L
      break;
      case('CPAR')
        //Imprimir Parâmetros
      break;
      case('CALA')
        //Imprir Alarmes
      break;
      case('IREG')
        //Imprimir informação sobre registos
      break;
      case('TRGC')
        //Imprimir Conteúdo Registos
      break;
      case('TRGI')
        //Imprimir Conteúdo Registos
      break;
      case('NMCH')
      printf("Memória Cheia");
      break;
    }
}
