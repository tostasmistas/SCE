#ifndef __COMANDFUNCTIONS_H
#define __COMANDFUNCTIONS_H

#define NCOMMANDS  (sizeof(commands)/sizeof(struct command_d))
#define ARGVECSIZE 10
#define MAX_LINE   50

#define SOM 0xFD      // inicio mensagem
#define EOM 0xFE      // fim mensagem
#define CRLG 0xC0     // consultar relogio
#define ARLG 0xC1     // acertar relogio
#define CTEL 0xC2     // consultar temperatura e luminosidade
#define CPAR 0xC3     // consultar parametros
#define MPMN 0xC4     // modificar periodo monitorizacao
#define CALA 0xC5     // consultar alarmes
#define DALR 0xC6     // definir alarme relogio
#define DALT 0xC7     // definir alarme temperatura
#define DALL 0xC8     // definir alarme luminosidade
#define AALA 0xC9     // activar/desactivar alarmes
#define IREG 0xCA     // informacao sobre registos
#define TRGC 0xCB     // transferir registos a partir da posicao corrente
#define TRGI 0xCC     // transferir registos a partir do indice especificado/
#define NMCH 0xCD     // notificacao memoria cheia
#define CMD_OK 0      // comando realizado com sucesso
#define CMD_ERRO 0xFF // erro no comando

int my_getline (char** argv, int argvsize);

void monitor (void);

void cmd_ini(int argc, char **argv);

void send_hex (unsigned char hex_code, unsigned int lenght);

void recv_hex (unsigned char buf_read[], unsigned int lenght) ;

void cmd_sos(void);

void cmd_sair(void);

void cmd_cr(void);

#endif
