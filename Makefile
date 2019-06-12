CC=gcc
LINK=gcc -lpthread
CFLAGS=-c -Wall -I.
OBJECT_FILES=gpioDIO.o Sensor.o raspberryServer.o Sunucu.o Surucu.o Keypad.o Uart.o

.SUFFIXES: .c .o

.c .o:
	$(CC) $^ -o $@ $<

raspberryServer: $(OBJECT_FILES)
	$(LINK) $(OBJECT_FILES) -o $@

#Dependances
gpioDIO.o: gpioDIO.c gpioDIO.h gpioTypes.h
Sensor.o: Sensor.c Sensor.h gpioDIO.h
Surucu.o: Surucu.c Surucu.h
Sunucu.o: Sunucu.c Sunucu.h
Uart.o: Uart.c Uart.h
Keypad.o: Keypad.c Keypad.h gpioDIO.h
raspberryServer.o: raspberryServer.c Sensor.h Surucu.h Sunucu.h Keypad.h Uart.h
