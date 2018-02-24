#include "mcu_general.h"






//* Debug utils
void setup_debug_tx(){
	UCSR0B = (1<<TXEN0);					// Transmitter Enable
	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);			// 8 bit character size
	uint8_t higher = (_DEBUG_BAUDRATE >> 8);
	UBRR0H = (higher);
	UBRR0L = _DEBUG_BAUDRATE;
}
void send_char8(uint8_t c){
	while(( UCSR0A & ( 1 << UDRE0 )) == 0 ){};
	UDR0 = c;
}
void send_char16(uint16_t c){
	send_char8(c >> 8);
	send_char8(c);
}
void send_char32(uint32_t c){
	send_char16(c >> 16);
	send_char16(c);
}
void toogle_led() {
	_OUTPUT_PORT ^= (1 << _LED_BIT);
}
