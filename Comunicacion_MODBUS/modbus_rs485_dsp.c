// compile with: gcc -o modB0 modbus_rs485_dsp.c -lwiringPi

#include <string.h>
#include <stdlib.h>

#include <stdio.h>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART

#include <wiringPi.h>
#include <errno.h>

#include "CRC.h"
#include <stdint.h>

//WORD CRC16 (const BYTE *nData, WORD wLength);

int main ()
{
	//-------------------------
	//----- SETUP USART 0 -----
	//-------------------------
	//At bootup, pins 8 and 10 are already set to UART0_TXD, UART0_RXD (ie the alt0 function) respectively
	int uart0_filestream = -1;
	
	//OPEN THE UART
	//The flags (defined in fcntl.h):
	//	Access modes (use 1 of these):
	//		O_RDONLY - Open for reading only.
	//		O_RDWR - Open for reading and writing.
	//		O_WRONLY - Open for writing only.
	//
	//	O_NDELAY / O_NONBLOCK (same function) - Enables nonblocking mode. When set read requests on the file can return immediately with a failure status
	//											if there is no input immediately available (instead of blocking). Likewise, write requests can also return
	//											immediately with a failure status if the output can't be written immediately.
	//
	//	O_NOCTTY - When set and path identifies a terminal device, open() shall not cause the terminal device to become the controlling terminal for the process.
	uart0_filestream = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
	//uart0_filestream = open("/dev/ttyS0", O_RDWR | O_NOCTTY);
	
	if (uart0_filestream == -1)
	{
		//ERROR - CAN'T OPEN SERIAL PORT
		printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
	}
	
	//CONFIGURE THE UART
	//The flags (defined in /usr/include/termios.h - see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html):
	//	Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
	//	CSIZE:- CS5, CS6, CS7, CS8                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
	//	CLOCAL - Ignore modem status lines
	//	CREAD - Enable receiver
	//	IGNPAR = Ignore characters with parity errors
	//	ICRNL - Map CR to NL on input (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
	//	PARENB - Parity enable
	//	PARODD - Odd parity (else even)
	
	struct termios options;
	tcgetattr(uart0_filestream, &options);
	options.c_cflag = B19200 | CS8 | CLOCAL | CREAD;		//<Set baud rate
	//options.c_cflag &= (PARENB | PARODD);
	//options.c_cflag &= CSTOPB;
	//options.c_cflag = B19200 | CS8 | CLOCAL | CREAD;
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uart0_filestream, TCIFLUSH);
	tcsetattr(uart0_filestream, TCSANOW, &options);
	
	//--------Pulse config-----------
	
	int baudR = 19200; 
	double pulse = (1.0/baudR)*80.0*1000000.0;
	
	if (wiringPiSetup () == -1)					/* initializes wiringPi setup */
	{
		fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
		return 1 ;
	}
	pinMode(1,OUTPUT);
	digitalWrite(1,LOW);	
	
	//----- TX BYTES -----
	unsigned char tx_buffer[8] ;
	unsigned char *p_tx_buffer;
	unsigned char tx_buffer_DSP[20] ;
	unsigned char *p_tx_buffer_DSP;
	char dir = 0x09;
	char func = 0x03;
	char varReg[] = {0x10,0x02};
	char NumRegs[] = {0x00,0x02};
	//char datOUT[] = {0x1F,0x03,0x10,0x02,0x00,0x02,0x62,0xB5};
	
	p_tx_buffer = &tx_buffer[0];
	//*p_tx_buffer++ = 0xFA;
	//*p_tx_buffer++ = 0xF5;
	//*p_tx_buffer++ = 0xAA;
	//*p_tx_buffer++ = 0x55;
	//*p_tx_buffer++ = 0xAA;
	
	*p_tx_buffer++ = '@';
	*p_tx_buffer++ = '8';
	*p_tx_buffer++ = '1';
	*p_tx_buffer++ = '2';
	*p_tx_buffer++ = '3';
	
	printf("En Envio\n");
	char rx_buffer[64];
	memset(rx_buffer,0,sizeof(rx_buffer));
	int r_bytes=0,noRegs=0;
	char aux[4];
	uint32_t data;
	
	while(1){
		
		//Solicitud a Medidor 
		
		if (uart0_filestream != -1)
		{
			
			int count = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));		//Filestream, bytes to write, number of bytes to write
			//int count = write(uart0_filestream, &tx_buffer[0], 1);		//Filestream, bytes to write, number of bytes to write
			//count = write(uart0_filestream, &tx_buffer[1], 1);		//Filestream, bytes to write, number of bytes to write
			//count = write(uart0_filestream, &tx_buffer[2], 1);		//Filestream, bytes to write, number of bytes to write
			//count = write(uart0_filestream, &tx_buffer[3], 1);		//Filestream, bytes to write, number of bytes to write
			//count = write(uart0_filestream, &tx_buffer[4], 1);		//Filestream, bytes to write, number of bytes to write
			
			
			
			printf("%i bytes sent : %c", count, tx_buffer[0] );
			//for(int i=1;i<sizeof(tx_buffer)-1;i++){
			for(int i=1;i<4;i++){
				printf ("%c", tx_buffer[i] ) ;
			}
			printf ("%c\n", tx_buffer[4] ) ;
			if (count < 0)
			{
				printf("UART TX error\n");
			}
			
		}
		delay(80);
		//----- CHECK FOR ANY RX BYTES -----
		//_____________Recibe respuesta del medidor________________
		if (uart0_filestream != -1)
		{
			printf("Inicia recepcion\n");
			// Read up to 255 characters from the port if they are there
			
			int rx_length =0;
			rx_length = read(uart0_filestream, &rx_buffer, sizeof(rx_buffer));
			printf("Lee\n");
			if (rx_length < 0)
			{
				//An error occured (will occur if there are no bytes)
				printf("fewer bytes\n");
			}
			else if (rx_length == 0)
			{
				//No data waiting
				printf("no data\n");
			}
			else
			{
				//Bytes received
				rx_buffer[rx_length] = '\0';
				char *p_rx_buffer = rx_buffer;
				printf("%i bytes read\n", rx_length);
				for(int i=0;i<rx_length;i++){
				  //printf ("%x\n", rx_buffer[i] & 0xff) ;
				  printf ("%x\n", *(p_rx_buffer++) & 0xff) ;
				}
				//printf ("%x\n", rx_buffer[7] & 0xff) ;
				//memset(rx_buffer,0,sizeof(rx_buffer));
				tcflush(uart0_filestream, TCIFLUSH);
			}
			
			if(rx_buffer[0]!=dir){
				printf("Not the Dir %x\n",rx_buffer[0] & 0xff);
			}
			else if (rx_buffer[1]!=func)
			{
				//No data waiting
				printf("Wrong Func\n");
			}
			else
			{
				r_bytes=rx_buffer[2];
				printf("No. Bytes Received %d\n",r_bytes);
				noRegs = r_bytes/4;
				for(int i=0;i<noRegs;i++){
					for(int j=0;j<4;j++){
						aux[3-j]=rx_buffer[3+(j+(i*4))];
					}
					data = *(uint32_t *)aux;
					printf("%d\n",data);
				}
				
				
			}
			
		}
		printf("*********Med Recv end*********\n");
		delay(700);
		
	}
}
