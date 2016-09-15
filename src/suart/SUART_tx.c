#include "suart.h"


#ifdef ENABLE_TRANSMITTER

static uint16_t tx_frame		= 0;
static uint8_t tx_pos			= 0;
static volatile bool tx_bussy	= false;

void SUART_tx_init()
{
	TX_DDR	= TX_DDR	| _BV(TX_PIN);
	TX_PORT = TX_PORT	| _BV(TX_PIN);

	OCR0A	= SUART_COMPARE_PERIOD;
	TCCR0A	= TCCR0A | _BV(WGM01);												// CTC режим
	TCCR0B	= TCCR0B | SUART_PRESCALER_MASK;
}

ISR(TIMER0_COMPA_vect)
{
	if(tx_frame & (1 << tx_pos))
		TX_PORT = TX_PORT | _BV(TX_PIN);
	else
		TX_PORT = TX_PORT & ~_BV(TX_PIN);

	if(tx_pos == 10)														// start + 8 bit data + stop
	{
		TIMSK	= TIMSK &~ _BV(OCIE0A);										// отключаем прерывание
		tx_bussy	= false;
	}

	tx_pos++;
}

void SUART_send_byte(uint8_t byte)
{
	tx_frame	= ((uint16_t)byte << 1) | (UINT16_C(0xFFFF) << 9);			// формируем старт бит и стоп бит
	tx_pos		= 0;
	tx_bussy	= true;
	TCNT0		= OCR0A-1;													// чтобы не пропускать цикл
	TIMSK		= TIMSK | _BV(OCIE0A);										// прерывание на совпадение
	while(tx_bussy == true);
}

int	stdio_put_char(char c, FILE* f)
{
	SUART_send_byte(c);
	return EXIT_SUCCESS;
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

#endif /* ENABLE_TRANSMITTER */
