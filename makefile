TARG		= uart_attiny
CC			= avr-gcc
OBJCOPY		= avr-objcopy
SRCS		= $(shell find . -name '*.c' -printf './%P ')
PROGR		= usbasp
MCU			= attiny85
F_CPU		= 1000000UL
LIBS		= 
OBJS		= $(SRCS:.c=.o)

CFLAGS		= -mmcu=$(MCU) -std=c99 -Wall -Os -DF_CPU=$(F_CPU) -DDEVICE=$(MCU)
LDFLAGS		= -mmcu=$(MCU) -Wall -Os -Werror 
 
all: $(TARG) load
 
$(TARG): $(OBJS)
	$(CC) $(LDFLAGS) $(LIBS) -o $@.elf $(OBJS)
	$(OBJCOPY) -O ihex $@.elf $@.hex

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.elf *.bin *.hex  $(OBJS) *.map

load:
	avrdude -c $(PROGR) -p $(MCU) -U flash:w:$(TARG).hex -D

erase:
	avrdude -c $(PROGR) -p $(MCU) -eu

eeprom_read:
	avrdude -c $(PROGR) -p $(MCU) -U eeprom:r:eeprom.hex:i

