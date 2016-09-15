#include "chunks.h"
#include "suart/suart.h"



int main()
{
	SUART_tx_init();
	SUART_init_tx_stdio();

	//SUART_rx_init();

	OSCCAL += 7;

	//################################
	ADMUX	= _BV(REFS1) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0);			// температурный датчик, внутренний источник на 1,1 вольт
	ADCSRA	= _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);						// 128 предделитель
	//################################

	sei();

	while(1)
	{
		_delay_ms(5);
		ADCSRA			= ADCSRA | _BV(ADSC);
		loop_until_bit_is_set(ADCSRA, ADIF);
		//uint8_t byte	= SUART_read_byte();
		//printf("byte = %d\n\n\n", byte);
		printf("ADC = %d\n", ADCW);
		_delay_ms(100);
	}

	return EXIT_SUCCESS;
}

