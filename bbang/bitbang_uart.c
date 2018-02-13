#include "include/bitbang_uart.h"



uint16_t RX_packet(){
	
	uint16_t s0 = 0;
	uint16_t d0 = 0;
	uint16_t e0 = 0;
	
	// Line is HIGH
	while(PINB & 1);
	_delay_us(_HALF_BIT_TIME);
	
	// Receive Start BIT (LOW)
	s0 = (PINB & 2) ? 1 : 0;

	// Receive Data
	for(uint8_t b = 0;b < 16; b++){
		if(PINB & 2){
			d0 |= (1 << b);
		}
		_delay_us(_BIT_TIME);
	}

	// Receive Stop BIT (HIGH)
	e0 = (PINB & 2) ? 1 : 0;

	debug_print(s0, d0, e0);

	return d0;
}
uint16_t TX_packet(){

	// Setup Lines 
	// CLK<-HIGH
	PORTB |= (1 << PORTB0);
	// SD<-HIGH
	PORTB |= (1 << PORTB1);
	// SO<-HIGH
	PORTB |= (1 << PORTB2);


	// TX data
	uint16_t d = 0b0000001110011011;


	// Start by CLK<-LOW
	PORTB &= ~(1 << PORTB0);

	// Send start bit SD<-LOW
	PORTB &= ~(1 << PORTB1);
	_delay_us(_BIT_TIME);

	// Send data

	// Send stop bit SD<-HIGH	
	PORTB |= (1 << PORTB1);
	_delay_us(_BIT_TIME);

	// Send done signal SO<-LOW
	PORTB &= ~(1 << PORTB2);
}
