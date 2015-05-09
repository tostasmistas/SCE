/***************************************************************************
| File: cmdfunc1.c
| Concretização de comandos: Agrupamento de dados na estrutura
***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <cyg/io/io.h>

Cyg_ErrNo err;
cyg_io_handle_t serH;

/*-------------------------------------------------------------------------+
| Function: cmd_cr - consulta relógio
+--------------------------------------------------------------------------*/
void cmd_cr (int argc, char** argv)
{
  char cmd_struct_send[2];

	cmd_struct[0]='CRLG';

	//enviar msg
	//receber msg com [h m s]

}

void cmd_ar (int argc, char** argv)
  char cmd_struct_send[5];
  unsigned int i;

	cmd_struct_send[0]='ARLG';
 if (argc > 1) {
    if(argc < 3) {
    	printf("Erro: Não introduziu todos os parâmetros.");
	  	}else{

		for(i=0, i<strlen(argv), i++){
			cmd_struct_send[i+1]=argv[i+1];
		}
    //Enviar mensagem para tarefa de Comunicação
    cyg_mbox_put( pcsendpic, cmd_struct_send );
		}
  }
  else{
	printf("Erro: Não introduziu parâmetros.");
	}
}

void cmd_ctl (int argc, char** argv)
{
  char cmd_struct_send[2];
  char *m;

	cmd_struct_send[0]='CTEL';

  //Enviar mensagem para tarefa de Comunicação
  cyg_mbox_put( pcsendpic, cmd_struct_send );

	//Falta retirar [T L]

}

void cmd_cp (int argc, char** argv)
{
  char cmd_struct_send[2];

	cmd_struct_send[0]='CPAR';

  //Enviar mensagem para tarefa de Comunicação
  cyg_mbox_put( pcsendpic, cmd_struct_send );


}

void cmd_mpm (int argc, char** argv)
{
  char cmd_struct_send[3];
  unsigned int i;
	cmd_struct_send[0]='MPMN';

 if (argc > 1) {
    if(argc < 2) {
    	printf("Erro: Não introduziu todos os parâmetros.");
	  	}else{

		for(i=0, i<strlen(argv), i++){
			cmd_struct_send[i+1]=argv[i+1];
		}
    //Enviar mensagem para tarefa de Comunicação
    cyg_mbox_put( pcsendpic, cmd_struct_send );
		}
  }
  else{
	printf("Erro: Não introduziu parâmetros.");
	}
}


void cmd_ca (int argc, char** argv)
{
  char cmd_struct_send[];

	cmd_struct_send[0]='CALA';

  //Enviar mensagem para tarefa de Comunicação
  cyg_mbox_put( pcsendpic, cmd_struct_send );


		//receber msg com [h m s T L A/a]
}

void cmd_dar (int argc, char** argv)
{
  char cmd_struct_send[5];
  unsigned int i;

	cmd_struct_send[0]='DALR';
 if (argc > 1) {
    if(argc < 3) {
    	printf("Erro: Não introduziu todos os parâmetros.");
	  	}else{

		for(i=0, i<strlen(argv), i++){
			cmd_struct_send[i+1]=argv[i+1];
		}
    //Enviar mensagem para tarefa de Comunicação
    cyg_mbox_put( pcsendpic, cmd_struct_send );

		}
  }
  else{
	printf("Erro: Não introduziu parâmetros.");
	}
}

void cmd_dat (int argc, char** argv)
{
  char cmd_struct_send[3];
  unsigned int i;
	cmd_struct_send[0]='DALT';

 if (argc > 1) {
    if(argc < 2) {
    	printf("Erro: Não introduziu todos os parâmetros.");
	  	}else{

		for(i=0, i<strlen(argv), i++){
			cmd_struct_send[i+1]=argv[i+1];
		}
    //Enviar mensagem para tarefa de Comunicação
    cyg_mbox_put( pcsendpic, cmd_struct_send );

		}
  }
  else{
	printf("Erro: Não introduziu parâmetros.");
	}
}

void cmd_dal (int argc, char** argv)
{
  char cmd_struct_send[3];
  unsigned int i;
	cmd_struct_send[0]='DALL';

 if (argc > 1) {
    if(argc < 2) {
    	printf("Erro: Não introduziu todos os parâmetros.");
	  	}else{

		for(i=0, i<strlen(argv), i++){
			cmd_struct_send[i+1]=argv[i+1];
		}
    //Enviar mensagem para tarefa de Comunicação
    cyg_mbox_put( pcsendpic, cmd_struct_send );

		}
  }
  else{
	printf("Erro: Não introduziu parâmetros.");
	}
}

void cmd_aa (int argc, char** argv)
{
  char cmd_struct_send[2];

	cmd_struct_send[0]='AALA';

  //Enviar mensagem para tarefa de Comunicação
  cyg_mbox_put( pcsendpic, cmd_struct_send );


}

void cmd_ir (int argc, char** argv)
{
  char cmd_struct_send[2];

	cmd_struct_send[0]='IREG';

  //Enviar mensagem para tarefa de Comunicação
  cyg_mbox_put( pcsendpic, cmd_struct_send );


 	//receber [N nr ie il]

}

void cmd_trc (int argc, char** argv)
{
  char cmd_struct_send[3];

	cmd_struct_send[0]='TRGC';
if (argc > 1) {
    if(argc < 2) {
    	printf("Erro: Não introduziu todos os parâmetros.");
	  	}else{

		for(i=0, i<strlen(argv), i++){
			cmd_struct_send[i+1]=argv[i+1];
		}
    //Enviar mensagem para tarefa de Comunicação
    cyg_mbox_put( pcsendpic, cmd_struct_send );

		//receber [regs]

		}
  }
  else{
	printf("Erro: Não introduziu parâmetros.");
	}

}

void cmd_tri (int argc, char** argv)
{
  char cmd_struct_send[4];

	cmd_struct_send[0]='TRGC';
if (argc > 1) {
    if(argc < 3) {
    	printf("Erro: Não introduziu todos os parâmetros.");
	  	}else{

		for(i=0, i<strlen(argv), i++){
			cmd_struct_send[i+1]=argv[i+1];
		}
    //Enviar mensagem para tarefa de Comunicação
    cyg_mbox_put( pcsendpic, cmd_struct_send );

		//receber [regs]

		}
  }
  else{
	printf("Erro: Não introduziu parâmetros.");
	}

}

//Falta aqui:
//Registos
