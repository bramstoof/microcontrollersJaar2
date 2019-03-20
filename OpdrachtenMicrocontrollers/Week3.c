#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LCD_E 	3
#define LCD_RS	2
#define LED PD4

void wait();
void lcd_strobe_lcd_e(void);
void init_4bits_mode(void);
void lcd_write_string(char *str);
void lcd_write_data(unsigned char byte);
void lcd_write_command(unsigned char byte);

void lcd_strobe_lcd_e(void)
{
	PORTC |= (1<<LCD_E);	// E high
	_delay_ms(1);			// nodig
	PORTC &= ~(1<<LCD_E);  	// E low
	_delay_ms(1);			// nodig?
}

void init_4bits_mode(void)
{
	// PORTC output mode and all low (also E and RS pin)
	DDRC = 0xFF;
	PORTC = 0x00;

	// Step 2 (table 12)
	PORTC = 0x20;	// function set
	lcd_strobe_lcd_e();

	// Step 3 (table 12)
	PORTC = 0x20;   // function set
	lcd_strobe_lcd_e();
	PORTC = 0x80;
	lcd_strobe_lcd_e();

	// Step 4 (table 12)
	PORTC = 0x00;   // Display on/off control
	lcd_strobe_lcd_e();
	PORTC = 0xF0;
	lcd_strobe_lcd_e();

	// Step 4 (table 12)
	PORTC = 0x00;   // Entry mode set
	lcd_strobe_lcd_e();
	PORTC = 0x60;
	lcd_strobe_lcd_e();

}

void lcd_write_string(char *str)
{
	// Het kan met een while:

	// while(*str) {
	// 	lcd_write_data(*str++);
	// }

	// of met een for:
	for(;*str; str++){
		lcd_write_data(*str);
	}
}

void lcd_write_data(unsigned char byte)
{
	// First nibble.
	PORTC = byte;
	PORTC |= (1<<LCD_RS);
	lcd_strobe_lcd_e();

	// Second nibble
	PORTC = (byte<<4);
	PORTC |= (1<<LCD_RS);
	lcd_strobe_lcd_e();
}

void lcd_write_command(unsigned char byte)
{
	// First nibble.
	PORTC = byte;
	PORTC &= ~(1<<LCD_RS);
	lcd_strobe_lcd_e();

	// Second nibble
	PORTC = (byte<<4);
	PORTC &= ~(1<<LCD_RS);
	lcd_strobe_lcd_e();
}

void lcd_init()
{
	lcd_write_command(0x30);      // Configure the LCD in 8-bit mode, 1 line and 5x7 font
	lcd_write_command(0x0C);      // Display On and Cursor Off
	lcd_write_command(0x01);      // Clear display screen
	lcd_write_command(0x06);      // Increment cursor
	lcd_write_command(0x80);      // Set cursor position to 1st line, 1st column
}



void week3Opdracht2()
{
	DDRD = 0xFF;
		int pressed = 0;
	init_4bits_mode();
	lcd_write_command(1);
	lcd_write_command(12);
	//lcd_init();
	int counter = 0;
	lcd_write_string("press C0");
	wait(500);
	//lcd_write_command(1);
	while (1)
	{
		if (PINB & 0x80)
		{
			if (!pressed)
			{
				pressed = 1;				// write 1 to all the bits of PortD
				counter++;
				lcd_write_command(1);
				lcd_write_command(2);
				char text[6];
				itoa(counter, text, 10);
				lcd_write_string(text);
			}
		}else
		{
			if (pressed)
			pressed = 0;
		}
	}
}

void week3Opdracht3()
{
	DDRD |= (1<<LED) ;
	TCCR1B = (1<<CS10) | (1<<CS12);
	OCR1A = 2344;
	TCNT1 = 0;
	int mode = 0;
	while(1)
	{
		while((TIFR & (1<<OCF1A)) == 0);
		if(mode ==0){			// on
			mode = 1;
			OCR1A = 234;		//15 ms
			
		}
		else if(mode  == 1){	// off
			mode = 0;
			OCR1A = 390;		//25 ms
		}
		PORTD ^= (1<< LED);
		TCNT1 = 0;
		TIFR |= (1<<OCF1A) ;
	}
}