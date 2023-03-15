FILE = main

F_CPU = 16000000

FLAGS = -Wall -Wextra -Werror -Os -DF_CPU=$(F_CPU)

DEVICE = atmega328p

PROGRAMMER = arduino

BAUD = 115200

PORT = $(shell ls /dev/ttyUSB*)

# inspiration for this exercice https://www.youtube.com/watch?v=iKqLbbyPydI

all: hex upload

hex:
	avr-gcc $(FLAGS) -mmcu=$(DEVICE) -o $(FILE).bin $(FILE).c 
	avr-objcopy -O ihex $(FILE).bin $(FILE).hex

upload:
	avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U flash:w:$(FILE).hex:i

clean:
	rm *.bin *.hex

.PHONY: all hex clean 