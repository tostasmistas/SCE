#include "main.h"
#include "memorias.h"

int NREG = 30;
int hours_interna = 0;
int minutes_interna = 0;
int seconds_interna = 0;
int alarme_hours_interna = 0;
int alarme_minutes_interna = 0;
int alarme_seconds_interna = 0;
int alarme_lum_interna = 0;
int alarme_temp_interna = 0;
int a = 0;

unsigned char indwrite = 0;
unsigned char full = 0;

char endereco = 1;
char palavra_magica = 0;
char palavra_magica_relogio_alarme = 0;
char palavra_magica_temp_alarme = 0;
char palavra_magica_lum_alarme = 0;
char palavra_magica_relogio = 0;
char palavra_magica_parametros = 0;
char checksum = 0;

char dataEEPROMext [8];

///////////////////////////////////////////////////////////////////////////////
//*************************** EEPROM Interna ********************************//
///////////////////////////////////////////////////////////////////////////////

void escrita_EEPROM_interna(char endereco, char dados)
{
	EEADR = endereco;
	EEDATA = dados;
	EECON1 &= 0x3F;
	EECON1 |= 0x04;
	INTCON &= 0x7F; // disable interrupts
	EECON2 = 0x55;
	EECON2 = 0xAA;
	EECON1 |= 0x02;
	INTCON |= 0x80; // enable interrupts
	EECON1 &= 0xFB;

	while((EECON1 & 0x02) == 2); // testar se a escrita esta concluida
}

char ler_EEPROM_interna(char endereco)
{
	char dados;

	EEADR = endereco;
	EECON1 &= 0x3F;
	EECON1 |= 0x01;
	dados = EEDATA;

	return dados;
}

void calcular_palavra_magica(void){

	palavra_magica = 0;

	palavra_magica = palavra_magica_relogio_alarme + palavra_magica_temp_alarme + palavra_magica_lum_alarme + palavra_magica_relogio + palavra_magica_parametros;

	escrita_EEPROM_interna(0x0B, palavra_magica);
}


unsigned char verificar_checksum(void)
{
	checksum = 0;
	a = 0;

	palavra_magica = ler_EEPROM_interna(0x0B);

	for(a = 0; a <= 10; a++){
		checksum += ler_EEPROM_interna(0x00+a);
	}

	if(checksum == palavra_magica){
		return 1;
	}
	else{
		return 0;
	}
}

void update_EEPROM_interna_relogio_alarme (void)
{
	a = 0;

	palavra_magica_relogio_alarme = 0;

	alarme_hours_interna = ler_EEPROM_interna(0x00);

	if(alarme_hours != alarme_hours_interna){
		escrita_EEPROM_interna(0x00, alarme_hours);
	}

	alarme_minutes_interna = ler_EEPROM_interna(0x01);

	if(alarme_minutes != alarme_minutes_interna){
		escrita_EEPROM_interna(0x01, alarme_minutes);
	}

	alarme_seconds_interna = ler_EEPROM_interna(0x02);

	if(alarme_seconds != alarme_seconds_interna){
		escrita_EEPROM_interna(0x02, alarme_seconds);
	}

	for(a = 0; a <= 2; a++){
		palavra_magica_relogio_alarme += ler_EEPROM_interna(0x00+a);
	}

	calcular_palavra_magica();
}

void ler_EEPROM_interna_relogio_alarme (void)
{
	alarme_hours = ler_EEPROM_interna(0x00);
	alarme_hd = alarme_hours/10;
	alarme_hu = alarme_hours%10;

	alarme_minutes = ler_EEPROM_interna(0x01);
	alarme_md = alarme_minutes/10;
	alarme_mu = alarme_minutes%10;

	alarme_seconds = ler_EEPROM_interna(0x02);
	alarme_sd = alarme_seconds/10;
	alarme_su = alarme_seconds%10;
}

void update_EEPROM_interna_temp_alarme (void)
{
	alarme_temp_interna = ler_EEPROM_interna(0x03);

	if(alarme_temp != alarme_temp_interna){
		escrita_EEPROM_interna(0x03, alarme_temp);
	}

	palavra_magica_temp_alarme = ler_EEPROM_interna(0x03);

	calcular_palavra_magica();
}

void ler_EEPROM_interna_temp_alarme (void)
{
	alarme_temp = ler_EEPROM_interna(0x03);
}

void update_EEPROM_interna_lum_alarme (void)
{
	alarme_lum_interna = ler_EEPROM_interna(0x04);

	if(alarme_lum != alarme_lum_interna){
		escrita_EEPROM_interna(0x04, alarme_lum);
	}

	palavra_magica_lum_alarme = ler_EEPROM_interna(0x04);

	calcular_palavra_magica();
}

void ler_EEPROM_interna_lum_alarme (void)
{
	alarme_lum = ler_EEPROM_interna(0x04);
}

void update_EEPROM_interna_relogio (void)
{
	//hours_interna = ler_EEPROM_interna(0x05);
	//if(hours != hours_interna){
		escrita_EEPROM_interna(0x05, hours);
	//}

	//minutes_interna = ler_EEPROM_interna(0x06);
	//if(minutes != minutes_interna){
		escrita_EEPROM_interna(0x06, minutes);
	//}

	//seconds_interna = ler_EEPROM_interna(0x05);
	//if(seconds != seconds_interna){
		escrita_EEPROM_interna(0x07, seconds);
	//}

	a = 0;
	palavra_magica_relogio = 0;

	for(a = 0; a <= 2; a++){
		palavra_magica_relogio += ler_EEPROM_interna(0x05+a);
	}

	calcular_palavra_magica();
}

void ler_EEPROM_interna_relogio (void)
{
	hours = ler_EEPROM_interna(0x05);
	hd = hours/10;
	hu = hours%10;

	minutes = ler_EEPROM_interna(0x06);
	md = minutes/10;
	mu = minutes%10;

	seconds = ler_EEPROM_interna(0x07);
	sd = seconds/10;
	su = seconds%10;
}

void update_EEPROM_interna_parametros (void)
{
	a = 0;

	palavra_magica_parametros = 0;

	/* update valor de NREG */
	escrita_EEPROM_interna(0x08, 30);

	/* update valor de PMON */
	escrita_EEPROM_interna(0x09, 5);

	/* update valor de TSOM */
	escrita_EEPROM_interna(0x0A, 2);

	for(a = 0; a <= 2; a++){
		palavra_magica_parametros += ler_EEPROM_interna(0x08+a);
	}

	calcular_palavra_magica();
}

void ler_EEPROM_interna_parametros (void)
{
	/* ler valor de NREG */
	NREG = ler_EEPROM_interna(0x08);

	/* ler valor de PMON */
	PMON = ler_EEPROM_interna(0x09);

	/* ler valor de TSOM */
	TSOM = ler_EEPROM_interna(0x0A);
}

///////////////////////////////////////////////////////////////////////////////
//*************************** EEPROM Externa ********************************//
///////////////////////////////////////////////////////////////////////////////

void init_EEPROM_externa(void){

  dataEEPROMext[0] = NREG;
  dataEEPROMext[1] = 0;
  dataEEPROMext[2] = 0;
  dataEEPROMext[3] = 0;
  dataEEPROMext[4] = 0;
  dataEEPROMext[5] = 0;
  dataEEPROMext[6] = 0;
  dataEEPROMext[7] = 0;
  writeEEPROMexterna(0x00,dataEEPROMext); //Cabeçalho NREG
  EEAckPolling(0xA0);
}

void writeEEPROMexterna (char endereco, char data[8])
{
	char ind = 0;
	char i = 0;
		IdleI2C();
		StartI2C(); IdleI2C();

		WriteI2C(0xA0); IdleI2C();
		WriteI2C(0x00); IdleI2C();   //HB
		WriteI2C(endereco); IdleI2C();   //LB
		for (ind = 0; ind < 8; ind++){
			WriteI2C(data[ind]); IdleI2C(); //Dados
		}
		//NotAckI2C(); IdleI2C();
		StopI2C();

}

void update_EEPROM_external(char codigoev)
{
	indwrite = endereco*8;
	dataEEPROMext[0] = hours;
	dataEEPROMext[1] = minutes;
	dataEEPROMext[2] = seconds;
	dataEEPROMext[3] = codigoev;

	//SetDDRamAddr(0x46);

	switch(codigoev){
		case 1:
			//while( BusyXLCD() );
			//putrsXLCD("I");
			dataEEPROMext[4] = (int)temperatura;
			dataEEPROMext[5] = 0;
			dataEEPROMext[6] = 0;
			dataEEPROMext[7] = 0;
			writeEEPROMexterna(indwrite,dataEEPROMext);
			EEAckPolling(0xA0);
		break;
		case 2:
			//while( BusyXLCD() );
			//putrsXLCD("N");
			dataEEPROMext[4] = hours;
			dataEEPROMext[5] = minutes;
			dataEEPROMext[6] = seconds;
			dataEEPROMext[7] = 0;
			writeEEPROMexterna(indwrite,dataEEPROMext); //Temperatura
			EEAckPolling(0xA0);

		break;
		case 3:
			//while( BusyXLCD() );
			//putrsXLCD("h");
			dataEEPROMext[4] = alarme_hours;
			dataEEPROMext[5] = alarme_minutes;
			dataEEPROMext[6] = alarme_seconds;
			dataEEPROMext[7] = 0;
			writeEEPROMexterna(indwrite,dataEEPROMext); //Temperatura
			EEAckPolling(0xA0);

		break;
		case 4:
			//while( BusyXLCD() );
			//putrsXLCD("t");
			dataEEPROMext[4] = alarme_temp;
			dataEEPROMext[5] = 0;
			dataEEPROMext[6] = 0;
			dataEEPROMext[7] = 0;
			writeEEPROMexterna(indwrite,dataEEPROMext); //Temperatura
			EEAckPolling(0xA0);

		break;
		case 5:
			//while( BusyXLCD() );
			//putrsXLCD("l");
			dataEEPROMext[4]=alarme_lum;
			dataEEPROMext[5]=0;
			dataEEPROMext[6]=0;
			dataEEPROMext[7]=0;
			writeEEPROMexterna(indwrite,dataEEPROMext); //Temperatura
			EEAckPolling(0xA0);

		break;
		case 6:
			//while( BusyXLCD() );
			//putrsXLCD("A");
			dataEEPROMext[4] = (int)temperatura;
			dataEEPROMext[5] = n_lum;
			dataEEPROMext[6] = 0;
			dataEEPROMext[7] = 0;
			writeEEPROMexterna(indwrite,dataEEPROMext); //Temperatura
			EEAckPolling(0xA0);

		break;
		case 7:
			//while( BusyXLCD() );
			//putrsXLCD("H");
			dataEEPROMext[4] = (int)temperatura;
			dataEEPROMext[5] = n_lum;
			dataEEPROMext[6] = 0;
			dataEEPROMext[7] = 0;
			writeEEPROMexterna(indwrite,dataEEPROMext); //Temperatura
			EEAckPolling(0xA0);

		break;
		case 8:
			//while( BusyXLCD() );
			//putrsXLCD("T");
			dataEEPROMext[4] = (int)temperatura;
			dataEEPROMext[5] = n_lum;
			dataEEPROMext[6] = 0;
			dataEEPROMext[7] = 0;

			writeEEPROMexterna(indwrite,dataEEPROMext); //Temperatura
			EEAckPolling(0xA0);

			break;
		case 9:
			//while( BusyXLCD() );
			//putrsXLCD("L");
			dataEEPROMext[4] = (int)temperatura;
			dataEEPROMext[5] = n_lum;
			dataEEPROMext[6] = 0;
			dataEEPROMext[7] = 0;
			writeEEPROMexterna(indwrite,dataEEPROMext); //Temperatura
			EEAckPolling(0xA0);

		break;
	}

	endereco++;
	dataEEPROMext[0] = NREG;
	dataEEPROMext[1] = endereco;
	dataEEPROMext[2] = 0;
	dataEEPROMext[3] = 0;
	dataEEPROMext[4] = 0;
	dataEEPROMext[5] = 0;
	dataEEPROMext[6] = 0;
	dataEEPROMext[7] = 0;
	writeEEPROMexterna(0x00,dataEEPROMext); //Cabeçalho NREG
	EEAckPolling(0xA0);
	if(endereco == NREG + 1){
		endereco = 1;
	}
	if(endereco > NREG/2 && full == 0){
		full = 1;
		SetDDRamAddr(0x48);
		while( BusyXLCD() );
		putrsXLCD((const far rom char*)"M");
	}
}
