#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <cyg/io/io.h>
#include <cyg/kernel/kapi.h>

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
#define NRBUF 150     // numero máximo de eventos registados na memória local
#define CPT  0xCE     // consultar periodo de transferencia
#define MPT  0xCF     // modificar periodo de transferencia
#define LAR  0xD0     // listar alarmes relógio
#define LAT  0xD1     // listar alarmes temperatura
#define LAL  0xD2     // listar alarmes luinosidade
#define IGA  0xD3     // listar informacao gestao alarmes
#define IG   0xD4     // listar informacao geral
#define TR   0xD5     // transferencia periódica de registos

extern cyg_handle_t 	threadInterface, threadCommunicationRX, threadCommunicationTX, threadProcessing; // handles for the threads
extern cyg_handle_t 	mbInter, mbComTX, mbProc; // handles for the mailboxs
extern cyg_handle_t	  processing_clockH, countH; // handles for the counters
extern cyg_handle_t 	alarmProcH; // handles for the alarms
extern cyg_alarm 		  alarmProc;
extern cyg_mutex_t 		localMemory_mutex, transferRegistos_mutex, escritaScreen_mutex;

// memória Local
extern char **localMemory; //memória local
extern int nr; //numero de registos
extern int indescrita; //indice de escrita
extern int indleitura; //indíce de leitura

// variavel global para identificar quem mandou transferir registos - esta protegida por um mutex
extern int transferRegistos;
