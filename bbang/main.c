#include <avr/io.h>
#include <stdint.h>

#define F_CPU 16000000UL
#define _CLK DDB0
#define _SDI DDB1

#define BAUD 9600
#define BAUDRATE F_CPU/16/BAUD-1


#define _BAUD 115200
#define _BIT_TIME 8.68055
#define _HALF_BIT_TIME 4.34027

#include <util/delay.h>
#include <avr/interrupt.h>



void toogle_onboard_led() {
	PORTB ^= (1 << PORTB5);
}
void setup_pin_modes(){
	
	// Config pin 13 as output
	DDRB |= (1 << DDB5);
	
	// Config pin 8 as input (Clock)
	DDRB &= ~(1 << _CLK); 
	//DDRB |= (1 << _CLK);
	
	// Config pin 9 as input (Serial Data In)
	DDRB &= ~(1 << _SDI); 
	//DDRB |= (1 << _SDI);
}
void setup_internal_int() {
	
	// Disable timer0 and timer2
	TIMSK0 = 0;
	TIMSK2 = 0;
	
	// Config timer1
	// OCRx = (F_CPU / Prescaler * T) - 1
	OCR1A = 0x3D08;								
	
	// CTC mode 4 1024 prescaler
	TCCR1B |= (1 << CS12) | (1 << CS10) | (1 << WGM12);;
	
	// Enable match compare interrupt  
	TIMSK1 |= (1 << OCIE1A);	
}
void setup_external_int() {
	
	// Trigger interrupt on LOW signal 
	EICRA = 0;
	
	// Disable interrupt INT1
	EIMSK = 0;
	
	// Enable interrupt INT0
	EIMSK |= (1 << INT0);
	
}
void setup_serial_tx(){
	UCSR0B = (1<<TXEN0);							// Transmitter Enable
	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);				// 8 bit character size
	UBRR0H = (BAUDRATE >> 8);						// Higher part of baudrate value
	UBRR0L = BAUDRATE;								// Lower part of baudrate value
}
void send_char(uint8_t c){
	while( ( UCSR0A & ( 1 << UDRE0 ) ) == 0 );
	UDR0 = c;
}
void debug_print_bin(uint16_t d){
	// Debug print
	for(uint8_t b = 15;b < 16; b--){
		if(d & (1 << b)) {
			send_char(49);
		}
		else {
			send_char(48);
		}
	}
}
void debug_print_hex(uint16_t d){
	// Debug print
	for(uint8_t b = 0;b < 4; b++){
		send_char(d);
		d << 4;
	}
}

void setup_special_timer() {
	
	// Disable timer0 and timer1
	TIMSK0 = 0;
	TIMSK1 = 0;
	
	// Max of 8us
	OCR2A = 200;
	
	// CTC mode 2
	TCCR2A |= (1 << WGM21);	
	// No prescaler
	TCCR2B |= (1 << CS20);
	
	//Set interrupt on compare match
	TIMSK2 |= (1 << OCIE2A);

}


uint8_t packet_rx() {
	uint8_t d = 0;
	while(PINB & 2);
	TCNT2 = 0;
	while(TCNT2 < 69)
	for(uint8_t bits=0;bits<8;bits++){
		TCNT2 = 0;
		while(TCNT2 < 138);
		d >>= 1;
		if(PINB & 2) {
			d |= 0x80;
		}
	}
	return d;
}


int main(void) {

	setup_serial_tx();
	setup_pin_modes();
	setup_special_timer();
	cli();
	
	
	uint8_t d0 = packet_rx();
	send_char(d0);
	
	
	toogle_onboard_led();
}

/*
ISR (TIMER2_COMPA_vect)
{
	cli();
	sei();
}
ISR (INT0_vect)
{
	cli();

	sei();
}
*/
