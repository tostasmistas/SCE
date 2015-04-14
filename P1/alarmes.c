#include "main.h"
#include "alarmes.h"

int seconds_alarme_TSOM = 0;

unsigned char up_L = 0;
unsigned char up_T = 0;

char spaces_alarmes[4] = {0};

void rotina_verificacao_alarmes(void){

  if(segundos_mudou	== 1){
    /* verificacao de alarmes */
    if(alarmes[0] == 'A'){

      if(n_lum > alarme_lum && up_L == 1){
        modo_sleep = 0;
        alarme_OFF = 0;
        alarme_lum_ON = 1;
        if(modo_modificacao == 1){
          modo_modificacao = 0;
          sai_modificacao = 1;
          cursor_pos = 0;
        }
        seconds_alarme_TSOM = seconds;
        up_L = 0;
        CCP1CON = 0x0F; // turn the buzzer on
        //update_EEPROM_external(9);

        //SetDDRamAddr(0x47);
        //while( BusyXLCD() );
        //putrsXLCD("O");
      }

      if(n_lum < alarme_lum && up_L == 0){
        modo_sleep = 0;
        alarme_OFF = 0;
        alarme_lum_ON = 1;
        if(modo_modificacao == 1){
          modo_modificacao = 0;
          sai_modificacao = 1;
          cursor_pos = 0;
        }
        seconds_alarme_TSOM = seconds;
        up_L = 1;
        CCP1CON = 0x0F; // turn the buzzer on
      //	update_EEPROM_external(9);

        //SetDDRamAddr(0x47);
        //while( BusyXLCD() );
        //putrsXLCD("O");
      }

      if(((int)temperatura) > alarme_temp && up_T == 1){
        modo_sleep = 0;
        alarme_OFF = 0;
        alarme_temp_ON = 1;
        if(modo_modificacao == 1){
          modo_modificacao = 0;
          sai_modificacao = 1;
          cursor_pos = 0;
        }
        seconds_alarme_TSOM = seconds;
        up_T = 0;
        CCP1CON = 0x0F; // turn the buzzer on
        //update_EEPROM_external(8);

        //SetDDRamAddr(0x47);
        //while( BusyXLCD() );
        //putrsXLCD("O");
      }

      if(((int)temperatura) < alarme_temp && up_T == 0){
        modo_sleep = 0;
        alarme_OFF = 0;
        alarme_temp_ON = 1;
        if(modo_modificacao == 1){
          modo_modificacao = 0;
          sai_modificacao = 1;
          cursor_pos = 0;
        }
        seconds_alarme_TSOM = seconds;
        up_T = 1;
        CCP1CON = 0x0F; // turn the buzzer on
        //update_EEPROM_external(8);

        //SetDDRamAddr(0x47);
        //while( BusyXLCD() );
        //putrsXLCD("O");
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
        CCP1CON = 0x0F; // turn the buzzer on
        //update_EEPROM_external(7);

        //SetDDRamAddr(0x47);
        //while( BusyXLCD() );
        //putrsXLCD("O");
      }

      if(seconds == seconds_alarme_TSOM + TSOM){
        CCP1CON = 0x00;

        //SetDDRamAddr(0x47);
        //while( BusyXLCD() );
        //putrsXLCD("J");
        //SetDDRamAddr(0x46);
        //while( BusyXLCD() );
        //putrsXLCD(" ");
      }
    }
    segundos_mudou = 0;
  }
}

void avisa_alarmes(void){

  if(desliga_alarmes == 1){
      SetDDRamAddr(0x09);
      while( BusyXLCD() );
      sprintf(spaces_alarmes, "   ");
      putsXLCD(spaces_alarmes);
  }

  if(alarme_clock_ON == 1){

    SetDDRamAddr(0x09);
    while( BusyXLCD() );
    putrsXLCD("A"); // imprimir A no LCD
  }

  if(alarme_temp_ON == 1){

    SetDDRamAddr(0x0A);
    while( BusyXLCD() );
    putrsXLCD("T"); // imprimir T no LCD
  }

  if(alarme_lum_ON == 1){
    SetDDRamAddr(0x0B);
    while( BusyXLCD() );
    putrsXLCD("L"); // imprimir L no LCD
  }

  if(n_lum > alarme_lum){
    up_L = 0;
  }
  else{
    up_L = 1;
  }

  if((int)temperatura > alarme_temp){
    up_T = 0;
  }
  else{
    up_T = 1;
  }
}
