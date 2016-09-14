#include "chunks.h"
#include "soft_uart.h"

int main()
{
	SUART_init();
	SUART_init_stdio();
	DDRB = DDRB | _BV(PB3);
	OSCCAL += 7;

	//################################
	ADMUX	= _BV(REFS1) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0);			// температурный датчик, внутренний источник на 1,1 вольт
	ADCSRA	= _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);						// 128 предделитель
	//################################


	sei();

	while(1)
	{
		ADCSRA = ADCSRA | _BV(ADSC);
		loop_until_bit_is_set(ADCSRA, ADIF);
		uint16_t val	= (ADCW - 272.9) / 1.075;
		printf("temp = %d°C\n", val);
		_delay_ms(100);
		PORTB = PORTB ^ _BV(PB3);
	}

	return EXIT_SUCCESS;
}

