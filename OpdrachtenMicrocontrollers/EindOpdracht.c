/*
 * EindOpdracht.c
 *
 * Created: 13/03/2019 10:56:14
 *  Author: Timo
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "wait.h"

void twi_init(void);
void twi_start(void);
void twi_stop(void);
void twi_tx(unsigned char data);

void EindOpdrachtMain( void )
{
		twi_init();		// Init TWI interface

		// Init HT16K22. Page 32 datasheet
		twi_start();
		twi_tx(0xE0);	// Display I2C addres + R/W bit
		twi_tx(0x21);	// Internal osc on (page 10 HT16K33)
		twi_stop();

		twi_start();
		twi_tx(0xE0);	// Display I2C address + R/W bit
		twi_tx(0xA0);	// HT16K33 pins all output
		twi_stop();

		twi_start();
		twi_tx(0xE0);	// Display I2C address + R/W bit
		twi_tx(0xE3);	// Display Dimming 4/16 duty cycle
		twi_stop();

		twi_start();
		twi_tx(0xE0);	// Display I2C address + R/W bit
		twi_tx(0x81);	// Display OFF - Blink On
		twi_stop();

		while (1)
		{
			twi_start();
			twi_tx(0xE0);	// Display I2C addres + R/W bit
			twi_tx(0x00);	// Address
			twi_tx(0x00);	// data
			twi_stop();

			wait(500);

			twi_start();
			twi_tx(0xE0);	// Display I2C addres + R/W bit
			twi_tx(0x00);	// Address
			twi_tx(0x01);	// data
			twi_stop();

			wait(500);
		}
}

void twi_init(void)
{
	TWSR = 0;
	TWBR = 32;	 // TWI clock set to 100kHz, prescaler = 0
}


void twi_start(void)
{
	TWCR = (0x80 | 0x20 | 0x04);
	while( 0x00 == (TWCR & 0x80) );
}


void twi_stop(void)
{
	TWCR = (0x80 | 0x10 | 0x04);
}


void twi_tx(unsigned char data)
{
	TWDR = data;
	TWCR = (0x80 | 0x04);
	while( 0 == (TWCR & 0x80) );
}



