#ifndef __MAIN_H_
#define __MAIN_H_

#include <p18f452.h>  /* for the special function register declarations */
#include <portb.h>    /* for the RB0/INT0 interrupt */
#include <adc.h>      /*for the ADC*/
#include <xlcd.h>
#include <timers.h>
#include <delays.h>
#include <stdio.h>
#include <i2c.h>
#include <stdlib.h>
#include <reset.h>
#include <pwm.h>

#define freq_CPU 4000 // 4 kHz

extern volatile int hours; //compiler users guide page vinte e oito
extern volatile int minutes;
extern volatile int seconds;
extern volatile int cursor_pos;

extern int PMON;
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


extern char temperatura;
extern char codigoev;

extern char temperatura_s[5];
extern char luminosidade[4];
extern char alarmes[2];
extern char alarmes_prev[2];

#endif
