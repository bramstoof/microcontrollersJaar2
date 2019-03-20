#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>


#define BIT(x)			(1 << (x))

#define LCD_E 	3
#define LCD_RS	2
char value[10];
void wait();

void timer1Init( void )
{
	OCR1A = 0;					// RED, default, off
	OCR1B = 0;					// GREEN, default, off
	OCR1C = 0;					// BLUE, default, off
	TCCR1A = 0b10101001;		// compare output OC1A,OC1B,OC1C
	TCCR1B = 0b00001011;		// fast PWM 8 bit, prescaler=64, RUN
}

// Set pulse width for RED on pin PB5, 0=off, 255=max
void setRed( unsigned char red )
{
	OCR1A = red;
}

// void setGreen( unsigned char green)
// void setBlue( unsigned char blue)

void adcInit2( void )
{
	ADMUX = 0b01100001;			// AREF=VCC, result left adjusted, channel1 at pin PF1
	ADCSRA = 0b11100110;		// ADC-enable, no interrupt, start, free running, division by 64
}

void adcInit3( void )
{
	ADMUX = 0b11100001;			// AREF=2,56 V, result left adjusted, channel1 at pin PF1
	ADCSRA = 0b10000110;		// ADC-enable, no interrupt, no free running, division by 64
}
void lcd_strobe_lcd_e3b(void)
/*
short:			Strobe LCD module E pin --__
inputs:
outputs:
notes:			According datasheet HD44780
Version :    	DMK, Initial code
*******************************************************************/
{
	PORTC |= (1<<LCD_E);	// E high
	wait(1);			// nodig
	PORTC &= ~(1<<LCD_E);  	// E low
	wait(1);			// Je weet maar nooit
}

void lcd_write_command3b(unsigned char byte)
/*
short:			Writes 8 bits COMMAND to lcd
inputs:			byte - written to LCD
outputs:
notes:			According datasheet HD44780 table 12
Version :    	DMK, Initial code
*******************************************************************/
{
	// First nibble.
	PORTC = byte;
	PORTC &= ~(1<<LCD_RS);
	lcd_strobe_lcd_e3b();

	// Second nibble
	PORTC = (byte<<4);
	PORTC &= ~(1<<LCD_RS);
	lcd_strobe_lcd_e3b();
}

void clear3b()
{
	lcd_write_command3b(0x01);
	lcd_write_command3b(0x02);
}

void init3b(){
	// PORTC output mode and all low (also E and RS pin)
	DDRC = 0xFF;
	PORTC = 0x00;

	// Step 2 (table 12)
	PORTC = 0x20;	// function set
	lcd_strobe_lcd_e3b();

	// Step 3 (table 12)
	PORTC = 0x20;   // function set
	lcd_strobe_lcd_e3b();
	PORTC = 0x80;
	lcd_strobe_lcd_e3b();

	// Step 4 (table 12)
	PORTC = 0x00;   // Display on/off control
	lcd_strobe_lcd_e3b();
	PORTC = 0xF0;
	lcd_strobe_lcd_e3b();

	// Step 5 (table 12)
	PORTC = 0x00;   // Entry mode set
	lcd_strobe_lcd_e3b();
	PORTC = 0x60;
	lcd_strobe_lcd_e3b();
	
	clear3b();
	wait(500);

}



void week4Opdracht1()
{

			DDRF = 0x00;				// set PORTF for input (ADC)
			DDRA = 0xFF;				// set PORTA for output
			DDRB = 0xFF;				// set PORTB for output
			adcInit2();					// initialize ADC

			while (1)
			{
				PORTB = ADCL;			// Show MSB/LSB (bit 10:0) of ADC
				PORTA = ADCH;
				wait(100);				// every 100 ms (busy waiting)
			}
	
}


void week4Opdracht2()
{
			DDRF = 0x00;					// set PORTF for input (ADC)
			DDRA = 0xFF;					// set PORTA for output
			adcInit3();						// initialize ADC
			while (1)
			{
				ADCSRA |= BIT(6);				// Start ADC
				while ( ADCSRA & BIT(6) ) ;		// Wait for completion
				PORTA = ADCH;					// Show MSB (bit 9:2) of ADC
				wait(500);						// every 50 ms (busy waiting)
			}
}
void lcd_write_data3b(unsigned char byte)
/*
short:			Writes 8 bits DATA to lcd
inputs:			byte - written to LCD
outputs:
notes:			According datasheet HD44780 table 12
Version :    	DMK, Initial code
*******************************************************************/
{
	// First nibble.
	PORTC = byte;
	PORTC |= (1<<LCD_RS);
	lcd_strobe_lcd_e3b();

	// Second nibble
	PORTC = (byte<<4);
	PORTC |= (1<<LCD_RS);
	lcd_strobe_lcd_e3b();
}

void adcInit3b( void )
{
	ADMUX = 0b11100001;			// AREF=VCC, result left adjusted, channel1 at pin PF1
	ADCSRA = 0b11100110;		// ADC-enable, no interrupt, start, free running, division by 64
}

void week4Opdracht3()
{
	DDRF = 0x00;				// set PORTF for input 
	DDRA = 0xFF;				// set PORTA for output
	DDRB = 0xFF;				// set PORTB for output
	adcInit3b();				// initialize 
	init3b();

	while (1)
	{
		clear3b();
		
		wait(100);
		sprintf(value, "%d", ADCH);
		
		lcd_write_string(value);
		memset(value, 0, 10);
		wait(1000);				
	}

}