#include "../bbang/bitbang_uart.h"
#include <avr/interrupt.h>


void setup_pin_modes(){
	_SET_INPUT(_CLOCK_BIT);
	_SET_INPUT(_SIN_BIT);
	
	_SET_INPUT(_DRX_BIT);
	_SET_OUTPUT(_DTX_BIT);
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



void master() {
	
	uint16_t buffer0 = 0xB9A0;
	
	// CLK <- LOW
	_SET_LOW(_CLOCK_BIT);
	
	// SD <- HIGH
	_SET_HIGH(_DTX_BIT);
	
	// tx
	tx_packet16(buffer0);
	
	// SO <- LOW
	_SET_LOW(_SIN_BIT);
	
	_WAIT_WHILE(!(_INPUT_PORT & _DRX_BIT));
	
	TCNT2=0;
	_WAIT_WHILE(TCNT2 < _BIT_TIME);
	
	buffer0 = 0;
	
	// rx
	buffer0 = rx_packet16();
	
	// SO <- HIGH
	_SET_HIGH(_SIN_BIT);

	send_char16(buffer0);
	
	_delay_ms(1);
	
	// CLK <- HIGH
	_SET_HIGH(_CLOCK_BIT);
}
void slave() {
	
	if(!(_INPUT_PORT & _CLOCK_BIT)) {
		
		uint16_t buffer0 = 0xB9A0;
		
		// rx
		//buffer0 = packet_rx();
		
		// wait for TX signal
		_WAIT_WHILE(_INPUT_PORT & _SIN_BIT);

		_SET_HIGH(_DTX_BIT);
		
		// delay?
		TCNT2 = 0;
		_WAIT_WHILE(TCNT2 < _BIT_TIME);
		
		// tx
		tx_packet16(buffer0);

		//send_char16(buffer0);

		// wait for transmissions end
		_WAIT_WHILE(_INPUT_PORT & _CLOCK_BIT);
		toogle_led();
	}
	

	

	
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

	buffer0 = rx_packet16();
	
	TCNT2=0;
	while(PINB & 2){};
	delay0=TCNT2;
	
	buffer1 = rx_packet16();
	
	TCNT2=0;
	while(PINB & 2){};
	delay1=TCNT2;
	
	buffer2 = rx_packet16();
	
	TCNT2=0;
	while(PINB & 2){};
	delay2=TCNT2;
	
	buffer3 = rx_packet16();
	
	TCNT2=0;
	while(PINB & 2){};
	delay3=TCNT2;
	
	send_char8(delay0);
	send_char8(delay1);
	send_char8(delay2);
	send_char8(delay3);
	
	send_char16(buffer0);
	send_char16(buffer1);
	send_char16(buffer2);
	send_char16(buffer3);

}

int main(void) {

	setup_pin_modes();
	setup_special_timer();
	cli();
	
	//_SET_HIGH(_CLOCK_BIT);
	//_SET_HIGH(_SIN_BIT);
	
	_delay_ms(1000);
	
	while(1) {
		slave();
	}




	//_SET_HIGH(_SDATA_BIT);
	
	
	
	sei();
	
	toogle_led();
	
}

