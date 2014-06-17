# AVR-GCC Makefile
PROJECT=avrspeedo
SOURCES=speedo.c
CC=avr-gcc
OBJCOPY=avr-objcopy
MMCU=attiny26

CFLAGS=-mmcu=$(MMCU) -Wall -Os -lm -std=c99

$(PROJECT).hex: $(PROJECT).out
	$(OBJCOPY) -j .text -O ihex $(PROJECT).out $(PROJECT).hex

$(PROJECT).out: $(SOURCES) Makefile
	$(CC) $(CFLAGS) -I./ -o $(PROJECT).out $(SOURCES)

program: $(PROJECT).hex
	avrdude -p t26 -c usbasp -e -U flash:w:$(PROJECT).hex
clean:
	rm -f $(PROJECT).out
	rm -f $(PROJECT).hex

