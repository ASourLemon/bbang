#ifndef __GENERAL_H__
#define __GENERAL_H__


#include <stdint.h>
#include <avr/io.h>

#define F_CPU 16000000UL

#ifndef _CONTROL_PORT
#define _CONTROL_PORT DDRB
#endif
#ifndef _INPUT_PORT
#define _INPUT_PORT PINB
#endif
#ifndef _OUTPUT_PORT
#define _OUTPUT_PORT PORTB
#endif

#define _LED_BIT 5

#define _SET_INPUT(x) _CONTROL_PORT &= ~(1 << x)
#define _SET_OUTPUT(x) _CONTROL_PORT |= (1 << x)
#define _SET_LOW(x) _OUTPUT_PORT &= ~(1 << x)
#define _SET_HIGH(x) _OUTPUT_PORT |= (1 << x)
#define _GET_INPUT(x) _INPUT_PORT & (1 << x)

#define _WAIT_WHILE(x) while(x){};

#define _DEBUG_BAUD 9600
#define _DEBUG_BAUDRATE F_CPU/16/_DEBUG_BAUD-1


void setup_debug_tx();
void send_char8(uint8_t c);
void send_char16(uint16_t c);
void send_char32(uint32_t c);
void toogle_led();


#endif /* __GENERAL_H__ */