#include <avr/interrupt.h>
#include "include/bitbang_uart.h"




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



int on = 0;
int main(void) {
	
	setup_pin_modes();

	RX_packet();

	
}

/*
ISR (TIMER1_COMPA_vect)
{
	cli();
	toogle_onboard_led();
	sei();
}

ISR (INT0_vect)
{
	cli();

	sei();
}*/

