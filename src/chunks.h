#ifndef UART_ATTINY_SRC_CHUNKS_H_
	#define UART_ATTINY_SRC_CHUNKS_H_

	#ifndef F_CPU
		//#define F_CPU 1843200UL
		#define F_CPU 3686400UL
	#endif

	#ifndef DEVICE
		#define DEVICE attiny85
	#endif

	#ifndef __AVR_ATtiny85__
		#define __AVR_ATtiny85__
	#endif

	#include <avr/io.h>
	#include <stdlib.h>
	#include <stdint.h>
	#include <stdio.h>
	#include <util/delay.h>
	#include <avr/interrupt.h>

#endif /* UART_ATTINY_SRC_CHUNKS_H_ */
