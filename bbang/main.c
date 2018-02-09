#include <avr/io.h>
#include <avr/interrupt.h>
#include "include/bitbang_uart.h"

#define F_CPU 16000000L


int isOn;

void setup(){
	
	isOn = 1;
	
	cli();
	
	// Config pin 13 as output
	DDRB |= 0B00100000;
	
	// Disable Timer0 and Timer2
	TIMSK0 = 0;
	TIMSK2 = 0;
	
	// Config Timer1
	// OCRx = (F_CPU / Prescaler * T) - 1
	OCR1A = 0x3D08;								
	
	// CTC Mode 4
	TCCR1B |= (1 << WGM12);	
	
	// 1024 Prescaler
	TCCR1B |= (1 << CS12) | (1 << CS10);
	
	// Enable Match Compare Interrupt  
	TIMSK1 |= (1 << OCIE1A);	
	
	sei();
}


int main(void)
{
	
	setup();
	
    while (1);
	
}

ISR (TIMER1_COMPA_vect)
{
	cli();
	if(PINB) {
		PORTB = 0;
	}
	else {
		PORTB = 32;
	}
	sei();
}


