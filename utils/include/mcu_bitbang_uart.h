#ifndef __BITBANG_UART_H___
#define __BITBANG_UART_H___


#define _SIGNAL_BAUD 115200
#define _RAW_BIT_TIME 1/_SIGNAL_BAUD
#define _RAW_HALF_BIT_TIME _BIT_TIME/2
#define _BIT_TIME F_CPU/1/_SIGNAL_BAUD-1
#define _HALF_BIT_TIME _BIT_TIME/2

#define _CLOCK_BIT 0
#define _DTX_BIT 1
#define _DRX_BIT 2
#define _SIN_BIT 3

#include "mcu_general.h"
#include <util/delay.h>

uint16_t rx_packet16();
void tx_packet16(uint16_t p);



#endif
