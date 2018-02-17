#include <stdio.h>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART

void setup();
void receiveByte();
void transmitByte();

int fd = -1;

int main() {
	
	setup();
	
	while(1) {
		//transmitByte();
		//sleep(1);
		receiveByte();
		sleep(1);
	}

	printf("Done!\n");
	return 0;
	
}



void setup() {
	
	const char *device = "/dev/ttyAMA0";
	fd = open(device, O_RDWR | O_NOCTTY);	
	if (fd == -1) { printf("Error - Unable to open UART.  Ensure it is not in use by another application\n"); }


	struct termios options;
	tcgetattr(fd, &options);
	
	options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;		//<Set baud rate
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &options);
	
}

void receiveByte() {
	
	if (fd != -1)
	{
		unsigned char rx_buffer[2];
		int rx_length = -1;
		int i;
		while(rx_length < 1) {
			rx_length = read(fd, (void*)rx_buffer, 1);
			if (rx_length < 0) { printf("Error!\n"); }
			else if (rx_length == 0) { printf("No data waiting\n"); }
			else
			{
				rx_buffer[rx_length] = '\0';
				printf("%i bytes read : %s\n", rx_length, rx_buffer);
			}
		}
	}
}

void transmitByte() {
	
	unsigned char tx_buffer[20];
	unsigned char *p_tx_buffer;
	
	p_tx_buffer = &tx_buffer[0];
	*p_tx_buffer++ = 'a';
	//*p_tx_buffer++ = 'b';
	//*p_tx_buffer++ = 'c';
	
	if (fd != -1)
	{
		int count = write(fd, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));		//Filestream, bytes to write, number of bytes to write
		if (count < 0)
		{
			printf("UART TX error\n");
		}
	}
	
}