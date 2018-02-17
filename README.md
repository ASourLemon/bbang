MCU Programming
=======================================

This is a repository for my microcontroller programming experiences. 

For now, the goal is to implement a 16 bit, RS232-like, signal receiver.

The target hardware is the Atmel ATmega328P.

For testing, I used TERMIOS(3), 8Bit UART on a Raspberry Pi 3 Model B.


References
--------------------------------------
- [Arduino Duemilanove Schematics](https://www.arduino.cc/en/uploads/Main/arduino-duemilanove-schematic.pdf)
- [ATmega168/328 Port Manipulation](https://www.arduino.cc/en/Reference/PortManipulation)
- [ATmega168/328 Timers](https://sites.google.com/site/qeewiki/books/avr-guide/timers-on-the-atmega328)
- [Raspberry Pi gPIo](https://docs.microsoft.com/en-us/windows/iot-core/media/pinmappingsrpi/rp2_pinout.png)
- [Linux TERMIOS(3) Documentation](http://man7.org/linux/man-pages/man3/termios.3.html)
- [Raspberry Pi TERMIOS UART](http://www.raspberry-projects.com/pi/programming-in-c/uart-serial-port/using-the-uart)
