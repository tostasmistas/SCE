#include "main.h"
#include "memorias.h"
#include "usart.h"

int j = 0;
int k = 0;
int aux = 0;

void escrever_USART(char send_byte){
  while (BusyUSART());
  putcUSART(send_byte);
}

void USART_protocolo_OK(char msg_rec[]){
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
        old_PMON=PMON;
        PMON = msg_rec[1];
        update_EEPROM_interna_parametros();
        update_EEPROM_external(10);
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
        update_EEPROM_external(3);
        escrever_USART(SOM);
        escrever_USART(DALR);
        escrever_USART(CMD_OK);
        escrever_USART(EOM);
        break;
      case DALT: // definir alarme temperatura
        alarme_temp = (int)msg_rec[1];
        update_EEPROM_interna_temp_alarme();
        update_EEPROM_external(4);
        escrever_USART(SOM);
        escrever_USART(DALT);
        escrever_USART(CMD_OK);
        escrever_USART(EOM);
        break;
      case DALL: // definir alarme luminosidade
        alarme_lum = msg_rec[1];
        update_EEPROM_interna_lum_alarme();
        update_EEPROM_external(5);
        escrever_USART(SOM);
        escrever_USART(DALL);
        escrever_USART(CMD_OK);
        escrever_USART(EOM);
        break;
      case AALA: // activar/desactivar alarmes
        if(alarmes[0] == 'a'){
          alarmes[0] = 'A';
        }
        else{
          alarmes[0] = 'a';
        }
        update_EEPROM_external(6);
        escrever_USART(SOM);
        escrever_USART(AALA);
        escrever_USART(CMD_OK);
        escrever_USART(EOM);
        break;
      case IREG: // informação sobre registos (NREG, nr, iescrita, ileitura)
        escrever_USART(SOM);
        escrever_USART(IREG);
        escrever_USART(NREG);
        escrever_USART(nr);
        escrever_USART(ie);
        escrever_USART(il);
        escrever_USART(EOM);
        break;
      case TRGC: // transferir n registos a partir da posição corrente
        escrever_USART(SOM);
        escrever_USART(TRGC);
        aux = 8*((int)msg_rec[1]);
        for(k=0; k<aux; k++){
          if(il<nr){
            escrever_USART(ler_registo());
          }
        }
        escrever_USART(EOM);
        break;
      case TRGI: // transferir n registos a partir do indice i
        escrever_USART(SOM);
        escrever_USART(TRGI);
        aux = ((int)msg_rec[2]);
        if (aux>=0 && aux<NREG){
          il = aux;
          il_byte=il*8;
          aux = 8*((int)msg_rec[1]);
          for(j=0; j<aux; j++){
            if(il<nr){
            escrever_USART(ler_registo());
          }
          }
        }
        else{
          escrever_USART(CMD_ERRO);
        }
        escrever_USART(EOM);
        break;
      case NMCH: // notificação memoria cheia - a thread de interface é que pede isto??????
        escrever_USART(SOM);
        escrever_USART(IREG);
        escrever_USART(NREG);
        escrever_USART(nr);
        escrever_USART(ie);
        escrever_USART(il);
        escrever_USART(EOM);
        break;
      default:
        break;
  }
}
