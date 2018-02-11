#ifndef __BITBANG_UART_H___
#define __BITBANG_UART_H___

#include <avr/io.h>
#include <stdint.h>

#define F_CPU 16000000UL
#define _CLK DDB0
#define _SDI DDB1

#define BAUD 9600
#define BAUDRATE F_CPU/16/BAUD-1

#define _BAUD 115200
#define _BIT_TIME 8.681
#define _HALF_BIT_TIME 4.340


#include <util/delay.h>


uint16_t RX_packet();



#endif
