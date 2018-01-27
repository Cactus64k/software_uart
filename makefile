TARG		= suart
CC			= avr-gcc
OBJCOPY		= avr-objcopy
SRCS		= $(shell find . -name '*.c' -printf './%P ')
PROGR		= usbtiny
MCU			= attiny85
LIBS		= -lprintf_flt
OBJS		= $(SRCS:.c=.o)

CFLAGS		= -mmcu=$(MCU) -std=c99 -Wall -O3
LDFLAGS		= -mmcu=$(MCU) -Wall -Os -Werror -u vfprintf
 
all: clean $(TARG) load
 
$(TARG): $(OBJS)
	$(CC) $(LDFLAGS) $(LIBS) -o $@.elf $(OBJS)
	$(OBJCOPY) -O ihex $@.elf $@.hex

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.elf *.bin *.hex  $(OBJS) *.map

load:
	avrdude -c $(PROGR) -p $(MCU) -U flash:w:$(TARG).hex

erase:
	avrdude -c $(PROGR) -p $(MCU) -eu

eeprom_read:
	avrdude -c $(PROGR) -p $(MCU) -U eeprom:r:eeprom.hex:i

