#include "main.h"
#include "sensores.h"

int lum_2 = 0;
unsigned char inicio = 1;

unsigned char tsttc (void)
{
	unsigned char value;

	do{
		IdleI2C();
		StartI2C(); IdleI2C();
		WriteI2C(0x9a); IdleI2C();
		WriteI2C(0x01); IdleI2C();
		RestartI2C(); IdleI2C();
		WriteI2C(0x9a | 0x01); IdleI2C();
		value = ReadI2C(); IdleI2C();
		NotAckI2C(); IdleI2C();
		StopI2C();
	} while (!(value & 0x40));

	IdleI2C();
	StartI2C(); IdleI2C();
	WriteI2C(0x9a | 0x00); IdleI2C();
	WriteI2C(0x00); IdleI2C();
	RestartI2C(); IdleI2C();
	WriteI2C(0x9a | 0x01); IdleI2C();
	value = ReadI2C(); IdleI2C();
	NotAckI2C(); IdleI2C();
	StopI2C();

	return value;
}

void rotina_sensores_PMON(void){

  if (PMON != 0 && seconds%PMON == 0){
    temperatura = tsttc();
    Delay10TCYx(5);
    ConvertADC();
    while(BusyADC());
    lum_2 = ReadADC();
    if(lum_2 >= 0 && lum_2 <= 171){
      n_lum = 0;
    }
    else if(lum_2 <= 343){
      n_lum = 1;
    }
    else if(lum_2 <= 514){
      n_lum = 2;
    }
    else if(lum_2 <= 685){
      n_lum = 3;
    }
    else if(lum_2 <= 856){
      n_lum = 4;
    }
    else{
      n_lum = 5;
    }
    SetDDRamAddr(0x4D);
    while( BusyXLCD() );
    if (modo_modificacao == 0){
      sprintf(luminosidade, "L %d", n_lum);
      putsXLCD(luminosidade);
    }
    SetDDRamAddr(0x40);
    while( BusyXLCD() );
    if (modo_modificacao == 0){
      sprintf(temperatura_s, "%d C", (int)temperatura);
      putsXLCD(temperatura_s);
    }
    if(inicio == 1){
      inicio = 0;
      codigoev = 1;
      //update_EEPROM_external(codigoev);
    }
  }
}
