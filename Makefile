CC = arm-none-linux-gnueabi-gcc
OBJECTS = main.o gprs.o daemon.o beep.o led.o adc.o cgi.o status.o
CFLAGS = -Wall #-g -D _DEBUG_GPRS_
LDFLAGS = -lpthread

main : $(OBJECTS)

.PHONY : clean
clean:
	-rm -f main $(OBJECTS)
