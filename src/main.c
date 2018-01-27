#include "chunks.h"
#include "suart/suart.h"

int main()
{
	SUART_tx_init();
	SUART_init_tx_stdio();

	//################################
	ADMUX	= _BV(MUX0); 

	ADCSRA	= _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);						// 128 предделитель
	//################################

	sei();

	while(1)
	{
		ADCSRA = ADCSRA | _BV(ADSC);
		loop_until_bit_is_set(ADCSRA, ADIF);
		double volts	= (float)ADCW * 5.f / 1023.f;
		double temp		= volts * 100 - 273;

		printf("ADCW = %.2f (%d) Привет привет Тестирую уарт как он работает\n", temp, ADCW);
		_delay_ms(1000);
	}

	return EXIT_SUCCESS;
}

