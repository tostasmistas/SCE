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

#define SOM 0xFD /* inicio mensagem */
#define EOM 0xFE /* fim mensagem */
#define CRLG 0xC0 /* consultar relogio */
#define ARLG 0xC1 /* acertar relogio */
#define CTEL 0xC2 /* consultar temperatura e luminosidade */
#define CPAR 0xC3 /* consultar parametros */
#define MPMN 0xC4 /* modificar periodo monitorizacao */
#define CALA 0xC5 /* consultar alarmes */
#define DALR 0xC6 /* definir alarme relogio */
#define DALT 0xC7 /* definir alarme temperatura */
#define DALL 0xC8 /* definir alarme luminosidade */
#define AALA 0xC9 /* activar/desactivar alarmes */
#define IREG 0xCA /* informacao sobre registos */
#define TRGC 0xCB /* transferir registos a partir da posicao corrente */
#define TRGI 0xCC /* transferir registos a partir do indice especificado */
#define NMCH 0xCD /* notificacao memoria cheia */
#define CMD_OK 0 /* comando realizado com sucesso */
#define CMD_ERRO 0xFF /* erro no comando */

extern volatile int hours; //compiler users guide page vinte e oito
extern volatile int minutes;
extern volatile int seconds;
extern volatile int cursor_pos;

extern int PMON;
extern int TSOM;
extern int NREG;
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

extern char temperatura;
extern char codigoev;

extern char temperatura_s[5];
extern char luminosidade[4];
extern char alarmes[2];
extern char alarmes_prev[2];
extern char msg_rec[4];

#endif
