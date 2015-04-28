#include "main.h"
#include "memorias.h"
#include "alarmes.h"
#include "modificacao.h"
#include "sensores.h"
#include "usart.h"

volatile int hours = 0;
volatile int minutes = 0;
volatile int seconds = 0;
volatile int cursor_pos = 0;

int PMON = 5;
int TSOM = 2;
int NREG = 30;
int alarme_hours = 0;
int alarme_minutes = 0;
int alarme_seconds = 0;
int sd = 0;
int su = 0;
int md = 0;
int mu = 0;
int hd = 0;
int hu = 0;
int n_lum = 0;
int alarme_lum = 0;
int alarme_temp = 20;
int alarme_sd = 0;
int alarme_su = 0;
int alarme_md = 0;
int alarme_mu = 0;
int alarme_hd = 0;
int alarme_hu = 0;

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
volatile unsigned char segundos_mudou = 0;
volatile unsigned char minutos_mudou = 0;
volatile unsigned char horas_mudou = 0;
volatile unsigned char disp_ahoras = 1;
volatile unsigned char ler_sensores = 1;
volatile unsigned char protocolo_OK = 0;

unsigned char checksumIsRight = 0;

char temperatura = 0;
char codigoev = 0;
char index = 0;

char temperatura_s[5] = {0};
char luminosidade[4] = {0};
char alarmes[2] = {0};
char alarmes_prev[2] = {0};
char msg_rec[4] = {0};

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
	//******************************** USART ************************************//
	///////////////////////////////////////////////////////////////////////////////
	if(PIR1bits.RCIF == 1){
		PIR1bits.RCIF = 0;
		byte_USART = getcUSART();
		if(byte_USART == EOM && SOM_int == 1){ // verifica se EOM
			EOM_int = 1;
			SOM_int = 0;
		}
		if(SOM_int == 1){ // verifica se ja houve SOM
			if(index < 4){
				msg_rec[index] = byte_USART;
				index++;
			}
		}
		if(byte_USART == SOM){ // verifica se SOM
			SOM_int = 1;
			index = 0;
			msg_rec = {0};
		}
		if(EOM_int == 1){
			EOM_int = 0;
			protocolo_OK = 1;
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	//******************************** LVD **************************************//
	///////////////////////////////////////////////////////////////////////////////
	if(PIR2bits.LVDIF == 1){
		escrita_EEPROM_interna(0x07, seconds);
		while(1);
	}

	///////////////////////////////////////////////////////////////////////////////
	//******************************** Relógio **********************************//
	///////////////////////////////////////////////////////////////////////////////
	if(PIR1bits.TMR1IF == 1){ // timer interrupt
		WriteTimer1( 0x8000 ); // reload timer: 1 second
		PIR1bits.TMR1IF = 0; /* clear flag to avoid another interrupt */
		segundos_mudou = 1;
		ler_sensores=1;
		seconds++;
		if(disp_ahoras == 0){
			disp_ahoras = 1;
		}
		if(seconds == 60){
			seconds = 0;
			minutos_mudou=1;
			minutes++;
			if(minutes == 60){
				minutes = 0;
				horas_mudou=1;
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
		//WriteCmdXLCD( CURSOR_OFF );// Enable display with no cursor
		if(alarme_OFF == 0){ //alarmes estao ligados
			alarme_OFF = 1; //desligar alarmes
			alarme_lum_ON = 0;
			alarme_temp_ON = 0;
			alarme_clock_ON = 0;
			desliga_alarmes = 1;
		}
		else{
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
				}
				else{
					modo_modificacao = 0; //sair do modo de modificacaoo
					sai_modificacao = 1; //acabei de sair do modo de modificacao
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
	PIE2bits.LVDIE = 0;         /* disable LVD interrupt */
	LVDCON = 0b1110;            /* 4.2V - 4.45V */
	LVDCONbits.LVDEN = 1;       /* enable LVD */
	while (!LVDCONbits.IRVST);  /* wait initialization */
	PIR2bits.LVDIF = 0;         /* clear interrupt flag */
	PIE2bits.LVDIE = 1;         /* enable LVD interrupt */
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
      				T1_SYNC_EXT_OFF );// tem que estar off para correr na placa, mas on para correr no proteus

	ADCON1 = 0x0E; // Port A: A0 - analog; A1-A7 - digital

	OpenADC (ADC_FOSC_RC & ADC_RIGHT_JUST & ADC_1ANA_0REF, ADC_CH0 & ADC_INT_OFF);

	/* BUTTON S3 */
	OpenRB0INT (	PORTB_CHANGE_INT_ON & /* enable the RB0/INT0 interrupt */
                PORTB_PULLUPS_ON &    /* configure the RB0 pin for input */
                FALLING_EDGE_INT);    /* trigger interrupt upon S3 button depression */

	OpenXLCD( FOUR_BIT & LINES_5X7 ); // 4-bit data interface; 2 x 16 characters

	OpenUSART(	USART_TX_INT_OFF  &
             	USART_RX_INT_OFF  &
             	USART_ASYNCH_MODE &
             	USART_EIGHT_BIT   &
             	USART_CONT_RX     &
             	USART_BRGH_HIGH,
             	25 );

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

	checksumIsRight = verificar_checksum();

	if(checksumIsRight == 1){
		ler_EEPROM_interna_parametros();
		ler_EEPROM_interna_relogio();
		ler_EEPROM_interna_relogio_alarme(); // carregar da EEPROM interna o valor do alarme do relogio
		ler_EEPROM_interna_temp_alarme(); // carregar da EEPROM interna o valor do alarme da temperatura
		ler_EEPROM_interna_lum_alarme(); // carregar da EEPROM interna o valor do alarme da luminosidade
	}

	ler_EEPROM_interna_relogio_seconds();

	update_EEPROM_interna_parametros();
	update_EEPROM_interna_relogio_alarme();
	update_EEPROM_interna_temp_alarme();
	update_EEPROM_interna_lum_alarme();
	update_EEPROM_interna_relogio_hours();
	update_EEPROM_interna_relogio_minutes();

	init_EEPROM_externa();

 	WriteTimer1( 0x8000 ); // load timer: 1 second

///////////////////////////////////////////////////////////////////////////////
//*************************** Ciclo Principal *******************************//
///////////////////////////////////////////////////////////////////////////////

	while (1){

		if(modo_sleep == 1 && cursor_pos == 8){
    	WriteCmdXLCD( DOFF );      // Turn display off
		}

		else{
			while( BusyXLCD() );
  		WriteCmdXLCD( DOFF );      // Turn display off
  		while( BusyXLCD() );
  		WriteCmdXLCD( CURSOR_OFF );// Enable display with no cursor
  		while( BusyXLCD() );
		}

		sd = seconds/10;
		su = seconds%10;
		md = minutes/10;
		mu = minutes%10;
		hd = hours/10;
		hu = hours%10;

		SetDDRamAddr(0x0D);
		putsXLCD(alarmes);

		rotina_verificacao_alarmes();

		rotina_modo_modificacao();

		rotina_sai_modificacao();

		avisa_alarmes();

		rotina_sensores_PMON();

		if(protocolo_OK == 1){
			USART_protocolo_OK(msg_rec);
		}

		if(CCP1CON == 0x00 && modo_modificacao == 0){
			modo_sleep = 1;
			Sleep();
		}
 	}
}
