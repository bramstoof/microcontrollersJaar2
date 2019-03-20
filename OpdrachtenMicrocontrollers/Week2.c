/*
 * Week2.c
 *
 * Created: 13/02/2019 15:10:10
 *  Author: Timo
 */ 
#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void wait();


void desplay(int value){
	const unsigned char Numbers[16] = {
		0b00111111,//0
		0b00000110,//1
		0b01011011,//2
		0b01001111,//3
		0b01100110,//4
		0b01101101,//5
		0b01111101,//6
		0b00000111,//7
		0b01111111,//8
		0b01101111,//9
		0b01110111,//a
		0b01111100,//b
		0b00111001,//c
		0b01011110,//d
		0b01111001,//e
		0b01110001 //f
	};
	if (0 <= value && 15 >= value)
	{
		PORTD = Numbers[value];
	}
	else
	{
		PORTD = 0b01111001;	
	}
	
}

void countUp(int value){
	desplay(++value);
}

void countDown(int value){
	desplay(--value);
}

ISR( INT0_vect )
{
    PORTD |= (1<<5);		
}

ISR( INT1_vect )
{
    PORTD &= ~(1<<5);		
}

void week2Opdracht3(){
	DDRD = 0b11111111;
	_Bool pressed = 0;
	_Bool pressed2 = 0;
	int value = 1;
	desplay(value);
	
	
	
	while (1){
		
		if (PINC & 0x80){
			if (!pressed){
				pressed = 1;				// write 1 to all the bits of PortD
				countUp(value++);
			}
		}
		else{
			if (pressed)
			pressed = 0;				// write 0 to all the bits of PortD
		}
		
		if (PINC & 0x40){
			if (!pressed2){
				pressed2 = 1;				// write 1 to all the bits of PortD
				countDown(value--);
			}
		}
		else{
			if (pressed2)
			pressed2 = 0;				// write 0 to all the bits of PortD
		}
		if(pressed == 1 && pressed2 == 1){
			value = 0;
			desplay(0);
		}
			
	}
}

void week2Opdracht4(){
	DDRD = 0b11111111;
	int count =0;
	const unsigned char pathern[14] = {
		0b00100000,
		0b00100001,
		0b00100011,
		0b01100011,
		0b01110011,
		0b01111011,
		0b01111111,
		0b01011111,
		0b01011110,
		0b01011100,
		0b00011100,
		0b00001100,
		0b00000100,
		0b00000000,
	};
	while (1)
	{
		PORTD = pathern[count];
		
		wait(50);
		
		if (++count == 14)
		{
			count = 0;
		}
	}
}

