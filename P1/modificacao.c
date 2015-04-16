#include "main.h"
#include "memorias.h"
#include "modificacao.h"

int Td = 0;
int Tu = 0;
int alarme_hours_prev = 0;
int alarme_minutes_prev = 0;
int alarme_seconds_prev = 0;
int alarme_lum_prev = 0;
int alarme_temp_prev = 20;

unsigned char mudei_horas = 0;

char time_s	[9] = {0};
char change_hours[3] = {0};
char change_minutes[3] = {0};
char change_seconds[3] = {0};
char change_hours_alarme[3] = {0};
char change_minutes_alarme[3] = {0};
char change_seconds_alarme[3] = {0};
char spaces[8] = {0};
char nova_L[2] = {0};
char nova_T[3] = {0};

///////////////////////////////////////////////////////////////////////////////
//************************** Modo de Modificação ****************************//
///////////////////////////////////////////////////////////////////////////////

void rotina_modo_modificacao(void)
{

	if(modo_modificacao == 1){ // estamos em modo modificacao

		if(cursor_pos  >= 4){
			SetDDRamAddr(0x00);        // First line, first column
			while( BusyXLCD() );
			sprintf(time_s, "%d%d:%d%d:%d%d", alarme_hd, alarme_hu, alarme_md, alarme_mu, alarme_sd, alarme_su);
			putsXLCD(time_s);
		}

		SetDDRamAddr(0x4D);
		while( BusyXLCD() );
		sprintf(luminosidade, "L %d", alarme_lum);
		putsXLCD(luminosidade);

		SetDDRamAddr(0x40);
		while( BusyXLCD() );
		sprintf((char*)temperatura_s, (const rom far char*)"%d%d C", alarme_temp/10, alarme_temp%10);
		putsXLCD(temperatura_s);

		SetDDRamAddr(0x0F);
		while( BusyXLCD() );
		putrsXLCD((const rom far char*)"P"); // letra P que so aparece quando em modo modificacaoo

		SetDDRamAddr(0x09);
		while( BusyXLCD() );
		putrsXLCD((const rom far char*)"ATL"); // letras ATL que aparecem quando em modo modificaoo	para se definir os alarmes

		SetDDRamAddr(0x0D);
		while( BusyXLCD() );
		putsXLCD(alarmes); // letra a ou A para desactivar/activar alarmes

		if(cursor_pos == 1){ // acertar horas
			SetDDRamAddr(0x00);
			while(PORTAbits.RA4  && cursor_pos == 1);
			if(cursor_pos == 1){
				hours++;
				mudei_horas=1;
				Delay1KTCYx(200);
				if (hours == 24){
					hours = 0;
				}
				hd = hours/10;
				hu = hours%10;
				while( BusyXLCD() );
				sprintf(change_hours, "%d%d", hd, hu);
	  		putsXLCD(change_hours);
			}
		}

		else if(cursor_pos == 2){ // acertar minutos
			SetDDRamAddr(0x03);
			while(PORTAbits.RA4 && cursor_pos == 2);
			if(cursor_pos == 2){
				minutes++;
				mudei_horas=1;
				Delay1KTCYx(200);
				if (minutes == 60){
					minutes = 0;
				}
				md = minutes/10;
				mu = minutes%10;
				while( BusyXLCD() );
				sprintf(change_minutes, "%d%d", md, mu);
	  		putsXLCD(change_minutes);
			}
		}

		else if(cursor_pos == 3){ // acertar segundos
			SetDDRamAddr(0x06);
			while(PORTAbits.RA4 && cursor_pos == 3);
			if(cursor_pos == 4 && mudei_horas == 1){
				mudei_horas == 0;
				update_EEPROM_external(2);
			}
			if(cursor_pos == 3){
				seconds++;
				mudei_horas = 1;
				Delay1KTCYx(200); // usado para resolver debounce - delay de 50ms
				if (seconds == 60){
					seconds = 0;
				}
				sd = seconds/10;
				su = seconds%10;
				while( BusyXLCD() );
				sprintf(change_seconds, "%d%d", sd, su);
	  		putsXLCD(change_seconds);
			}
		}

		else if(cursor_pos == 4){ // acertar alarme do relogio
			if(change_A == 0){
				SetDDRamAddr(0x09);
			}
			else{
				if(change_AH == 1){
					SetDDRamAddr(0x00); // colocar o cursor no sitio das dezenas das horas
				}
				else if(change_AM == 1){
					SetDDRamAddr(0x03); // colocar o cursor no sitio das dezenas dos minutos
				}
				else if(change_AS == 1){
					SetDDRamAddr(0x06); // colocar o cursor	no sitio das dezenas dos segundos
				}
			}
			if(change_A == 0){
				while(PORTAbits.RA4 && cursor_pos == 4); // carregar em S2 para indicar que se quer definir alarme
			}
			if(cursor_pos == 4){
				if(change_AH == 1){
					SetDDRamAddr(0x00); // colocar o cursor no sitio das dezenas das horas
				}
				Delay1KTCYx(200);
				change_A = 1;
				while(PORTAbits.RA4 && change_A == 1); // carregar em S2 uma vez para incrementar as horas
				if(mudei_ahoras == 1 && (alarme_hours_prev != alarme_hours || alarme_minutes_prev != alarme_minutes || alarme_seconds_prev != alarme_seconds )){
					mudei_ahoras = 0;
					update_EEPROM_interna_relogio_alarme();
					update_EEPROM_external(3);
				}
				if(change_A == 1){
					if(change_AH == 1){
						SetDDRamAddr(0x00);
						alarme_hours++;
						Delay1KTCYx(200);
						if (alarme_hours == 24){
							alarme_hours = 0;
						}
						alarme_hd = alarme_hours/10;
						alarme_hu = alarme_hours%10;
						while( BusyXLCD() );
						sprintf(change_hours_alarme, "%d%d", alarme_hd, alarme_hu);
			  		putsXLCD(change_hours_alarme);
					}
					else if(change_AM == 1){
						SetDDRamAddr(0x03);
						alarme_minutes++;
						Delay1KTCYx(200);
						if (alarme_minutes == 60){
							alarme_minutes = 0;
						}
						alarme_md = alarme_minutes/10;
						alarme_mu = alarme_minutes%10;
						while( BusyXLCD() );
						sprintf(change_minutes_alarme, "%d%d", alarme_md, alarme_mu);
			  		putsXLCD(change_minutes_alarme);
					}
					else if(change_AS == 1){
						SetDDRamAddr(0x06);
						alarme_seconds++;
						Delay1KTCYx(200);
						if (alarme_seconds == 60){
							alarme_seconds = 0;
						}
						alarme_sd = alarme_seconds/10;
						alarme_su = alarme_seconds%10;
						while( BusyXLCD() );
						sprintf(change_seconds_alarme, "%d%d", alarme_sd, alarme_su);
			  		putsXLCD(change_seconds_alarme);
					}
				}
			}
		}

		else if(cursor_pos == 5){ // acertar alarme da temperatura
			if(change_T == 0){
				SetDDRamAddr(0x0A);
			}
			else{
				SetDDRamAddr(0x40);
			}
			if(change_T == 0){
				while(PORTAbits.RA4 && cursor_pos == 5); // carregar em S2 para indicar que se quer definir alarme
			}
			if(cursor_pos == 5){
				SetDDRamAddr(0x40); // levar o cursor ate ao sitio onde aparece o nivel da temperatura
				Delay1KTCYx(200);
				change_T = 1;
				while(PORTAbits.RA4 && change_T == 1);
				if(mudei_atemp == 1 && alarme_temp_prev != alarme_temp){
					mudei_atemp = 0;
					update_EEPROM_interna_temp_alarme();
					update_EEPROM_external(4);
				}
				if(change_T == 1){
					alarme_temp++;
					if (alarme_temp == 40){
						alarme_temp = 10;
					}
					Delay1KTCYx(200);
					Td = alarme_temp/10;
					Tu = alarme_temp%10;
					while( BusyXLCD() );
					sprintf(nova_T, "%d%d", Td, Tu);
	  			putsXLCD(nova_T);
				}
			}
		}

		else if(cursor_pos == 6){ // acertar alarme da luminosidade
			if(change_L == 0){
				SetDDRamAddr(0x0B);
			}
			else{
				SetDDRamAddr(0x4F);
			}
			if(change_L == 0){
				while(PORTAbits.RA4 && cursor_pos == 6); // carregar em S2 para indicar que se quer definir alarme
			}
			if(cursor_pos == 6){
				SetDDRamAddr(0x4F); // levar o cursor ate ao sitio onde aparece o nivel da luminosidade
				Delay1KTCYx(200);
				change_L = 1;
				while(PORTAbits.RA4 && change_L == 1); // carregar novamente em S2 para incrementar "n" e definir o valor de luminosidade pretendido
				if(mudei_alum == 1 && alarme_lum_prev != alarme_lum){
					mudei_alum = 0;
					update_EEPROM_interna_lum_alarme();
					update_EEPROM_external(5);
				}
				if(change_L == 1){
					if(alarme_lum < 5){
						alarme_lum++;
					}
					else{
						alarme_lum = 0;
					}
					Delay1KTCYx(200);
					while( BusyXLCD() );
					sprintf(nova_L, "%d", alarme_lum);
	  			putsXLCD(nova_L);
				}
			}
		}

		else if(cursor_pos == 7){ // activar/desactivar alarmes
			SetDDRamAddr(0x0D);
			while(PORTAbits.RA4 && cursor_pos == 7 && alarme_OFF == 1);
			if(d_a_alarmes == 1 && alarmes_prev[0] != alarmes[0]){
				d_a_alarmes = 0;
				update_EEPROM_external(6);
			}
			if(cursor_pos == 7 && alarme_OFF == 1){
				Delay1KTCYx(200);
				if(alarmes[0] == 'a'){
					alarmes[0] = 'A';
				}
				else{
					alarmes[0] = 'a';
				}
				while( BusyXLCD() );
	  		putsXLCD(alarmes);
			}
		}

		else{ //activar modo poupanca de energia
			SetDDRamAddr(0x0F);
			if (modo_sleep == 0) while(PORTAbits.RA4 && cursor_pos == 8);
			if(cursor_pos == 8){
				modo_sleep = 1;
				Delay1KTCYx(200);
				while( BusyXLCD() );
				WriteCmdXLCD(DOFF);
				Sleep();
			}
		}
	}

	else{ // nao esta no modo modificao
		SetDDRamAddr(0x00);        // First line, first column
		while( BusyXLCD() );
		sprintf(time_s, "%d%d:%d%d:%d%d", hd, hu, md, mu, sd, su);
		putsXLCD(time_s);
	}
}

void rotina_sai_modificacao(void)
{

	if(sai_modificacao == 1){
		mudei_horas = 0;
		mudei_ahoras = 0;
		mudei_alum = 0;
		mudei_atemp = 0;
		d_a_alarmes = 0;
		alarme_temp_prev = alarme_temp;
		alarme_lum_prev = alarme_lum;
		alarme_hours_prev = alarme_hours;
		alarme_minutes_prev = alarme_minutes;
		alarme_seconds_prev = alarme_seconds;
		alarmes_prev[0] = alarmes[0];
		change_AH = 1;

		SetDDRamAddr(0x46);
		while( BusyXLCD() );
		putrsXLCD(" ");

		SetDDRamAddr(0x09);
		while( BusyXLCD() );
		sprintf(spaces, "       ");
		putsXLCD(spaces);

		SetDDRamAddr(0x4D);
		while( BusyXLCD() );
		sprintf(luminosidade, "L %d", n_lum);
		putsXLCD(luminosidade);

		SetDDRamAddr(0x40);
		while( BusyXLCD() );
		sprintf(temperatura_s, "%d C", (int)temperatura);
		putsXLCD(temperatura_s);

		sai_modificacao = 0;
	}
}
