#include "suart.h"

#ifdef ENABLE_RECEIVER

static uint16_t rx_frame		= 0;
static volatile uint8_t rx_pos			= 0;

void SUART_rx_init()
{
	RX_DDR	= RX_DDR	&~_BV(RX_PIN);
	RX_PORT = RX_PORT	| _BV(RX_PIN);

	TCCR0A	= TCCR0A |	_BV(WGM01);												// CTC режим
	TCCR0B	= TCCR0B |	SUART_PRESCALER_MASK;
	OCR0A	= SUART_COMPARE_PERIOD;

	GIMSK	= GIMSK |	_BV(PCIE);												// изменение уровня на ноге
	PCMSK	= PCMSK |	_BV(RX_INT_PIN);
}

ISR(PCINT0_vect)
{
	if(!(RX_PORT_PIN & RX_PIN))
	{
		rx_pos			= 0;
		rx_frame		= 9;

		GIMSK			= GIMSK &~_BV(PCIE);										// отключаем прерывание по изменению уровня

		OCR0B			= (TCNT0 > SUART_COMPARE_PERIOD/2)? TCNT0-SUART_COMPARE_PERIOD/2 : TCNT0+SUART_COMPARE_PERIOD/2;
		TIMSK			= TIMSK | _BV(OCIE0B);										// включаем прерывание по свопадению
	}
}

ISR(TIMER0_COMPB_vect)
{
	if(RX_PORT_PIN & _BV(RX_PIN))
		rx_frame = rx_frame | (UINT16_C(1) << rx_pos);

	if(rx_pos == 9)																// start + 8 bit data + 1stop
	{
		TIMSK			= TIMSK &~ _BV(OCIE0B);									// отключаем прерывание
		GIMSK			= GIMSK | _BV(PCIE);									// включаем прерывание на ноге
	}
	else
		rx_pos++;
}


uint8_t SUART_read_byte()
{

	while(rx_pos != 10);

	uint8_t byte = (rx_frame >> 1) & 0xFF;
	return byte;
}


static int stdin_get_char(FILE *stream)
{
	return SUART_read_byte();
}

void SUART_init_rx_stdio()
{
	static FILE f_rx;
	stdout	= &f_rx;
	fdev_setup_stream(stdout, NULL, stdin_get_char, _FDEV_SETUP_READ);
}


#endif /* ENABLE_RECEIVER */
