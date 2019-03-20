#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "wait.h"
#include "Week5.h"

#define BIT(x) ( 1<<x )
#define DDR_SPI DDRB // spi Data direction register
#define PORT_SPI PORTB // spi Output register
#define SPI_SCK 1 // PB1: spi Pin System Clock
#define SPI_MOSI 2 // PB2: spi Pin MOSI
#define SPI_MISO 3 // PB3: spi Pin MISO
#define SPI_SS 0 // PB0: spi Pin Slave Select


void spi_masterInit(void)
{
	DDR_SPI = 0xff; // All pins output: MOSI, SCK, SS, SS_display
	DDR_SPI &= ~BIT(SPI_MISO); // except: MISO input
	PORT_SPI |= BIT(SPI_SS); // SS_ADC == 1: deselect slave
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1); // or: SPCR = 0b11010010;
	// Enable spi, MasterMode, Clock rate fck/64
	// bitrate=125kHz, Mode = 0: CPOL=0, CPPH=0
}
// Write a byte from master to slave
void spi_write( unsigned char data )
{
	SPDR = data; // Load byte --> starts transmission
	while( !(SPSR & BIT(SPIF)) ); // Wait for transmission complete
}
// Write a byte from master to slave and read a byte from slave - not used here
char spi_writeRead( unsigned char data )
{
	SPDR = data; // Load byte --> starts transmission
	while( !(SPSR & BIT(SPIF)) ); // Wait for transmission complete
	data = SPDR; // New received data (eventually, MISO) in SPDR
	return data; // Return received byte
}
// Select device on pinnumer PORTB
void spi_slaveSelect(unsigned char chipNumber)
{
	PORTB &= ~BIT(chipNumber);
}
// Deselect device on pinnumer PORTB
void spi_slaveDeSelect(unsigned char chipNumber)
{
	PORTB |= BIT(chipNumber);
}

// Initialize the driver chip (type MAX 7219)
void displayDriverInit()
{
	spi_slaveSelect(0); // Select display chip (MAX7219)
	spi_write(0x09); // Register 09: Decode Mode
	spi_write(0xFF); // -> 1's = BCD mode for all digits
	spi_slaveDeSelect(0); // Deselect display chip
	spi_slaveSelect(0); // Select dispaly chip
	spi_write(0x0A); // Register 0A: Intensity
	spi_write(0x04); // -> Level 4 (in range [1..F])
	spi_slaveDeSelect(0); // Deselect display chip
	spi_slaveSelect(0); // Select display chip
	spi_write(0x0B); // Register 0B: Scan-limit
	spi_write(0x01); // -> 1 = Display digits 0..1
	spi_slaveDeSelect(0); // Deselect display chip
	spi_slaveSelect(0); // Select display chip
	spi_write(0x0C); // Register 0B: Shutdown register
	spi_write(0x01); // -> 1 = Normal operation
	spi_slaveDeSelect(0); // Deselect display chip
}
// Set display on ('normal operation')
void displayOn()
{
	spi_slaveSelect(0); // Select display chip
	spi_write(0x0C); // Register 0B: Shutdown register
	spi_write(0x01); // -> 1 = Normal operation
	spi_slaveDeSelect(0); // Deselect display chip
}
// Set display off ('shut down')
void displayOff()
{
	spi_slaveSelect(0); // Select display chip
	spi_write(0x0C); // Register 0B: Shutdown register
	spi_write(0x00); // -> 1 = Normal operation
	spi_slaveDeSelect(0); // Deselect display chip
}

void spi_writeWord(unsigned char adress, unsigned char data )
{
	spi_slaveSelect(0); // Select display chip
	spi_write(adress); // digit adress: (digit place)
	spi_write(data); // digit value: 0
	spi_slaveDeSelect(0); // Deselect display chip
}

void writeLedDisplay( int value )
{
	if(value > 9999){
		value = 9999;
	}
	if(value < 0)
	{
		if(value < -999)
		{
			value = -999;
		}

		int tempValue = abs(value);
		int num3 = (tempValue / 1) % 10;
		int num2 = (tempValue / 10) % 10;
		int num1 = (tempValue / 100) % 10;

		spi_writeWord(4, 10);
		spi_writeWord(3, num1);
		spi_writeWord(2, num2);
		spi_writeWord(1, num3);

	}else
	{
		int num4 = (value / 1) % 10;
		int num3 = (value / 10) % 10;
		int num2 = (value / 100) % 10;
		int num1 = (value / 1000) % 10;

		spi_writeWord(4, num1);
		spi_writeWord(3, num2);
		spi_writeWord(2, num3);
		spi_writeWord(1, num4);
	}
}



void week5Opdracht1()
{
	
	
	DDRB=0x01; // Set PB0 pin as output for display select
	spi_masterInit(); // Initialize spi module
	displayDriverInit(); // Initialize display chip
	// clear display (all zero's)
	for (char i =1; i<=4; i++)
	{
		spi_writeWord(i, 0);
	}
	wait(1000);
	// write 4-digit data
	writeLedDisplay(-115);
	wait(10000);
	writeLedDisplay(4356);
	wait(1000);

}


























//#define F_CPU 8000000
//#include <avr/io.h>
//#include <util/delay.h>
//#include <avr/interrupt.h>
//#include "Week5.h"
//#include "wait.h"
//
//#define BIT(x)		(1 << (x))
////#define UART0_BAUD	9600					// Baud rate USART0
//#define MYUBRR		F_CPU/16/UART0_BAUD - 1	// My USART Baud Rate Register
////
//
//char character;
//
//#define LCD_E 	3
//#define LCD_RS	2
//#define LED PD4
//
//void wait();
//void lcd_strobe_lcd_e(void);
//void init_4bits_mode(void);
//void lcd_write_string(char *str);
//void lcd_write_data(unsigned char byte);
//void lcd_write_command(unsigned char byte);
//
//void lcd_strobe_lcd_e(void)
//{
	//PORTC |= (1<<LCD_E);	// E high
	//_delay_ms(1);			// nodig
	//PORTC &= ~(1<<LCD_E);  	// E low
	//_delay_ms(1);			// nodig?
//}
//
//void init_4bits_mode(void)
//{
	//// PORTC output mode and all low (also E and RS pin)
	//DDRC = 0xFF;
	//PORTC = 0x00;
//
	//// Step 2 (table 12)
	//PORTC = 0x20;	// function set
	//lcd_strobe_lcd_e();
//
	//// Step 3 (table 12)
	//PORTC = 0x20;   // function set
	//lcd_strobe_lcd_e();
	//PORTC = 0x80;
	//lcd_strobe_lcd_e();
//
	//// Step 4 (table 12)
	//PORTC = 0x00;   // Display on/off control
	//lcd_strobe_lcd_e();
	//PORTC = 0xF0;
	//lcd_strobe_lcd_e();
//
	//// Step 4 (table 12)
	//PORTC = 0x00;   // Entry mode set
	//lcd_strobe_lcd_e();
	//PORTC = 0x60;
	//lcd_strobe_lcd_e();
//
//}
//
//void lcd_write_string(char *str)
//{
	//// Het kan met een while:
//
	//// while(*str) {
	//// 	lcd_write_data(*str++);
	//// }
//
	//// of met een for:
	//for(;*str; str++){
		//lcd_write_data(*str);
	//}
//}
//
//void lcd_write_data(unsigned char byte)
//{
	//// First nibble.
	//PORTC = byte;
	//PORTC |= (1<<LCD_RS);
	//lcd_strobe_lcd_e();
//
	//// Second nibble
	//PORTC = (byte<<4);
	//PORTC |= (1<<LCD_RS);
	//lcd_strobe_lcd_e();
//}
//
//void lcd_write_command(unsigned char byte)
//{
	//// First nibble.
	//PORTC = byte;
	//PORTC &= ~(1<<LCD_RS);
	//lcd_strobe_lcd_e();
//
	//// Second nibble
	//PORTC = (byte<<4);
	//PORTC &= ~(1<<LCD_RS);
	//lcd_strobe_lcd_e();
//}
//
//void lcd_init()
//{
	//lcd_write_command(0x30);      // Configure the LCD in 8-bit mode, 1 line and 5x7 font
	//lcd_write_command(0x0C);      // Display On and Cursor Off
	//lcd_write_command(0x01);      // Clear display screen
	//lcd_write_command(0x06);      // Increment cursor
	//lcd_write_command(0x80);      // Set cursor position to 1st line, 1st column
//}
//
//
//
//
//void spi_masterInit(void)
//{
	//DDR_SPI = 0xff; // All pins output: MOSI, SCK, SS, SS_display
	//DDR_SPI &= ~BIT(SPI_MISO); // except: MISO input
	//PORT_SPI |= BIT(SPI_SS); // SS_ADC == 1: deselect slave
	//SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1); // or: SPCR = 0b11010010;
	//// Enable spi, MasterMode, Clock rate fck/64
	//// bitrate=125kHz, Mode = 0: CPOL=0, CPPH=0
//}
//
//// Write a byte from master to slave
//void spi_write( unsigned char data )
//{
	//SPDR = data; // Load byte --> starts transmission
	//while( !(SPSR & BIT(SPIF)) ); // Wait for transmission complete
//}
//
//// Write a byte from master to slave and read a byte from slave - not used here
//char spi_writeRead( unsigned char data )
//{
	//SPDR = data; // Load byte --> starts transmission
	//while( !(SPSR & BIT(SPIF)) ); // Wait for transmission complete
	//data = SPDR; // New received data (eventually, MISO) in SPDR
	//return data; // Return received byte
//}
//
//// Select device on pinnumer PORTB
//void spi_slaveSelect(unsigned char chipNumber)
//{
	//PORTB &= ~BIT(chipNumber);
//}
//
//// Deselect device on pinnumer PORTB
//void spi_slaveDeSelect(unsigned char chipNumber)
//{
	//PORTB |= BIT(chipNumber);
//}
//
//// Initialize the driver chip (type MAX 7219)
//void displayDriverInit()
//{
	//spi_slaveSelect(0); // Select display chip (MAX7219)
	//spi_write(0x09); // Register 09: Decode Mode
	//spi_write(0xFF); // -> 1's = BCD mode for all digits
	//spi_slaveDeSelect(0); // Deselect display chip
	//spi_slaveSelect(0); // Select dispaly chip
	//spi_write(0x0A); // Register 0A: Intensity
	//spi_write(0x04); // -> Level 4 (in range [1..F])
	//spi_slaveDeSelect(0); // Deselect display chip
	//spi_slaveSelect(0); // Select display chip
	//spi_write(0x0B); // Register 0B: Scan-limit
	//spi_write(0x01); // -> 1 = Display digits 0..1
	//spi_slaveDeSelect(0); // Deselect display chip
	//spi_slaveSelect(0); // Select display chip
	//spi_write(0x0C); // Register 0B: Shutdown register
	//spi_write(0x01); // -> 1 = Normal operation
	//spi_slaveDeSelect(0); // Deselect display chip
//}
//
//// Set display on ('normal operation')
//void displayOn()
//{
	//spi_slaveSelect(0); // Select display chip
	//spi_write(0x0C); // Register 0B: Shutdown register
	//spi_write(0x01); // -> 1 = Normal operation
	//spi_slaveDeSelect(0); // Deselect display chip
//}
//
//// Set display off ('shut down')
//void displayOff()
//{
	//spi_slaveSelect(0); // Select display chip
	//spi_write(0x0C); // Register 0B: Shutdown register
	//spi_write(0x00); // -> 1 = Normal operation
	//spi_slaveDeSelect(0); // Deselect display chip
//}
//int mainx()
//{
	//DDRB=0x01; // Set PB0 pin as output for display select
	//spi_masterInit(); // Initialize spi module
	//displayDriverInit(); // Initialize display chip
	//
	//// clear display (all zero's)
	//for (char i =1; i<=2; i++)
	//{
		//spi_slaveSelect(0); // Select display chip
		//spi_write(i); // digit adress: (digit place)
		//spi_write(0); // digit value: 0
		//spi_slaveDeSelect(0); // Deselect display chip
	//}
	//
	//wait(1000);
	//// write 4-digit data
	//for (char i =1; i<=2; i++)
	//{
		//spi_slaveSelect(0); // Select display chip
		//spi_write(i); // digit adress: (digit place)
		//spi_write(i); // digit value: i (= digit place)
		//spi_slaveDeSelect(0); // Deselect display chip
		//wait(1000);
	//}
	//
	//wait(1000);
	//return (1);
//}
//
//
//void usart0_init( void )						// initialize USART0 - receive/transmit
//{
	//int	ubrr = MYUBRR;
	//UBRR0H = ubrr>>8;						// baudrate register, hoge byte
	//UBRR0L = ubrr;							// baudrate register, lage byte
	//UCSR0C = 0b00000110;					// asynchroon, 8 data - 1 stop - no parity
	//UCSR0B = 0b00000000;					// receiver & transmitter enable
//}
//
//void usart0_start( void )					// receiver & transmitter enable
//{
	//UCSR0B |= BIT(RXEN)|BIT(TXEN);			// set bit RXEN = Receiver enable and TXEN = Transmitter enable
//}
//
//int uart0_sendChar( char ch )
//{
	//while (!(UCSR0A & BIT(UDRE0))) ;		// wait until UDRE0 is set: transmit buffer is ready
	//UDR0 = ch;								// send ch
	//return 0;								// OK
//}
//
//char uart0_receiveChar( void )
//{
	//while (!(UCSR0A & BIT(RXC0))) ;			// if RX0 is set: unread data present in buffer
	//return UDR0;							// read ch
//}
//
//// Main program: USART0: send & receive
//void week5Opdracht1( void )
//{
	//DDRB = 0xFF;							// set PORTB for output, for testing
	//DDRA = 0xFF;							// set PORTA for output, for testing
	//usart0_init();							// initialize USART0
	//usart0_start();							// uart0: start send & receive
//
	//while (1)
	//{
		//wait(50);							// every 50 ms (busy waiting)
		//PORTB ^= BIT(7);					// toggle bit 7 for testing
//
		//character = uart0_receiveChar();	// read char
		//PORTA = character;					// show read character, for testing
//
		//uart0_sendChar(character);			// send back
	//}
//}
//
//void week5Opdracht2()
//{
		//char buffer[16];						// declare string buffer
		//DDRB = 0xFF;							// set PORTB for output
//
		//lcd_init();								// initialize LCD-display
		//usart0_init();							// initialize USART0
		//usart0_start();
//
		//while (1)
		//{
			//wait(50);							// every 50 ms (busy waiting)
			//PORTB ^= BIT(7);					// toggle bit 7 for testing
//
			//uart0_receiveString( buffer );		// receive string from uart
			//
			//// write string to LCD display
		//}
//}