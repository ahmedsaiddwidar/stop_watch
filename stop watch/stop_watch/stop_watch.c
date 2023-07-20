/*
 * stop_watch.c
 *
 *  Created on: May 27, 2023
 *      Author: Ahmed Dwidar
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define arraySegment_size 6
char seg[arraySegment_size];

void settingSegment(void)
{
	if(seg[0] == 10)
	{
		seg[0] = 0;
		seg[1]++;
	}

	if(seg[1] == 6)
	{
		seg[0] = 0;
		seg[1] = 0;
		seg[2]++;
	}


	if(seg[2] == 10)
	{
		seg[0] = 0;
		seg[1] = 0;
		seg[2] = 0;
		seg[3]++;
	}

	if(seg[3] == 6)
	{
		seg[0] = 0;
		seg[1] = 0;
		seg[2] = 0;
		seg[3] = 0;
		seg[4]++;
	}

	if(seg[4] == 10)
	{
		seg[0] = 0;
		seg[1] = 0;
		seg[2] = 0;
		seg[3] = 0;
		seg[4] = 0;
		seg[5]++;
	}

	if(seg[5] == 10)
	{
		seg[0] = 0;
		seg[1] = 0;
		seg[2] = 0;
		seg[3] = 0;
		seg[4] = 0;
		seg[5] = 0;
	}

}
ISR(TIMER1_COMPA_vect)
{
	seg[0]++;
}

ISR(INT0_vect)
{
	TCNT1 = 0;
	seg[0] = 0;
	seg[1] = 0;
	seg[2] = 0;
	seg[3] = 0;
	seg[4] = 0;
	seg[5] = 0;
}
ISR(INT1_vect)
{
	TCCR1B &= ~(1<<WGM12) & ~(1<<CS10) & ~(1<<CS12);
}
ISR(INT2_vect)
{
	TCCR1B = (1<<WGM12) | (1<<CS10) |(1<<CS12);
}
void TIMER1_setting(void)
{
	TCNT1 = 0;

	OCR1A = 1000;

	TCCR1A = (1<<FOC1A);

	TCCR1B = (1<<WGM12) | (1<<CS10) |(1<<CS12);

	TIMSK |= (1<<OCIE1A);
}
void INT0_Init(void)
{
	DDRD &= ~(1<<2);
	PORTD |= (1<<2);

	GICR |= (1<<INT0);
	MCUCR |= (1<<ISC01);
	MCUCR &= ~(1<<ISC00);

}
void INT1_Init(void)
{
	DDRD &= ~(1<<3);

	GICR |= (1<<INT1);
	MCUCR |= (1<<ISC10);
	MCUCR |= (1<<ISC11);


}
void INT2_Init(void)
{
	DDRB &= ~(1<<3);
	PORTB |= (1<<3);
	GICR |= (1<<INT2);
	MCUCSR &= ~(1<<ISC2);
}
void displaySegment(void)
{
	int i;
	for(i = 0; i < arraySegment_size; i++)
	{
		PORTA = (1<<i);
		PORTC = seg[i];
		_delay_ms(2);
	}
}
int main()
{
	DDRC = 0x0F;  //4 pins in Portc are outputs
	PORTC = 0x00; // initial value of PC0 to show in decoder
	DDRA = 0x3F; // first 6 pins in PORTA are outputs
	PORTA = 0x01; // Show PA0 as initial pin in 7 segment
	SREG = (1<<7);


	INT0_Init();
	INT1_Init();
	INT2_Init();
	TIMER1_setting();
	while(1)
	{
		displaySegment();

		settingSegment();


	}

}
