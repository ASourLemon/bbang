SDIR=source
IDIR=include
ODIR=obj
LDIR=lib

CC=avr-gcc
SC=ar rcs
CFLAGS= -mmcu=atmega328p -Os -g -Wall -I$(IDIR)

LIBS=

all: 
	$(CC) -c $(SDIR)/mcu_general.c -o $(ODIR)/mcu_general.o $(CFLAGS) $(LIBS)
	$(CC) -c $(SDIR)/mcu_bitbang_uart.c -o $(ODIR)/mcu_bitbang_uart.o $(CFLAGS) $(LIBS)
	$(SC) $(LDIR)/libmcu_general.a $(ODIR)/mcu_general.o $(ODIR)/mcu_bitbang_uart.o
