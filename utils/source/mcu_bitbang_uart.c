#include "mcu_bitbang_uart.h"


uint16_t rx_packet16() {
	
	uint16_t d = 0;
	uint8_t s = 0;
	uint8_t e = 0;

	// wait for sender start bit
	_WAIT_WHILE(_GET_INPUT(_DRX_BIT));
	
	// start bit (low)
	TCNT2 = 0;
	_WAIT_WHILE(TCNT2 < 40); //FIXME?: This magic number should be closer to _HALF_BIT_TIME. 20 Works for my case
	s = _GET_INPUT(_DRX_BIT) ? 1 : 0;
	
	// data bits
	TCNT2 = 0;
	for(uint8_t bits=0;bits<16;bits++){
		_WAIT_WHILE(TCNT2 < _BIT_TIME);
		TCNT2 = 0;
		d >>= 1;
		if(_GET_INPUT(_DRX_BIT)) {
			d |= 0x8000;
		}
	}
	
	// stop bit (HIGH)
	TCNT2 = 0;
	e = _GET_INPUT(_DRX_BIT) ? 1 : 0;
	_WAIT_WHILE(TCNT2 < _BIT_TIME);
	
	// check for start OR stop bits errors
	if(s || !e) {
		_SET_HIGH(_LED_BIT);
	}
	return d;
}
void tx_packet16(uint16_t p) {
	
	// start bit (low)
	TCNT2 = 0;
	_SET_LOW(_DTX_BIT);
	_WAIT_WHILE(TCNT2 < _BIT_TIME);
	
	// data bits
	for(uint8_t bits=0;bits<16;bits++){
		TCNT2 = 0;
		if(p & 1) {
			_SET_HIGH(_DTX_BIT);
		}
		else {
			_SET_LOW(_DTX_BIT);
		}
		p >>= 1;
		_WAIT_WHILE(TCNT2 < _BIT_TIME);
	}
	
	// stop bit (HIGH)
	TCNT2 = 0;
	_SET_HIGH(_DTX_BIT);
	_WAIT_WHILE(TCNT2 < _BIT_TIME);
}



