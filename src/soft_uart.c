#include "soft_uart.h"

uint16_t tx_frame		= 0;
uint8_t tx_pos			= 0;
volatile bool tx_bussy	= false;


void SUART_init()
{
	TX_DDR	= TX_DDR	| _BV(TX_PIN);
	TX_PORT = TX_PORT	| _BV(TX_PIN);

	TCCR0A	= _BV(WGM01);									// CTC режим
	OCR0A	= F_CPU/(BAUD * TIMER_PRESCALER) -1UL;
	TIMSK	= _BV(OCIE0A);									// прерывание на совпадение
}

ISR(TIMER0_COMPA_vect)
{
	if(tx_frame & (1 << tx_pos))
		TX_PORT = TX_PORT | _BV(TX_PIN);
	else
		TX_PORT = TX_PORT & ~_BV(TX_PIN);

	if(tx_pos == 10)														// start + 8 bit data + stop
	{
		TCCR0B		= TCCR0B & ~PRESCALER_MASK;								// отключаем таймер
		tx_bussy	= false;
	}

	tx_pos++;
}

void SUART_send_byte(uint8_t byte)
{
	tx_frame	= ((uint16_t)byte << 1) | (UINT16_C(0xFFFF) << 9);			// формируем старт бит и стоп бит
	tx_pos		= 0;
	TCNT0		= OCR0A-1;													// чтобы не пропускать цикл
	tx_bussy	= true;
	TCCR0B		= PRESCALER_MASK;											// включаем таймер
	while(tx_bussy == true);
}

void SUART_init_stdio()
{
	static FILE f;
	stdout	= &f;

	fdev_setup_stream(stdout, SUART_send_byte, NULL, _FDEV_SETUP_WRITE);
}

void SUART_send_string(char* string)
{
	while(*string != '\0')
	{
		SUART_send_byte(*string);
		string++;
	}

}
