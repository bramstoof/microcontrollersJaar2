#ifndef Week5_H_
#define Week5_H_


//#include <avr/io.h>
//#include <util/delay.h>
//#include <avr/interrupt.h>
//
//#define LF			0x0a					// ascii code for Line Feed
//#define CR			0x0d					// ascii code for Carriage Return
////#define BIT(x)		(1 << (x))
//#define UART0_BAUD	2400					// Baud rate USART0
//#define MYUBRR		F_CPU/16/UART0_BAUD - 1	// My USART Baud Rate Register
//
//#define DDR_SPI DDRB // spi Data direction register
//#define PORT_SPI PORTB // spi Output register
//#define SPI_SCK 1 // PB1: spi Pin System Clock
//#define SPI_MOSI 2 // PB2: spi Pin MOSI
//#define SPI_MISO 3 // PB3: spi Pin MISO
//#define SPI_SS 0 // PB0: spi Pin Slave Select
//
//
//void usart0_init( void )	;				// initialize USART0 - receive/transmit
//
//void usart0_start( void ) ;					// UART0 receiver & transmitter enable
//
//char uart0_receiveChar() ;					// UART0: receive ch
//
//int uart0_sendChar( char ch ) ;				// UART0: send ch
//
//int uart0_receiveString( char* string);		// UART0: receive string until LF

void week5Opdracht1(void);
//void week5Opdracht2(void);


#endif