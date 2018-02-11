#include "include/bitbang_uart.h"


void toogle_onboard_led() {
	PORTB ^= (1 << PORTB5);
}
void config_serial_tx(){
	UCSR0B = (1<<TXEN0);							// Transmitter Enable
	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);				// 8 bit character size
	UBRR0H = (BAUDRATE >> 8);						// Higher part of baudrate value
	UBRR0L = BAUDRATE;								// Lower part of baudrate value
}
void send_char(uint16_t c){
	while( ( UCSR0A & ( 1 << UDRE0 ) ) == 0 );
	UDR0 = c;
}
void debug_print(uint16_t s, uint16_t d, uint16_t e){
	// Debug print
	send_char(s + 48);
	for(uint8_t b = 15;b < 16; b--){
		if(d & (1 << b)) {
			send_char(49);
		}
		else {
			send_char(48);
		}
	}
	send_char(e + 48);
}


uint16_t RX_packet(){
	
	config_serial_tx();

	uint16_t s0 = 0;
	uint16_t d0 = 0;
	uint16_t e0 = 0;

	uint16_t s1 = 0;
	uint16_t d1 = 0;
	uint16_t e1 = 0;

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

	_delay_us(_BIT_TIME * 2);





	s1 = (PINB & 2) ? 1 : 0;
	for(uint8_t b = 0;b < 16; b++){
		if(PINB & 2){
			d1 |= (1 << b);
		}
		_delay_us(_BIT_TIME);
	}
	e1 = (PINB & 2) ? 1 : 0;

	debug_print(s0, d0, e0);
	debug_print(s1, d1, e1);

	
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
