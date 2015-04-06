#include <p18f452.h>  /* for the special function register declarations */
#include <portb.h>    /* for the RB0/INT0 interrupt */
#include <adc.h>      /*for the ADC*/
#include <xlcd.h>
#include <timers.h>
#include <delays.h>
#include <stdio.h>
#include <i2c.h>
#include <stdlib.h>

int NREG = 0;
int PMON = 0;
int TSOM = 0;

volatile int hours = 0; //compiler users guide page vinte e oito
int hours_interna = 0;
volatile int minutes = 0;
int minutes_interna = 0;
volatile int seconds = 0;
int seconds_interna = 0;
int alarme_hours = 0;
int alarme_hours_interna = 0;
int alarme_minutes = 0;
int alarme_minutes_interna = 0;
int alarme_seconds = 0;
int alarme_seconds_interna = 0;
int seconds_alarme_TSOM = 0;
int sd = 0;
int su = 0;
int md = 0;
int mu = 0;
int hd = 0;
int hu = 0;
int Td = 0;
int Tu = 0;
volatile int cursor_pos = 0;
int lum_2 = 0;
int n_lum = 0;
int alarme_hours_prev = 0;
int alarme_minutes_prev = 0;
int alarme_seconds_prev = 0;
int alarme_lum = 0;
int alarme_lum_interna = 0;
int alarme_temp = 20;
int alarme_temp_interna = 0;
int alarme_sd = 0;
int alarme_su = 0;
int alarme_md = 0;
int alarme_mu = 0;
int alarme_hd = 0;
int alarme_hu = 0;
int alarme_lum_prev = 0;
int alarme_temp_prev = 20;

/* flags */
volatile char modo_sleep = 0;
volatile char modo_modificacao = 0;
volatile char change_L = 0;
volatile char change_T = 0;
volatile char change_A = 0;
volatile char change_AS = 0;
volatile char change_AM = 0;
volatile char change_AH = 1;
volatile char sai_modificacao = 0;
char up_L = 0;
char up_T = 0;
volatile char alarme_OFF = 1;
volatile char alarme_lum_ON = 0;
volatile char alarme_temp_ON = 0;
volatile char alarme_clock_ON = 0;
volatile char desliga_alarmes = 0;
char first_iteration = 1;
char temperatura = 0;
char nova_L = 0;
char nova_T = 0;
char inicio = 1;
char codigoev = 0;
char indwrite = 0;
char endereco = 0;
char mudei_horas = 0;
volatile char mudei_ahoras = 0;
volatile char mudei_alum = 0;
volatile char mudei_atemp = 0;
volatile char d_a_alarmes = 0;
char disp_ahoras = 1;

/* strings */
char time[9];
char change_hours[3];
char change_minutes[3];
char change_seconds[3];
char change_hours_alarme[3];
char change_minutes_alarme[3];
char change_seconds_alarme[3];

char temperatura_s[5];
char luminosidade [4];
char temp[3];
char spaces[8];
char alarmes[2];
char alarmes_prev[2];
char spaces_alarmes[4];

void DelayFor18TCY( void )
{
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
	Nop();
}

void DelayPORXLCD( void )
{
	Delay1KTCYx(60);
  	 return;
}

void DelayXLCD( void )
{
	Delay1KTCYx(20);
  	return;
}

unsigned char tsttc (void){
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

void InitializeBuzzer (void)
{
    T2CON = 0x05;		/* postscale 1:1, Timer2 ON, prescaler 4 */
    TRISCbits.TRISC2 = 0;  /* configure the CCP1 module for the buzzer */
    PR2 = 0x80;		/* initialize the PWM period */
    CCPR1L = 0x80;		/* initialize the PWM duty cycle */
}

void isr (void);  /* prototype needed for 'goto' below */
/*
* For high interrupts, control is transferred to address 0x8.
*/
#pragma code HIGH_INTERRUPT_VECTOR = 0x8
void high_ISR (void)
{
 _asm
   goto isr
 _endasm
}
#pragma code  /* allow the linker to locate the remaining code */

///////////////////////////////////////////////////////////////////////////////
//****************************** Interrupção ********************************//
///////////////////////////////////////////////////////////////////////////////

#pragma interrupt isr
void isr (void)
{
///////////////////////////////////////////////////////////////////////////////
//******************************** Relógio **********************************//
///////////////////////////////////////////////////////////////////////////////
	if(PIR1bits.TMR1IF == 1){ // timer interrupt
		WriteTimer1( 0x8000 ); // reload timer: 1 second
		PIR1bits.TMR1IF = 0; /* clear flag to avoid another interrupt */
		seconds++;
		if(disp_ahoras==0){
			disp_ahoras=1;
		}
		if(seconds == 60){
			seconds = 0;
			minutes++;
			if(minutes == 60){
				minutes = 0;
				hours++;
				if(hours == 24){
					hours = 0;
				}
			}
	  	}
	}

///////////////////////////////////////////////////////////////////////////////
//***************************** Botão S3 ************************************//
//**************** Entrar ou sair do modo de modificação ********************//
//************************** Mudar o cursor *********************************//
///////////////////////////////////////////////////////////////////////////////

	if (INTCONbits.INT0IF == 1){ // button S3 interrupt
		INTCONbits.INT0IF = 0;
		if(alarme_OFF==0){ //alarmes estao ligados
			alarme_OFF = 1; //desligar alarmes
			alarme_lum_ON = 0;
			alarme_temp_ON = 0;
			alarme_clock_ON = 0;
			desliga_alarmes = 1;
		}else{
			if(cursor_pos == 4 && change_A == 1){
				if(change_AH == 1){ // acabei de mudar as horas do alarme
					change_AM = 1;
					change_AH = 0;
					SetDDRamAddr(0x03);
				}
				else if(change_AM == 1){ // acabei de mudar os minutos do alarme
					change_AS = 1;
					change_AM = 0;
					SetDDRamAddr(0x06);
				}
				else if(change_AS == 1){ // acabei de mudar os segundos do alarme
					change_AS = 0;
					change_A = 0; // ja defini o alarme do relogio
					disp_ahoras = 1;
					cursor_pos = 5;
					mudei_ahoras = 1; //flag para EEPROM
				}
			}
			else{
				cursor_pos++;
				if (cursor_pos >=1 && cursor_pos <= 8){
					modo_modificacao = 1;
					if(cursor_pos==8){
						d_a_alarmes = 1;
					}
					if(change_L == 1){ //acabei de definir a luminosidade do alarme
						change_L = 0;
						mudei_alum = 1;
					}
					if(change_T == 1){ //acabei de definir a temperatura do alarme
						change_T = 0;
						mudei_atemp = 1;
					}
				}else{
					modo_modificacao = 0; //sair do modo de modificacaoo
					sai_modificacao = 1; //acabei de sair do modo de modificacaoo
					cursor_pos = 0;
				}
				if (modo_sleep == 1){
					modo_sleep = 0; //sair do modo sleep
					cursor_pos = 0;	//nao estamos em modo modificacao
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//************************* Fim da Interrupção ******************************//
///////////////////////////////////////////////////////////////////////////////

void EnableHighInterrupts (void)
{
	RCONbits.IPEN = 1;    /* enable interrupt priority levels */
	INTCONbits.GIEH = 1;  /* enable all high priority interrupts */
}

///////////////////////////////////////////////////////////////////////////////
//*************************** EEPROM Interna ********************************//
///////////////////////////////////////////////////////////////////////////////

void update_EEPROM_interna_relogio_alarme (void)
{
	/* ler valor do alarme do relogio actualmente armazenado na EEPROM interna */
	EEADR = 0x00; // write the address of the memory location to be read
	EECON1 = 0x01;
	alarme_hours_interna = EEDATA;

	if(alarme_hours != alarme_hours_interna){
		/* escrever novo valor do alarme do relogio na EEPROM interna */
		EEADR = 0x00;
		EEDATA = alarme_hours;
		EECON1 |= 0x04;
		INTCON &= 0x7F; // disable interrupts
		EECON2 = 0x55;
		EECON2 = 0xAA;
		EECON1 = 0x02;
		INTCON |= 0x80; // enable interrupts
		EECON1 = 0x00;
	}

	EEADR = 0x01;
	EECON1 = 0x01;
	alarme_minutes_interna = EEDATA;

	if(alarme_minutes != alarme_minutes_interna){
		EEADR = 0x01;
		EEDATA = alarme_minutes;
		EECON1 |= 0x04;
		INTCON &= 0x7F; // disable interrupts
		EECON2 = 0x55;
		EECON2 = 0xAA;
		EECON1 = 0x02;
		INTCON |= 0x80; // enable interrupts
		EECON1 = 0x00;
	}

	EEADR = 0x02;
	EECON1 = 0x01;
	alarme_seconds_interna = EEDATA;

	if(alarme_seconds != alarme_seconds_interna){
		EEADR = 0x02;
		EEDATA = alarme_seconds;
		EECON1 |= 0x04;
		INTCON &= 0x7F; // disable interrupts
		EECON2 = 0x55;
		EECON2 = 0xAA;
		EECON1 = 0x02;
		INTCON |= 0x80; // enable interrupts
		EECON1 = 0x00;
	}
}

void ler_EEPROM_interna_relogio_alarme (void)
{
	EEADR = 0x00;
	EECON1 = 0x01;
	alarme_hours = EEDATA;

	EEADR = 0x01;
	EECON1 = 0x01;
	alarme_minutes = EEDATA;

	EEADR = 0x02;
	EECON1 = 0x01;
	alarme_seconds = EEDATA;
}

void update_EEPROM_interna_temp_alarme (void)
{
	EEADR = 0x03;
	EECON1 = 0x01;
	alarme_temp_interna = EEDATA;

	if(alarme_temp != alarme_temp_interna){
		EEADR = 0x03;
		EEDATA = alarme_temp;
		EECON1 |= 0x04;
		INTCON &= 0x7F; // disable interrupts
		EECON2 = 0x55;
		EECON2 = 0xAA;
		EECON1 = 0x02;
		INTCON |= 0x80; // enable interrupts
		EECON1 = 0x00;
	}
}

void ler_EEPROM_interna_temp_alarme (void)
{
	EEADR = 0x03;
	EECON1 = 0x01;
	alarme_temp = (int)EEDATA;
}

void update_EEPROM_interna_lum_alarme (void)
{
	EEADR = 0x04;
	EECON1 = 0x01;
	alarme_lum_interna = EEDATA;

	if(alarme_lum != alarme_lum_interna){
		EEADR = 0x04;
		EEDATA = alarme_lum;
		EECON1 |= 0x04;
		INTCON &= 0x7F; // disable interrupts
		EECON2 = 0x55;
		EECON2 = 0xAA;
		EECON1 = 0x02;
		INTCON |= 0x80; // enable interrupts
		EECON1 = 0x00;
	}
}

void ler_EEPROM_interna_lum_alarme (void)
{
	EEADR = 0x04;
	EECON1 = 0x01;
	alarme_lum = (int)EEDATA;
}

void update_EEPROM_interna_relogio (void)
{
	EEADR = 0x05;
	EECON1 = 0x01;
	hours_interna = EEDATA;

	if(hours != hours_interna){
		EEADR = 0x05;
		EEDATA = hours;
		EECON1 |= 0x04;
		INTCON &= 0x7F; // disable interrupts
		EECON2 = 0x55;
		EECON2 = 0xAA;
		EECON1 = 0x02;
		INTCON |= 0x80; // enable interrupts
		EECON1 = 0x00;
	}

	EEADR = 0x06;
	EECON1 = 0x01;
	minutes_interna = EEDATA;

	if(minutes != minutes_interna){
		EEADR = 0x06;
		EEDATA = minutes;
		EECON1 |= 0x04;
		INTCON &= 0x7F; // disable interrupts
		EECON2 = 0x55;
		EECON2 = 0xAA;
		EECON1 = 0x02;
		INTCON |= 0x80; // enable interrupts
		EECON1 = 0x00;
	}

	EEADR = 0x07;
	EECON1 = 0x01;
	seconds_interna = EEDATA;

	if(seconds != seconds_interna){
		EEADR = 0x07;
		EEDATA = seconds;
		EECON1 |= 0x04;
		INTCON &= 0x7F; // disable interrupts
		EECON2 = 0x55;
		EECON2 = 0xAA;
		EECON1 = 0x02;
		INTCON |= 0x80; // enable interrupts
		EECON1 = 0x00;
	}
}

void update_EEPROM_interna_parametros (void)
{
		/* update valor de NREG */
		EEADR = 0x08;
		EEDATA = 30;
		EECON1 |= 0x04;
		INTCON &= 0x7F; // disable interrupts
		EECON2 = 0x55;
		EECON2 = 0xAA;
		EECON1 = 0x02;
		INTCON |= 0x80; // enable interrupts
		EECON1 = 0x00;

		Delay1KTCYx(200);

		/* update valor de PMON */
		EEADR = 0x09;
		EEDATA = 5;
		EECON1 |= 0x04;
		INTCON &= 0x7F; // disable interrupts
		EECON2 = 0x55;
		EECON2 = 0xAA;
		EECON1 = 0x02;
		INTCON |= 0x80; // enable interrupts
		EECON1 = 0x00;

		Delay1KTCYx(200);

		/* update valor de TSOM */
		EEADR = 0x0A;
		EEDATA = 2;
		EECON1 |= 0x04;
		INTCON &= 0x7F; // disable interrupts
		EECON2 = 0x55;
		EECON2 = 0xAA;
		EECON1 = 0x02;
		INTCON |= 0x80; // enable interrupts
		EECON1 = 0x00;
}

void ler_EEPROM_interna_parametros (void)
{
	EEADR = 0x08;
	EECON1 = 0x01;
	NREG = EEDATA;

	EEADR = 0x09;
	EECON1 = 0x01;
	PMON = EEDATA;

	EEADR = 0x0A;
	EECON1 = 0x01;
	TSOM = EEDATA;
}


///////////////////////////////////////////////////////////////////////////////
//*************************** EEPROM Externa ********************************//
///////////////////////////////////////////////////////////////////////////////

void update_EEPROM_external(char codigoev)
{
	indwrite=endereco*8;
	EEByteWrite(0xA0, 0x30, 0xA5);
	EEAckPolling(0xA0);

	SetDDRamAddr(0x46);
	EEByteWrite(0xA0,indwrite+4,hours); //Hours
	EEAckPolling(0xA0);

	EEByteWrite(0xA0,indwrite+5,minutes); //Minutes
	EEAckPolling(0xA0);
	EEByteWrite(0xA0,indwrite+6,seconds); //Seconds
	EEAckPolling(0xA0);
	EEByteWrite(0xA0,indwrite+7,codigoev); //Código do Evento
	EEAckPolling(0xA0);

	switch(codigoev){
		case 1:
			while( BusyXLCD() );
			putrsXLCD("I");
			EEByteWrite(0xA0,indwrite+8,(int)temperatura); //Temperatura
			EEAckPolling(0xA0);
			EEByteWrite(0xA0,indwrite+9,n_lum); //Luminosidade
			EEAckPolling(0xA0);
			EEByteWrite(0xA0,indwrite+10,0x00);
			EEAckPolling(0xA0);
			EEByteWrite(0xA0,indwrite+11,0x00);
			EEAckPolling(0xA0);
		break;
		case 2:
			while( BusyXLCD() );
			putrsXLCD("N");
			EEByteWrite(0xA0,indwrite+8,hours); //Nova Hora
			EEAckPolling(0xA0);
			EEByteWrite(0xA0,indwrite+9,minutes); //Minutes
			EEAckPolling(0xA0);
			EEByteWrite(0xA0,indwrite+10,seconds); //Seconds
			EEAckPolling(0xA0);
			EEByteWrite(0xA0,indwrite+11,0x00);
			EEAckPolling(0xA0);
		break;
		case 3:
			while( BusyXLCD() );
			putrsXLCD("h");
			EEByteWrite(0xA0,indwrite+8,alarme_hours); //Nova Hora Alarme
			EEAckPolling(0xA0);
			EEByteWrite(0xA0,indwrite+9,alarme_minutes); //Minutes Alarme
			EEAckPolling(0xA0);
			EEByteWrite(0xA0,indwrite+10,alarme_seconds); //Seconds Alarme
			EEAckPolling(0xA0);
			EEByteWrite(0xA0,indwrite+11,0x00);
			EEAckPolling(0xA0);
		break;
		case 4:
			while( BusyXLCD() );
			putrsXLCD("t");
			EEByteWrite(0xA0,indwrite+8,alarme_temp); //Alarme Temperatura
			EEAckPolling(0xA0);
			EEByteWrite(0xA0,indwrite+9,0x00);
			EEAckPolling(0xA0);
			EEByteWrite(0xA0,indwrite+10,0x00);
			EEAckPolling(0xA0);
			EEByteWrite(0xA0,indwrite+11,0x00);
			EEAckPolling(0xA0);
		break;
		case 5:
			while( BusyXLCD() );
			putrsXLCD("l");
			EEByteWrite(0xA0,indwrite+8,alarme_lum); //Alarme Luminosidade
			EEAckPolling(0xA0);
			EEByteWrite(0xA0,indwrite+9,0x00);
			EEAckPolling(0xA0);
			EEByteWrite(0xA0,indwrite+10,0x00);
			EEAckPolling(0xA0);
			EEByteWrite(0xA0,indwrite+11,0x00);
			EEAckPolling(0xA0);
		break;
		case 6:
			while( BusyXLCD() );
			putrsXLCD("A");
 		  EEByteWrite(0xA0,indwrite+8,(int)temperatura); //Temperatura
  	  EEAckPolling(0xA0);
   	  EEByteWrite(0xA0,indwrite+9,n_lum); //Luminosidade
			EEAckPolling(0xA0);
			EEByteWrite(0xA0,indwrite+10,0x00);
			EEAckPolling(0xA0);
			EEByteWrite(0xA0,indwrite+11,0x00);
			EEAckPolling(0xA0);
		break;
		case 7:
			while( BusyXLCD() );
			putrsXLCD("H");
			EEByteWrite(0xA0,indwrite+8,(int)temperatura); //Temperatura
			EEAckPolling(0xA0);
			EEByteWrite(0xA0,indwrite+9,n_lum); //Luminosidade
			EEAckPolling(0xA0);
			EEByteWrite(0xA0,indwrite+10,0x00);
			EEAckPolling(0xA0);
			EEByteWrite(0xA0,indwrite+11,0x00);
			EEAckPolling(0xA0);
		break;
		case 8:
			while( BusyXLCD() );
			putrsXLCD("T");
			EEByteWrite(0xA0,indwrite+8,(int)temperatura); //Temperatura
			EEAckPolling(0xA0);
			EEByteWrite(0xA0,indwrite+9,n_lum); //Luminosidade
			EEAckPolling(0xA0);
			EEByteWrite(0xA0,indwrite+10,0x00);
			EEAckPolling(0xA0);
			EEByteWrite(0xA0,indwrite+11,0x00);
			EEAckPolling(0xA0);
		break;
		case 9:
			while( BusyXLCD() );
			putrsXLCD("L");
			EEByteWrite(0xA0,indwrite+8,(int)temperatura); //Temperatura
			EEAckPolling(0xA0);
			EEByteWrite(0xA0,indwrite+9,n_lum); //Luminosidade
			EEAckPolling(0xA0);
			EEByteWrite(0xA0,indwrite+10,0x00);
			EEAckPolling(0xA0);
			EEByteWrite(0xA0,indwrite+11,0x00);
			EEAckPolling(0xA0);
		break;
	}

	endereco ++;
	EEByteWrite(0xA0,0x01,endereco); //Cabeçalho Indíce Escrita
	EEAckPolling(0xA0);
	if(endereco == NREG){
		endereco = 0;
	}
}

///////////////////////////////////////////////////////////////////////////////
//************************* Programa Principal ******************************//
///////////////////////////////////////////////////////////////////////////////

void main (void)
{
	/* TIMER 1*/
	OpenTimer1(	TIMER_INT_ON   &
							T1_16BIT_RW    &
      				T1_SOURCE_EXT  &
      				T1_PS_1_1      &
      				T1_OSC1EN_ON   &
      				T1_SYNC_EXT_ON );// tem que estar off para correr na placa, mas on para correr no proteus

	ADCON1 = 0x0E; // Port A: A0 - analog; A1-A7 - digital

	OpenADC (ADC_FOSC_RC & ADC_RIGHT_JUST & ADC_1ANA_0REF, ADC_CH0 & ADC_INT_OFF);

	/* BUTTON S3 */
	OpenRB0INT (	PORTB_CHANGE_INT_ON & /* enable the RB0/INT0 interrupt */
                PORTB_PULLUPS_ON &    /* configure the RB0 pin for input */
                FALLING_EDGE_INT);    /* trigger interrupt upon S3 button depression */


	OpenXLCD( FOUR_BIT & LINES_5X7 ); // 4-bit data interface; 2 x 16 characters

	OpenI2C(MASTER, SLEW_ON);// Initialize I2C module
	SSPADD = 9; //400kHz Baud clock(9) @16MHz
	//100kHz Baud clock(39) @16MHz

	//InitializeBuzzer();

	EnableHighInterrupts();

	alarmes[0] = 'a';
	alarmes[1] = 0;
	alarmes_prev[0] = 'a';
	alarmes_prev[1] = 0;

	update_EEPROM_interna_parametros();
	ler_EEPROM_interna_parametros(); // carregar da EEPROM interna os valores de NREG, PMON e TSOM

	ler_EEPROM_interna_relogio_alarme(); // carregar da EEPROM interna o valor do alarme do relogio
	//ler_EEPROM_interna_temp_alarme(); // carregar da EEPROM interna o valor do alarme da temperatura
	ler_EEPROM_interna_lum_alarme(); // carregar da EEPROM interna o valor do alarme da luminosidade


 	WriteTimer1( 0x8000 ); // load timer: 1 second

	//EEPROM External Init
	EEByteWrite(0xA0,0x00,0x1E); //Cabeçalho NREG
	EEAckPolling(0xA0);
	EEByteWrite(0xA0,0x01,0x00); //Cabeçalho Indíce Escrita
	EEAckPolling(0xA0);
	EEByteWrite(0xA0,0x02,0x00); //Cabeçalho Indíce Leitura
	EEAckPolling(0xA0);
	EEByteWrite(0xA0,0x03,0x00); //Cabeçalho Número Registos Válidos
	EEAckPolling(0xA0);

///////////////////////////////////////////////////////////////////////////////
//*************************** Ciclo Principal *******************************//
///////////////////////////////////////////////////////////////////////////////

	while (1){

		while(modo_sleep == 1){
    	WriteCmdXLCD( DOFF );      // Turn display off
		}

 		while( BusyXLCD() );
  	WriteCmdXLCD( DOFF );      // Turn display off
  	while( BusyXLCD() );
  	WriteCmdXLCD( CURSOR_OFF );// Enable display with no cursor
  	while( BusyXLCD() );

		sd = seconds/10;
		su = seconds%10;
		md = minutes/10;
		mu = minutes%10;
		hd = hours/10;
		hu = hours%10;

		//update_EEPROM_interna_relogio();

		SetDDRamAddr(0x0D);
		putsXLCD(alarmes);

///////////////////////////////////////////////////////////////////////////////
//************************** Modo de Modificação ****************************//
///////////////////////////////////////////////////////////////////////////////

		if(modo_modificacao == 1){ // estamos em modo modificacao

			if(cursor_pos>=4 ){
				SetDDRamAddr(0x00);        // First line, first column
				while( BusyXLCD() );
				sprintf(time, "%d%d:%d%d:%d%d", alarme_hd, alarme_hu, alarme_md, alarme_mu, alarme_sd, alarme_su);
				putsXLCD(time);
			}

			SetDDRamAddr(0x4D);
			while( BusyXLCD() );
			sprintf(luminosidade, "L %d", alarme_lum);
			putsXLCD(luminosidade);

			SetDDRamAddr(0x40);
			while( BusyXLCD() );
			sprintf(temperatura_s, "%d C", alarme_temp);
			putsXLCD(temperatura_s);

			SetDDRamAddr(0x0F);
			while( BusyXLCD() );
			putrsXLCD("P"); // letra P que so aparece quando em modo modificacaoo

			SetDDRamAddr(0x09);
			while( BusyXLCD() );
			putrsXLCD("ATL"); // letras ATL que aparecem quando em modo modificaoo	para se definir os alarmes

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
				if(cursor_pos==4 && mudei_horas==1){
					mudei_horas==0;
					update_EEPROM_external(2);
				}
				if(cursor_pos == 3){
					seconds++;
					mudei_horas=1;
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
					if(mudei_ahoras==1 && (alarme_hours_prev!=alarme_hours || alarme_minutes_prev!=alarme_minutes || alarme_seconds_prev!=alarme_seconds )){
						mudei_ahoras=0;
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
				update_EEPROM_interna_relogio_alarme();
			}

			else if(cursor_pos == 5){ // acertar alarme da temperatura
				if(change_T == 0){
					SetDDRamAddr(0x0A);
				}
				else{
					SetDDRamAddr(0x40);
				}
				while(PORTAbits.RA4 && cursor_pos == 5); // carregar em S2 para indicar que se quer definir alarme
				if(mudei_atemp==1 && alarme_temp_prev!=alarme_temp){
					mudei_atemp=0;
					update_EEPROM_external(4);
				}
				if(cursor_pos == 5){
					SetDDRamAddr(0x40); // levar o cursor ate ao sitio onde aparece o nivel da temperatura
					Delay1KTCYx(200);
					change_T = 1;
				//	while(PORTAbits.RA4 && change_T == 1);
					if(change_T == 1){
						alarme_temp++;
						if (alarme_temp == 50){
							alarme_temp = 0;
						}
						Delay1KTCYx(200);
						Td = alarme_temp/10;
						Tu = alarme_temp%10;
						while( BusyXLCD() );
						sprintf(nova_T, "%d%d", Td, Tu);
		  			putsXLCD(nova_T);
					}
				}
				update_EEPROM_interna_temp_alarme();
			}

			else if(cursor_pos == 6){ // acertar alarme da luminosidade
				if(change_L == 0){
					SetDDRamAddr(0x0B);
				}
				else{
					SetDDRamAddr(0x4F);
				}
				while(PORTAbits.RA4 && cursor_pos == 6); // carregar em S2 para indicar que se quer definir alarme
				if(mudei_alum==1 && alarme_lum_prev!=alarme_lum){
					mudei_alum=0;
					update_EEPROM_external(5);
				}
				if(cursor_pos == 6){
					SetDDRamAddr(0x4F); // levar o cursor ate ao sitio onde aparece o nivel da luminosidade
					Delay1KTCYx(200);
					change_L = 1;
				//	while(PORTAbits.RA4 && change_L == 1); // carregar novamente em S2 para incrementar "n" e definir o valor de luminosidade pretendido
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
				update_EEPROM_interna_lum_alarme();
			}

			else if(cursor_pos == 7){ // activar/desactivar alarmes
				SetDDRamAddr(0x0D);
				while(PORTAbits.RA4 && cursor_pos == 7 && alarme_OFF == 1);
				if(d_a_alarmes==1 && alarmes_prev[0]!=alarmes[0]){
					d_a_alarmes=0;
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
				while(PORTAbits.RA4 && cursor_pos == 8);
				if(cursor_pos == 8){
					modo_sleep = 1;
					Delay1KTCYx(200);
					while( BusyXLCD() );
					WriteCmdXLCD(DOFF);
				}
			}
		}

		else{ // nao esta no modo modificao
			SetDDRamAddr(0x00);        // First line, first column
			while( BusyXLCD() );
			sprintf(time, "%d%d:%d%d:%d%d", hd, hu, md, mu, sd, su);
			putsXLCD(time);
		}

    /* verificacao de alarmes */
  	if(alarmes[0] == 'A'){

			if(n_lum > alarme_lum && up_L == 1){
  			alarme_OFF = 0;
  			alarme_lum_ON = 1;
  			if(modo_modificacao == 1){
  				modo_modificacao = 0;
  				sai_modificacao = 1;
  				cursor_pos = 0;
  			}
  			seconds_alarme_TSOM = seconds;
  			up_L = 0;
  			//CCP1CON = 0x0F; // turn the buzzer on
				update_EEPROM_external(9);

				SetDDRamAddr(0x47);
				while( BusyXLCD() );
				putrsXLCD("O");
  		}

			if(n_lum < alarme_lum && up_L == 0){
  			alarme_OFF = 0;
  			alarme_lum_ON = 1;
  			if(modo_modificacao == 1){
  				modo_modificacao = 0;
  				sai_modificacao = 1;
  				cursor_pos = 0;
  			}
  			seconds_alarme_TSOM = seconds;
  			up_L = 1;
  			//CCP1CON = 0x0F; // turn the buzzer on
				update_EEPROM_external(9);


				SetDDRamAddr(0x47);
				while( BusyXLCD() );
				putrsXLCD("O");
  		}

			if(((int)temperatura) > alarme_temp && up_T == 1){
  			alarme_OFF = 0;
  			alarme_temp_ON = 1;
  			if(modo_modificacao == 1){
  				modo_modificacao = 0;
  				sai_modificacao = 1;
  				cursor_pos = 0;
  			}
  			seconds_alarme_TSOM = seconds;
  			up_T = 0;
  			//CCP1CON = 0x0F; // turn the buzzer on
				update_EEPROM_external(8);

				SetDDRamAddr(0x47);
				while( BusyXLCD() );
				putrsXLCD("O");
  		}

			if(((int)temperatura) < alarme_temp && up_T == 0){
				alarme_OFF = 0;
  			alarme_temp_ON = 1;
  			if(modo_modificacao == 1){
  				modo_modificacao = 0;
  				sai_modificacao = 1;
  				cursor_pos = 0;
  			}
  			seconds_alarme_TSOM = seconds;
  			up_T = 1;
  			//CCP1CON = 0x0F; // turn the buzzer on
				update_EEPROM_external(8);

				SetDDRamAddr(0x47);
				while( BusyXLCD() );
				putrsXLCD("O");
  		}

			if(alarme_hours == hours && alarme_minutes == minutes && alarme_seconds == seconds && disp_ahoras==1){
				disp_ahoras=0;
				alarme_OFF = 0;
				alarme_clock_ON = 1;
				if(modo_modificacao == 1){
  				modo_modificacao = 0;
  				sai_modificacao = 1;
  				cursor_pos = 0;
  			}
  			seconds_alarme_TSOM = seconds;
  			//CCP1CON = 0x0F; // turn the buzzer on
				update_EEPROM_external(7);

				SetDDRamAddr(0x47);
				while( BusyXLCD() );
				putrsXLCD("O");
			}

			if(seconds == seconds_alarme_TSOM + TSOM){
				//CCP1CON = 0x00;

				SetDDRamAddr(0x47);
				while( BusyXLCD() );
				putrsXLCD("J");
				SetDDRamAddr(0x46);
				while( BusyXLCD() );
				putrsXLCD(" ");
			}
  	}

  	if(sai_modificacao == 1){
			mudei_horas=0;
			mudei_ahoras=0;
			mudei_alum=0;
			mudei_atemp=0;
			d_a_alarmes=0;
			alarme_temp_prev=alarme_temp;
			alarme_lum_prev=alarme_lum;
			alarme_hours_prev=alarme_hours;
			alarme_minutes_prev=alarme_minutes;
			alarme_seconds_prev=alarme_seconds;
			alarmes_prev[0]=alarmes[0];

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

		if(desliga_alarmes==1){
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

		if (PMON != 0 && seconds%PMON == 0){
			temperatura = tsttc();
			Delay10TCYx(5);
			ConvertADC();
			while(BusyADC());
			lum_2=ReadADC();
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
				update_EEPROM_external(codigoev);
			}
		}
 	}
}
