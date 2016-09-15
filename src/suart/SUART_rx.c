#include "suart.h"

#ifdef ENABLE_RECEIVER

static uint16_t rx_frame		= 0;
static uint8_t rx_pos			= 0;
static bool rx_frame_valid		= false;
static bool rx_bussy			= false;

void SUART_rx_init()
{
	RX_DDR	= RX_DDR	&~_BV(RX_PIN);
	//RX_PORT = RX_PORT	| _BV(RX_PIN);

	OCR0B	= SUART_COMPARE_PERIOD;
	TCCR0A	= TCCR0A |	_BV(WGM01);											// CTC режим
	TCCR0B	= TCCR0B |	SUART_PRESCALER_MASK;


	GIMSK	= GIMSK |	_BV(PCIE);
	PCMSK	= PCMSK |	_BV(RX_INT_PIN);
}

ISR(PCINT0_vect)
{
	if(!(RX_PORT_PIN & RX_PIN))
	{
		rx_pos		= 0;
		rx_frame	= 0;
		rx_bussy	= true;
		OCR0B		= TCNT0 + (F_CPU/(BAUD * SUART_TIMER_PRESCALER) -1UL)/2;	// смешение на полбита по времени
		TIMSK		= TIMSK | _BV(OCIE0B);										// включаем прерывание по свопадению
		GIFR		= _BV(PCIF);												// отключаем прерывание по изменению уровня
	}
}

ISR(TIMER0_COMPB_vect)
{
	if(RX_PORT_PIN & _BV(RX_PIN))
		rx_frame = rx_frame | (UINT16_C(1) << rx_pos);
	else
		rx_frame = rx_frame &~(UINT16_C(1) << rx_pos);

	if(rx_pos == 10)															// start + 8 bit data + 1stop
	{
		TIMSK			= TIMSK &~ _BV(OCIE0B);									// отключаем прерывание
		rx_bussy		= false;
		rx_frame_valid	= true;
	}



	rx_pos++;
}


uint8_t SUART_read_byte()
{
	if(rx_frame_valid == true)
	{
		uint8_t byte = (rx_frame >> 1);
		return byte;
	}
	else
		return 0;
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
