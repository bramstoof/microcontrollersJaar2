
#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>

unsigned int waitingTime;
unsigned int state;

enum opdr7states {state_high, state_low, tri_state};

typedef struct {
	unsigned char data;
	unsigned int delay ;
} PATTERN_STRUCT;

PATTERN_STRUCT pattern[] = {
	{0x00, 100}, {0x01, 100}, {0x02, 100}, {0x04, 100}, {0x10, 100}, {0x20, 100}, {0x40, 100}, {0x80, 100},
	{0x00, 100},
	{0xAA,  50}, {0x55,  50},
	{0xAA,  50}, {0x55,  50},
	{0xAA,  50}, {0x55,  50},
	{0x00, 100},
	{0x81, 100}, {0x42, 100}, {0x24, 100}, {0x18, 100}, {0x0F, 200}, {0xF0, 200}, {0x0F, 200}, {0xF0, 200},
	{0x00, 0x00}
};

void wait();
void Week1OpdrachtB2();
void Week1OpdrachtB3();
void Week1OpdrachtB4();
void Week1OpdrachtB5();
void Week1OpdrachtB6();
void Week1OpdrachtB7();
void setCharliePlexingLed();

void Week1OpdrachtB2()
{
	while (1)
	{
		PORTD = 0x40;			// the hexadecimals for port D7
		wait( 500 );
		PORTD = 0x80;			// the hexadecimals for port D6
		wait( 500 );
	}
}

void Week1OpdrachtB3()
{
	PORTC = 0x10;
	_Bool LedState = 1;
	_Bool buttonPressed = 0;
	while (1)
	{
		if (PINC & 0x01)
		{
			if (!buttonPressed)
			buttonPressed = 1;				// write 1 to all the bits of PortD
		}
		else
		{
			if (buttonPressed)
			buttonPressed = 0;				// write 0 to all the bits of PortD
		}
		
		
		if (buttonPressed)
		{
			if (LedState)
			{
				PORTD = 0x01;
				LedState = 0;
			}
			else{
				PORTD = 0x00;
				LedState = 1;
			}
			wait(500);
		}
		else{
			PORTD = 0x00;
		}
	}
}

	void Week1OpdrachtB4(){
		
		unsigned int shiftingLights = 0x01;
		PORTD = shiftingLights; // Write 10101010b PORTD
		
		static int goingup = 1;
		
		while (1)
		{
			while(goingup == 1)
			{
				PORTD = shiftingLights;
				shiftingLights <<= 1;
				wait( 50 );
				
				if(shiftingLights > 0x81)
				{
					goingup = 0;
				}
			}
			while(goingup == 0)
			{
				PORTD = shiftingLights;
				shiftingLights >>= 1;
				wait( 50 );
				
				if(shiftingLights < 0x02)
				{
					goingup = 1;
				}
			}
		}
		
	}


	void Week1OpdrachtB5()
	{
		while(1)
		{
			int index = 0;
			// as long as delay has meaningful content
			while( pattern[index].delay != 0 )
			{
				// Write data to PORTD
				PORTD = pattern[index].data;
				// wait
				wait(pattern[index].delay);
				// increment for next round
				index++;
			}
		}
		
	}

	void Week1OpdrachtB6()
	{
		int state = 1;
		while (1)
		{
			if(PINC & (1<< PINC0))//if(PIND & 0x01)
			{
				switch(state)
				{
					case 1:
					state = 25;
					break;
					case 25:
					state = 1;
					break;
				}
				//PORTD = 0xFF;
				wait(100);
			}
			waitingTime = (100 / state);
			PORTD = 0x01;
			wait(waitingTime);
			PORTD = 0x00;
			wait(waitingTime);
		}
	}


	void Week1OpdrachtB7()
	{
		while (1)
		{
			for ( int i = 0; i < 6; i++ )
			{
				// Turn on Led i
				setCharliePlexingLed( i );
				wait(1000);
			}
		}
		
		
	}
	
	
	void setCharliePlexingLed(int lednr)
	{
		
		PORTD &= 0xF8;	// set the lower 3 bits to 0, ie off
		DDRD  |= 0x07;	// set the lower 3 bits to 1, ie output
		
		switch ( lednr )
		{
			case 0:
			DDRD  &= 0xFB;  // Disconnect the nr. 2 pin
			PORTD |= 0x01;  // Set pin 0 to high and pin 1 to low
			break;
			
			case 1:
			DDRD  &= 0xFB;  // Disconnect the nr. 2 pin
			PORTD |= 0x02;  // Set pin 1 to high and pin 0 to low
			break;
			
			case 2:
			DDRD  &= 0xFE;  // Disconnect the nr. 0 pin
			PORTD |= 0x02;  // Set pin 1 to high and pin 2 to low
			break;
			
			case 3:
			DDRD  &= 0xFE;  // Disconnect the nr. 0 pin
			PORTD |= 0x04;  // Set pin 2 to high and pin 1 to low
			break;
			
			case 4:
			DDRD  &= 0xFD;  // Disconnect the nr. 1 pin
			PORTD |= 0x01;  // Set pin 0 to high and pin 2 to low
			break;
			
			case 5:
			DDRD  &= 0xFD;  // Disconnect the nr. 1 pin
			PORTD |= 0x04;  // Set pin 2 to high and pin 0 to low
			break;
		}
	}
	



