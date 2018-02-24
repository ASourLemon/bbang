SDIR=apps/source
BDIR=apps/bin
LDIR=utils/lib
IDIR=utils/include

CC=avr-gcc
CFLAGS= -mmcu=atmega328p -Os -g -Wall

gba_link: 
	$(CC) -o $(BDIR)/gba $(SDIR)/gba_link.c -L$(LDIR) -lmcu_general $(CFLAGS) -I$(IDIR)
