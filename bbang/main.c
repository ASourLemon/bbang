#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL

#define _SIGNAL_BAUD 115200
#define _RAW_BIT_TIME 1/BAUD
#define _RAW_HALF_BIT_TIME _BIT_TIME/2
#define _BIT_TIME F_CPU/1/_SIGNAL_BAUD-1
#define _HALF_BIT_TIME _BIT_TIME/2

#define _CONTROL_PORT DDRB
#define _INPUT_PORT PINB
#define _OUTPUT_PORT PORTB

#define _CLOCK_BIT 0
#define _SDATA_BIT 1
#define _SIN_BIT 2
#define _LED_BIT 5

#define _SET_INPUT(x) _CONTROL_PORT &= ~(1 << x)
#define _SET_OUTPUT(x) _CONTROL_PORT |= (1 << x)
#define _SET_LOW(x) _OUTPUT_PORT &= ~(1 << x)
#define _SET_HIGH(x) _OUTPUT_PORT |= (1 << x)
#define _GET_INPUT(x) _INPUT_PORT & (1 << x)

#define _WAIT_WHILE(x) while(x){};
	

#define _DEBUG_BAUD 9600
#define _DEBUG_BAUDRATE F_CPU/16/_DEBUG_BAUD-1



void setup_pin_modes(){
	_SET_INPUT(_CLOCK_BIT);
	_SET_INPUT(_SIN_BIT);
	_SET_INPUT(_SDATA_BIT);
	_SET_OUTPUT(_LED_BIT);
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
void setup_special_timer() {
	
	// Disable timer0 and timer1
	TIMSK0 = 0;
	TIMSK1 = 0;
	
	// Max of 8us
	OCR2A = 0xFF;
	
	// CTC mode 2
	TCCR2A |= (1 << WGM21);
	// No prescaler
	TCCR2B |= (1 << CS20);
	
	//Set interrupt on compare match
	TIMSK2 |= (1 << OCIE2A);

	cli();
}



// Debug utils
void setup_debug_tx(){
	UCSR0B = (1<<TXEN0);							// Transmitter Enable
	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);				// 8 bit character size
	UBRR0H = (_DEBUG_BAUDRATE >> 8);				// Higher part of baudrate value
	UBRR0L = _DEBUG_BAUDRATE;						// Lower part of baudrate value
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



uint16_t packet_rx() {
	
	uint16_t d = 0;
	uint8_t s = 0;
	uint8_t e = 0;

	// wait for sender start bit
	_WAIT_WHILE(_GET_INPUT(_SDATA_BIT));
	
	// start bit (low)
	TCNT2 = 0;
	_WAIT_WHILE(TCNT2 < 20); //FIXME?: This magic number should be closer to _HALF_BIT_TIME. 20 Works for my case
	s = _GET_INPUT(_SDATA_BIT) ? 1 : 0;
	
	// data bits
	TCNT2 = 0;
	for(uint8_t bits=0;bits<16;bits++){
		_WAIT_WHILE(TCNT2 < _BIT_TIME);
		TCNT2 = 0;
		d >>= 1;
		if(_GET_INPUT(_SDATA_BIT)) {
			d |= 0x8000;
		}
	}
	
	// stop bit (HIGH)
	TCNT2 = 0;
	e = _GET_INPUT(_SDATA_BIT) ? 1 : 0;
	_WAIT_WHILE(TCNT2 < _BIT_TIME);
	
	// check for start OR stop bits errors
	if(s || !e) {
		_SET_HIGH(_LED_BIT);
	}
	return d;
}
void packet_tx(uint8_t packet) {
	
	// start bit (low)
	TCNT2 = 0;
	_SET_LOW(_SDATA_BIT);
	_WAIT_WHILE(TCNT2 < 138);
	
	// data bits
	for(uint8_t bits=0;bits<8;bits++){
		TCNT2 = 0;
		if(packet & 1) {
			_SET_HIGH(_SDATA_BIT);
		}
		else {
			_SET_LOW(_SDATA_BIT);
		}
		packet >>= 1;
		_WAIT_WHILE(TCNT2 < 138);
	}
	
	// stop bit (HIGH)
	TCNT2 = 0;
	_SET_HIGH(_SDATA_BIT);
	_WAIT_WHILE(TCNT2 < 138);
}


void master() {
	
	uint16_t buffer0 = 0xB9A0;
	// CLK <- LOW
	PORTB &= ~4;
			
	// SD <- HIGH
	PORTB |= 2;
	DDRB |= 2;
	packet_tx(buffer0);
			
	// SO <- LOW
	PORTB &= ~1;
	DDRB &= ~2;
	buffer0 = packet_rx();
			
	// SD <- HIGH
	PORTB |= 2;
	for(uint8_t bits=0;bits<34;bits++){
		TCNT2 = 0;
		while(TCNT2 < 138){};
	}
	// CLK <- HIGH
	PORTB |= 4;
}
void slave() {
	uint16_t buffer0 = 0xB9A0;
	
	
	while(PINB & 4){};
	
	/* RX
	DDRB &= ~2;
	buffer0 = packet_rx();*/
			
	// wait for ready signal
	while(PINB & 1){};

	/* delay
	TCNT2 = 0;
	while(TCNT2 < 138){};*/
					
	// TX
	DDRB |= 2;
	packet_tx(buffer0);
	
}
void sniffer() {
	
	uint16_t buffer0 = 0;
	uint16_t buffer1 = 0;
	uint16_t buffer2 = 0;
	uint16_t buffer3 = 0;
	
	uint8_t delay0 = 0;
	uint8_t delay1 = 1;
	uint8_t delay2 = 0;
	uint8_t delay3 = 1;

	while(PINB & 4){};

	buffer0 = packet_rx();
	
	TCNT2=0;
	while(PINB & 2){};
	delay0=TCNT2;
	
	buffer1 = packet_rx();
	
	TCNT2=0;
	while(PINB & 2){};
	delay1=TCNT2;
	
	buffer2 = packet_rx();
	
	TCNT2=0;
	while(PINB & 2){};
	delay2=TCNT2;
	
	buffer3 = packet_rx();
	
	TCNT2=0;
	while(PINB & 2){};
	delay3=TCNT2;
	
	send_char8(delay0);
		send_char8(delay1);
			send_char8(delay2);
				send_char8(delay3);
	
	/*/*send_char16(buffer0);
	send_char16(buffer1);
	send_char16(buffer2);
	send_char16(buffer3);*/

}

int main(void) {

	setup_debug_tx();
	setup_pin_modes();
	setup_special_timer();
	cli();
	
	_SET_OUTPUT(_SDATA_BIT);
	
	uint8_t b = 0B01100001;
	while(1) {
		packet_tx(b);
	}

	


	//_SET_HIGH(_SDATA_BIT);
	
	
	
	sei();
	
	toogle_led();
	
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
