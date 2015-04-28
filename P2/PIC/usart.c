#include "main.h"
#include "usart.h"

void escrever_USART(char send_byte){
  while (BusyUSART());
  putcUSART(send_byte);
}

void check_USART_protocolo_OK(char msg_rec[4]){
    switch(msg_rec[0]){
      case CRLG: // consultar relogio
        escrever_USART(SOM);
        escrever_USART(CRLG);
        escrever_USART(hours);
        escrever_USART(minutes);
        escrever_USART(seconds);
        escrever_USART(EOM);
        break;
      case ARLG: // acertar relogio
        hours = msg_rec[1];
        hd = hours/10;
      	hu = hours%10;
        update_EEPROM_interna_relogio_hours();
        minutes = msg_rec[2];
        md = minutes/10;
      	mu = minutes%10;
        update_EEPROM_interna_relogio_minutes();
        seconds = msg_rec[3];
        sd = seconds/10;
        su = seconds%10;
        update_EEPROM_interna_relogio_seconds();
        update_EEPROM_external(2);
        escrever_USART(SOM);
        escrever_USART(ARLG);
        escrever_USART(CMD_OK);
        escrever_USART(EOM);
        break;
      case CTEL: // consultar temperatura e luminosidade
        escrever_USART(SOM);
        escrever_USART(CTEL);
        escrever_USART(temperatura);
        escrever_USART(n_lum);
        escrever_USART(EOM);
        break;
      case CPAR: // consultar parametros
        escrever_USART(SOM);
        escrever_USART(CPAR);
        escrever_USART(NREG);
        escrever_USART(PMON);
        escrever_USART(TSOM);
        escrever_USART(EOM);
        break;
      case MPMN: // modificar PMON
        PMON = msg_rec[1];
        update_EEPROM_interna_parametros();
        escrever_USART(SOM);
        escrever_USART(MPMN);
        escrever_USART(CMD_OK);
        escrever_USART(EOM);
        break;
      case CALA: // consultar alarmes
        escrever_USART(SOM);
        escrever_USART(CALA);
        escrever_USART(alarme_hours);
        escrever_USART(alarme_minutes);
        escrever_USART(alarme_seconds);
        escrever_USART(alarme_temp);
        escrever_USART(alarme_lum);
        escrever_USART(alarmes[0]);
        escrever_USART(EOM);
        break;
      case DALR: // definir alarme relogio
        alarme_hours = msg_rec[1];
        alarme_hd = alarme_hours/10;
      	alarme_hu = alarme_hours%10;
        alarme_minutes = msg_rec[2];
        alarme_md = alarme_minutes/10;
      	alarme_mu = alarme_minutes%10;
        alarme_seconds = msg_rec[3];
        alarme_sd = alarme_seconds/10;
      	alarme_su = alarme_seconds%10;
        update_EEPROM_interna_relogio_alarme();
        update_EEPROM_external(2);
        escrever_USART(SOM);
        escrever_USART(DARL);
        escrever_USART(CMD_OK);
        escrever_USART(EOM);
        break;
      case DALT:

      break;
      case DALL:

      break;
      case AALA:

      break;
      case IREG:

      break;
      case TRGC:

      break;
      case TRGI:

      break;
      case NMCH:

      break;



  }
}
