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
volatile unsigned char modo_sleep = 0;
volatile unsigned char modo_modificacao = 0;
volatile unsigned char change_L = 0;
volatile unsigned char change_T = 0;
volatile unsigned char change_A = 0;
volatile unsigned char change_AS = 0;
volatile unsigned char change_AM = 0;
volatile unsigned char change_AH = 1;
volatile unsigned char sai_modificacao = 0;
volatile unsigned char mudei_ahoras = 0;
volatile unsigned char mudei_alum = 0;
volatile unsigned char mudei_atemp = 0;
volatile unsigned char d_a_alarmes = 0;
volatile unsigned char alarme_OFF = 1;
volatile unsigned char alarme_lum_ON = 0;
volatile unsigned char alarme_temp_ON = 0;
volatile unsigned char alarme_clock_ON = 0;
volatile unsigned char desliga_alarmes = 0;
unsigned char up_L = 0;
unsigned char up_T = 0;
unsigned char first_iteration = 1;
unsigned char temperatura = 0;
unsigned char nova_L = 0;
unsigned char nova_T = 0;
unsigned char inicio = 1;
unsigned char codigoev = 0;
unsigned char indwrite = 0;
unsigned char endereco = 1;
unsigned char mudei_horas = 0;
unsigned char disp_ahoras = 1;
unsigned char IRVSTbit = 0;


/* strings */
char time[9];
char change_hours[3];
char change_minutes[3];
char change_seconds[3];
char change_hours_alarme[3];
char change_minutes_alarme[3];
char change_seconds_alarme[3];
char 	dataEEPROMext [8];
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


void writeEEPROMexterna (char endereco, char data[8]){

	char ind=0;
	char i=0;
		IdleI2C();
		StartI2C(); IdleI2C();

			WriteI2C(0xA0); IdleI2C();
			WriteI2C(0x00); IdleI2C();   //HB
			WriteI2C(endereco); IdleI2C();   //LB
		for (ind=0; ind<8; ind++){
			WriteI2C(data[ind]); IdleI2C(); //Dados
		}
		//NotAckI2C(); IdleI2C();
		StopI2C();

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
		if(alarme_OFF == 0){ //alarmes estao ligados
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


void init_LVD (void)
{
	/* step 1 */
	LVDCON &= 0xFE;
	LVDCON |= 0x0E;

	/* step 2 */
	PIE2 &= 0xFB; // clear LVDIE bit
	INTCON &= 0x7F; // clear GIE bit

	/* step 3 */
	LVDCON |= 0x10;

	/* step 4 */
	while(!((LVDCON & 0x20) >> 5)); // so avanca quando IRVST estiver a high

	/* step 5 */
	PIR2 &= 0xFB; // clear LVDIF bit

	/* step 6 */
	PIE2 |= 0x04; // set LVDIE bit
	INTCON |= 0x80; // set GIE bit
}

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

	Delay1KTCYx(200);
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

void update_EEPROM_interna_relogio_alarme (void)
{
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
}

void ler_EEPROM_interna_lum_alarme (void)
{
	alarme_lum = ler_EEPROM_interna(0x04);
}

void update_EEPROM_interna_relogio (void)
{
	hours_interna = ler_EEPROM_interna(0x05);
	if(hours != hours_interna){
		escrita_EEPROM_interna(0x05, hours);
	}

	minutes_interna = ler_EEPROM_interna(0x06);
	if(minutes != minutes_interna){
		escrita_EEPROM_interna(0x06, minutes);
	}

	seconds_interna = ler_EEPROM_interna(0x05);
	if(seconds != seconds_interna){
		escrita_EEPROM_interna(0x07, seconds);
	}
}

void update_EEPROM_interna_parametros (void)
{
		/* update valor de NREG */
		escrita_EEPROM_interna(0x08, 30);

		/* update valor de PMON */
		escrita_EEPROM_interna(0x09, 5);

		/* update valor de TSOM */
		escrita_EEPROM_interna(0x0A, 2);
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

void update_EEPROM_external(char codigoev){


	indwrite=endereco*8;
	dataEEPROMext[0]=hours;
	dataEEPROMext[1]=minutes;
	dataEEPROMext[2]=seconds;
	dataEEPROMext[3]=codigoev;


	SetDDRamAddr(0x46);


	switch(codigoev){
		case 1:
			while( BusyXLCD() );

			putrsXLCD("I");
			dataEEPROMext[4]=(int)temperatura;
			dataEEPROMext[5]=0;
			dataEEPROMext[6]=0;
			dataEEPROMext[7]=0;
			writeEEPROMexterna(indwrite,dataEEPROMext);
			EEAckPolling(0xA0);
		break;
		case 2:
			while( BusyXLCD() );
			putrsXLCD("N");
			dataEEPROMext[4]=hours;
			dataEEPROMext[5]=minutes;
			dataEEPROMext[6]=seconds;
			dataEEPROMext[7]=0;
			writeEEPROMexterna(indwrite,dataEEPROMext); //Temperatura
			EEAckPolling(0xA0);

		break;
		case 3:
			while( BusyXLCD() );
			putrsXLCD("h");
			dataEEPROMext[4]=alarme_hours;
			dataEEPROMext[5]=alarme_minutes;
			dataEEPROMext[6]=alarme_seconds;
			dataEEPROMext[7]=0;
			writeEEPROMexterna(indwrite,dataEEPROMext); //Temperatura
			EEAckPolling(0xA0);

		break;
		case 4:
			while( BusyXLCD() );
			putrsXLCD("t");
			dataEEPROMext[4]=alarme_temp;
			dataEEPROMext[5]=0;
			dataEEPROMext[6]=0;
			dataEEPROMext[7]=0;
			writeEEPROMexterna(indwrite,dataEEPROMext); //Temperatura
			EEAckPolling(0xA0);

		break;
		case 5:
			while( BusyXLCD() );
			putrsXLCD("l");
			dataEEPROMext[4]=alarme_lum;
			dataEEPROMext[5]=0;
			dataEEPROMext[6]=0;
			dataEEPROMext[7]=0;
			writeEEPROMexterna(indwrite,dataEEPROMext); //Temperatura
			EEAckPolling(0xA0);

		break;
		case 6:
			while( BusyXLCD() );
			putrsXLCD("A");
			dataEEPROMext[4]=(int)temperatura;
			dataEEPROMext[5]=n_lum;
			dataEEPROMext[6]=0;
			dataEEPROMext[7]=0;
			writeEEPROMexterna(indwrite,dataEEPROMext); //Temperatura
			EEAckPolling(0xA0);

		break;
		case 7:
			while( BusyXLCD() );
			putrsXLCD("H");
			dataEEPROMext[4]=(int)temperatura;
			dataEEPROMext[5]=n_lum;
			dataEEPROMext[6]=0;
			dataEEPROMext[7]=0;
			writeEEPROMexterna(indwrite,dataEEPROMext); //Temperatura
			EEAckPolling(0xA0);

		break;
		case 8:
			while( BusyXLCD() );
			putrsXLCD("T");
			dataEEPROMext[4]=(int)temperatura;
			dataEEPROMext[5]=n_lum;
			dataEEPROMext[6]=0;
			dataEEPROMext[7]=0;

			writeEEPROMexterna(indwrite,dataEEPROMext); //Temperatura
			EEAckPolling(0xA0);

			break;
		case 9:
			while( BusyXLCD() );
			putrsXLCD("L");
			dataEEPROMext[4]=(int)temperatura;
			dataEEPROMext[5]=n_lum;
			dataEEPROMext[6]=0;
			dataEEPROMext[7]=0;
			writeEEPROMexterna(indwrite,dataEEPROMext); //Temperatura
			EEAckPolling(0xA0);

		break;
	}

	endereco ++;
	dataEEPROMext[0]=NREG;
	dataEEPROMext[1]=endereco;
	dataEEPROMext[2]=0;
	dataEEPROMext[3]=0;
	dataEEPROMext[4]=0;
	dataEEPROMext[5]=0;
	dataEEPROMext[6]=0;
	dataEEPROMext[7]=0;
	writeEEPROMexterna(0x00,dataEEPROMext); //Cabeçalho NREG
	EEAckPolling(0xA0);
<<<<<<< HEAD
	if(endereco==NREG+1){
		endereco=1;
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

	InitializeBuzzer();

	init_LVD();

	EnableHighInterrupts();

	alarmes[0] = 'a';
	alarmes[1] = 0;
	alarmes_prev[0] = 'a';
	alarmes_prev[1] = 0;

	update_EEPROM_interna_parametros();
	ler_EEPROM_interna_parametros(); // carregar da EEPROM interna os valores de NREG, PMON e TSOM

	ler_EEPROM_interna_relogio_alarme(); // carregar da EEPROM interna o valor do alarme do relogio
	ler_EEPROM_interna_temp_alarme(); // carregar da EEPROM interna o valor do alarme da temperatura
	ler_EEPROM_interna_lum_alarme(); // carregar da EEPROM interna o valor do alarme da luminosidade

 	WriteTimer1( 0x8000 ); // load timer: 1 second

	//EEPROM External Init
	dataEEPROMext[0]=NREG;
	dataEEPROMext[1]=0;
	dataEEPROMext[2]=0;
	dataEEPROMext[3]=0;
	dataEEPROMext[4]=0;
	dataEEPROMext[5]=0;
	dataEEPROMext[6]=0;
	dataEEPROMext[7]=0;
	writeEEPROMexterna(0x00,dataEEPROMext); //Cabeçalho NREG
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

		if(isLVD()){
			update_EEPROM_interna_relogio();
		}

		SetDDRamAddr(0x0D);
		putsXLCD(alarmes);

///////////////////////////////////////////////////////////////////////////////
//************************** Modo de Modificação ****************************//
///////////////////////////////////////////////////////////////////////////////

		if(modo_modificacao == 1){ // estamos em modo modificacao

			if(cursor_pos  >= 4){
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
				if(mudei_atemp == 1 && alarme_temp_prev != alarme_temp){
					mudei_atemp = 0;
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
				if(mudei_alum == 1 && alarme_lum_prev != alarme_lum){
					mudei_alum = 0;
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
				disp_ahoras = 0;
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
