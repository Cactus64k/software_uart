#ifndef SOFT_UART_H_
	#define SUART_H

	//###############OPTIONS###############
	#ifndef F_CPU
		//#define F_CPU 1843200UL
		#define F_CPU 3686400UL
	#endif

	#ifndef BAUD
		#define BAUD 9600UL
	#endif

	#define TX_PORT	PORTB
	#define TX_DDR	DDRB
	#define TX_PIN	PB0

	//#####################################

	#include <avr/io.h>
	#include <stdlib.h>
	#include <stdio.h>
	#include <stdint.h>
	#include <avr/interrupt.h>
	#include <stdbool.h>

	#define SUART_PRESCALER_1 ((F_CPU/(BAUD * 1UL)))
	#if SUART_PRESCALER_1 < 0xFF
		#define SUART_TIMER_PRESCALER 1
	#else
		#define SUART_PRESCALER_8 ((F_CPU/(BAUD * 8UL)))
		#if SUART_PRESCALER_8 < 0xFF
			#define SUART_TIMER_PRESCALER 8
		#else
			#define SUART_PRESCALER_64 ((F_CPU/(BAUD * 64UL)))
			#if SUART_PRESCALER_64 < 0xFF
				#define SUART_TIMER_PRESCALER 64
			#else
				#define SUART_PRESCALER_256 ((F_CPU/(BAUD * 256UL)))
				#if SUART_PRESCALER_256 < 0xFF
					#define SUART_SUART_TIMER_PRESCALER 256
				#else
					#define SUART_PRESCALER_1024 ((F_CPU/(BAUD * 1024UL)))
					#if SUART_PRESCALER_1024 < 0xFF
						#define SUART_TIMER_PRESCALER 1024
					#else
						#error "Failed to calculate timer prescaler"
					#endif	/* PRESCALER_1024 */
				#endif		/* PRESCALER_256 */
			#endif			/* PRESCALER_64 */
		#endif				/* PRESCALER_8 */
	#endif					/* PRESCALER_1 */

	#if SUART_TIMER_PRESCALER == 1
		#define SUART_PRESCALER_MASK (_BV(CS00))
	#elif SUART_TIMER_PRESCALER == 8
		#define SUART_PRESCALER_MASK (_BV(CS01))
	#elif SUART_TIMER_PRESCALER == 64
		#define SUART_PRESCALER_MASK (_BV(CS01) | _BV(CS00))
	#elif SUART_TIMER_PRESCALER == 256
		#define SUART_PRESCALER_MASK (_BV(CS02))
	#elif SUART_TIMER_PRESCALER == 1024
		#define SUART_PRESCALER_MASK (_BV(CS02) | _BV(CS00))
	#endif

	#define SUART_COMPARE_PERIOD (F_CPU/(BAUD * SUART_TIMER_PRESCALER))

	void SUART_tx_init();
	void SUART_init_tx_stdio();
	void SUART_send_string(char* string);
	void SUART_send_byte(uint8_t byte);

#endif /* SOFT_UART_H_ */
