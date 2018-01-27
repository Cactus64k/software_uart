#include "suart.h"

static uint16_t tx_frame		= 0;
static volatile uint8_t tx_pos	= 0;
#define PACKAGE_SIZE 10

ISR(TIMER0_COMPA_vect)
{
	if(tx_frame & ((uint16_t)1 << tx_pos))
		TX_PORT = TX_PORT | _BV(TX_PIN);
	else
		TX_PORT = TX_PORT & ~_BV(TX_PIN);

	tx_pos++;
}

void SUART_send_byte(uint8_t byte)
{
	tx_frame	= ((uint16_t)byte << 1) | (0xFFFF << 9);		// формируем старт бит и стоп бит
	tx_pos		= 0;
	TCNT0		= 0;


	TIMSK		= TIMSK | _BV(OCIE0A);		// прерывание на совпадение

	while(tx_pos < PACKAGE_SIZE);
	
	TIMSK		= TIMSK &~ _BV(OCIE0A);
}

int	stdio_put_char(char c, FILE* f)
{
	SUART_send_byte(c);
	return EXIT_SUCCESS;
}

void SUART_tx_init()
{
	TX_DDR	= TX_DDR	| _BV(TX_PIN);		// передающий пин на выход
	TX_PORT = TX_PORT	| _BV(TX_PIN);		// в режиме idle пин должен быть в единице
	TCCR0A	= TCCR0A	| _BV(WGM01);		// CTC режим
	TCCR0B	= TCCR0B	| SUART_PRESCALER_MASK;
	OCR0A	= SUART_COMPARE_PERIOD;
}

void SUART_init_tx_stdio()
{
	static FILE f_tx;
	stdout	= &f_tx;
	fdev_setup_stream(stdout, stdio_put_char, NULL, _FDEV_SETUP_WRITE);
}

void SUART_send_string(char* string)
{
	while(*string != '\0')
	{
		SUART_send_byte(*string);
		string++;
	}

}

