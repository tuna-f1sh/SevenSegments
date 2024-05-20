# Project Name
PROJECT=segments_avr_firmware

# AVR Device
MCU=atmega328p
F_CPU=16000000

# Compiler and Object Copy
CC=avr-gcc
OBJCOPY=avr-objcopy

# Compiler Flags
CFLAGS=-Os -DF_CPU=$(F_CPU) -mmcu=$(MCU)

# Source Files
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

# Default target.
all: $(PROJECT).hex

# Compile: create object files from C source files.
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

# Link: create ELF output file from object files.
$(PROJECT).elf: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

# Hex file
$(PROJECT).hex: $(PROJECT).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# Flash the program
flash: $(PROJECT).hex
	avrdude -p $(MCU) -c usbasp -U flash:w:$<:i -B 125kHz

fuses:
	avrdude -p $(MCU) -c usbasp -U lfuse:w:0xFF:m -U hfuse:w:0xD9:m -U efuse:w:0xFF:m -B 125kHz

# Clean up
clean:
	rm -f $(OBJ) $(PROJECT).elf $(PROJECT).hex

.PHONY: all clean flash
