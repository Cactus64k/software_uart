#ifndef SOFT_UART_H_
	#define SOFT_UART_H_

	//###############OPTIONS###############
	#ifndef BAUD
		#define BAUD 9600
	#endif

	#define TX_PORT PORTB
	#define TX_DDR DDRB
	#define TX_PIN PB4

	//#####################################

	#include <avr/io.h>
	#include <stdlib.h>
	#include <stdio.h>
	#include <stdint.h>
	#include <avr/interrupt.h>
	#include <stdbool.h>

	#define PRESCALER_1 (F_CPU/(BAUD * 1UL)) -1UL
	#if PRESCALER_1 < 0xFF
		#define TIMER_PRESCALER 1
	#else
		#define PRESCALER_8 (F_CPU/(BAUD * 8UL)) -1UL
		#if PRESCALER_8 < 0xFF
			#define TIMER_PRESCALER 8
		#else
			#define PRESCALER_64 (F_CPU/(BAUD * 64UL)) -1UL
			#if PRESCALER_64 < 0xFF
				#define TIMER_PRESCALER 64
			#else
				#define PRESCALER_256 (F_CPU/(BAUD * 256UL)) -1UL
				#if PRESCALER_256 < 0xFF
					#define TIMER_PRESCALER 256
				#else
					#define PRESCALER_1024 (F_CPU/(BAUD * 1024UL)) -1UL
					#if PRESCALER_1024 < 0xFF
						#define TIMER_PRESCALER 1024
					#else
						#error "Failed to calculate timer prescaler"
					#endif	/* PRESCALER_1024 */
				#endif		/* PRESCALER_256 */
			#endif			/* PRESCALER_64 */
		#endif				/* PRESCALER_8 */
	#endif					/* PRESCALER_1 */

	#if TIMER_PRESCALER == 1
		#define PRESCALER_MASK _BV(CS00)
	#elif TIMER_PRESCALER == 8
		#define PRESCALER_MASK _BV(CS01)
	#elif TIMER_PRESCALER == 64
		#define PRESCALER_MASK _BV(CS01) | _BV(CS00)
	#elif TIMER_PRESCALER == 256
		#define PRESCALER_MASK _BV(CS02)
	#elif TIMER_PRESCALER == 1024
		#define PRESCALER_MASK _BV(CS02) | _BV(CS00)
	#endif

	void SUART_init();
	void SUART_init_stdio();
	void SUART_send_string(char* string);
	void SUART_send_byte(uint8_t byte);

#endif /* SOFT_UART_H_ */
