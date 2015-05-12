#ifndef __MAIN_H_
#define __MAIN_H_

#include <p18f452.h>
#include <portb.h>
#include <adc.h>
#include <xlcd.h>
#include <timers.h>
#include <delays.h>
#include <stdio.h>
#include <i2c.h>
#include <stdlib.h>
#include <reset.h>
#include <pwm.h>
#include <usart.h>

#define freq_CPU 4000 // 4 kHz

/*#define SOM 0xFD      // inicio mensagem
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
#define CMD_ERRO 0xFF // erro no comando*/

/*
#define SOM 0x41      // inicio mensagem                                            (A)
#define EOM 0x42      // fim mensagem                                               (B)
#define CRLG 0x43     // consultar relogio                                          (C)
#define ARLG 0x44     // acertar relogio                                            (D)
#define CTEL 0x45     // consultar temperatura e luminosidade                       (E)
#define CPAR 0x46     // consultar parametros                                       (F)
#define MPMN 0x47     // modificar periodo monitorizacao                            (G)
#define CALA 0x48     // consultar alarmes                                          (H)
#define DALR 0x49     // definir alarme relogio                                     (I)
#define DALT 0x4A     // definir alarme temperatura                                 (J)
#define DALL 0x4B     // definir alarme luminosidade                                (K)
#define AALA 0x4C     // activar/desactivar alarmes                                 (L)
#define IREG 0x4D     // informacao sobre registos                                  (M)
#define TRGC 0x4E     // transferir registos a partir da posicao corrente           (N)
#define TRGI 0x4F     // transferir registos a partir do indice especificado        (O)
#define NMCH 0x50     // notificacao memoria cheia                                  (P)
#define CMD_OK 0x51      // comando realizado com sucesso                           (Q)
#define CMD_ERRO 0x52 // erro no comando                                            (R)
*/

extern volatile int hours; //compiler users guide page vinte e oito
extern volatile int minutes;
extern volatile int seconds;
extern volatile int cursor_pos;

extern int PMON;
extern int old_PMON;
extern int TSOM;
extern int alarme_hours;
extern int alarme_minutes;
extern int alarme_seconds;
extern int sd;
extern int su;
extern int md;
extern int mu;
extern int hd;
extern int hu;
extern int n_lum;
extern int alarme_lum;
extern int alarme_temp;
extern int alarme_sd;
extern int alarme_su;
extern int alarme_md;
extern int alarme_mu;
extern int alarme_hd;
extern int alarme_hu;

/* flags */
extern volatile unsigned char modo_sleep;
extern volatile unsigned char modo_modificacao;
extern volatile unsigned char change_L;
extern volatile unsigned char change_T;
extern volatile unsigned char change_A;
extern volatile unsigned char change_AS;
extern volatile unsigned char change_AM;
extern volatile unsigned char change_AH;
extern volatile unsigned char sai_modificacao;
extern volatile unsigned char mudei_ahoras;
extern volatile unsigned char mudei_alum;
extern volatile unsigned char mudei_atemp;
extern volatile unsigned char d_a_alarmes;
extern volatile unsigned char alarme_OFF;
extern volatile unsigned char alarme_lum_ON;
extern volatile unsigned char alarme_temp_ON;
extern volatile unsigned char alarme_clock_ON;
extern volatile unsigned char desliga_alarmes;
extern volatile unsigned char segundos_mudou;
extern volatile unsigned char minutos_mudou;
extern volatile unsigned char horas_mudou;
extern volatile unsigned char disp_ahoras;
extern volatile unsigned char ler_sensores;
extern volatile unsigned char protocolo_OK;

extern unsigned char checksumIsRight;
extern unsigned char NREG;
extern unsigned char nr;
extern unsigned char ie;
extern unsigned char il;

extern char temperatura;
extern char codigoev;

extern char temperatura_s[5];
extern char luminosidade[4];
extern char alarmes[2];
extern char alarmes_prev[2];
extern char msg_rec[4];

#endif
