#include "main.h"
#include "memorias.h"
#include "alarmes.h"

int seconds_alarme_TSOM = 0;

unsigned char up_L = 0;
unsigned char up_T = 0;

char spaces_alarmes[4] = {0};

void change_buzzer_freq(int freq){
  OpenPWM1((char)(freq_CPU*1000./(freq*64)-1));
}

void rotina_verificacao_alarmes(void)
{

  if(segundos_mudou	== 1){
    if(minutos_mudou==1){
  	   update_EEPROM_interna_relogio_minutes();
      minutos_mudou=0;
    }
    if(horas_mudou==1){
  	   update_EEPROM_interna_relogio_hours();
      horas_mudou=0;
    }
    /* verificacao de alarmes */
    if(alarmes[0] == 'A' && modo_modificacao == 0){

      if(n_lum > alarme_lum && up_L == 1){
        modo_sleep = 0;
        cursor_pos = 0;
        alarme_OFF = 0;
        alarme_lum_ON = 1;
        if(modo_modificacao == 1){
          modo_modificacao = 0;
          sai_modificacao = 1;
          cursor_pos = 0;
        }
        seconds_alarme_TSOM = seconds;
        up_L = 0;
        change_buzzer_freq(900);
        update_EEPROM_external(9);
      }

      if(n_lum < alarme_lum && up_L == 0){
        modo_sleep = 0;
        cursor_pos = 0;
        alarme_OFF = 0;
        alarme_lum_ON = 1;
        if(modo_modificacao == 1){
          modo_modificacao = 0;
          sai_modificacao = 1;
          cursor_pos = 0;
        }
        seconds_alarme_TSOM = seconds;
        up_L = 1;
        change_buzzer_freq(900);
        update_EEPROM_external(9);
      }

      if(((int)temperatura) > alarme_temp && up_T == 1){
        modo_sleep = 0;
        cursor_pos = 0;
        alarme_OFF = 0;
        alarme_temp_ON = 1;
        if(modo_modificacao == 1){
          modo_modificacao = 0;
          sai_modificacao = 1;
          cursor_pos = 0;
        }
        seconds_alarme_TSOM = seconds;
        up_T = 0;
        change_buzzer_freq(450);
        update_EEPROM_external(8);
      }

      if(((int)temperatura) < alarme_temp && up_T == 0){
        modo_sleep = 0;
        cursor_pos = 0;
        alarme_OFF = 0;
        alarme_temp_ON = 1;
        if(modo_modificacao == 1){
          modo_modificacao = 0;
          sai_modificacao = 1;
          cursor_pos = 0;
        }
        seconds_alarme_TSOM = seconds;
        up_T = 1;
        change_buzzer_freq(450);
        update_EEPROM_external(8);
      }

      if(alarme_hours == hours && alarme_minutes == minutes && alarme_seconds == seconds && disp_ahoras == 1){
        modo_sleep = 0;
        cursor_pos = 0;
        disp_ahoras = 0;
        alarme_OFF = 0;
        alarme_clock_ON = 1;
        if(modo_modificacao == 1){
          modo_modificacao = 0;
          sai_modificacao = 1;
          cursor_pos = 0;
        }
        seconds_alarme_TSOM = seconds;
        change_buzzer_freq(300);
        update_EEPROM_external(7);
      }

      if(seconds == seconds_alarme_TSOM + TSOM){
        CCP1CON = 0x00;
      }
    }
    segundos_mudou = 0;
  }
}

void avisa_alarmes(void)
{

  if(desliga_alarmes == 1){
      SetDDRamAddr(0x09);
      while( BusyXLCD() );
      sprintf(spaces_alarmes, (const rom far char*)"   ");
      putsXLCD(spaces_alarmes);
  }

  if(alarme_clock_ON == 1){

    SetDDRamAddr(0x09);
    while( BusyXLCD() );
    putrsXLCD((const far rom char*)"A"); // imprimir A no LCD
  }

  if(alarme_temp_ON == 1){

    SetDDRamAddr(0x0A);
    while( BusyXLCD() );
    putrsXLCD((const far rom char*)"T"); // imprimir T no LCD
  }

  if(alarme_lum_ON == 1){
    SetDDRamAddr(0x0B);
    while( BusyXLCD() );
    putrsXLCD((const far rom char*)"L"); // imprimir L no LCD
  }

  if(n_lum > alarme_lum){
    up_L = 0;
  }
  else if(n_lum < alarme_lum){
    up_L = 1;
  }

  if((int)temperatura > alarme_temp){
    up_T = 0;
  }
  else if ((int)temperatura < alarme_temp){
    up_T = 1;
  }
}
