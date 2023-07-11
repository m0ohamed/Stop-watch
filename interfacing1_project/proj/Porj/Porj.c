#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
//control variables shared between ISRs and Background
unsigned char izero=0, sec1=0,sec2=0,min1=0,min2=0,hour1=0,hour2=0;

//timer 1 init
void Timer1_Init()
{
	TCNT1=0;
	TCCR1B = (1<<FOC1A) |(1<<WGM13) | (1<<WGM12) |(1<<CS10) |(1<<CS12);
	ICR1=1000;
	TIMSK=(1<<OCIE1A);

}
//interrupt 0 init
void INT0_Init()
{
	MCUCR &=~(1<<ISC00);
	MCUCR |=(1<<ISC01);
	GICR |=(1<<6);

}

//interrupt 1 init
void INT1_Init()
{
	MCUCR |=(1<<ISC10) | (1<<ISC11);
	GICR |=(1<<7);

}

//interrupt 2 init
void INT2_Init()
{
	MCUCSR &=~(1<<ISC2);
	GICR |=(1<<5);

}

//ISR for timer 1 when compare value detected
ISR(TIMER1_COMPA_vect)
{
//1,,,,,,9 seconds first 7-segment
	sec1++;
	//1,,,,,,,,,9 seconds second 7-segment
	if(sec1==10)
	{
		sec2++;
		sec1=0;
	}
	//1,,,,,,,,,9 minutes third 7-segment
	if(sec2==6)
	{
		min1++;
		sec2=0;
	}

	//1,,,,,,,,,9 minutes fourth 7-segment

	if(min1==10)
	{
		min2++;
		min1=0;
	}

	//1,,,,,,,,,9 hours fifth 7-segment

	if(min2==6)
	{
		hour1++;
		min2=0;
	}

	//1,,,,,,,,,9 hours sixth 7-segment

	if(hour1==10)
	{
		hour2++;
		hour1=0;
	}

}

//ISR interrupt 0 for reset the stop watch
ISR(INT0_vect)
{
	TCNT1=0;
	sec1=0;sec2=0;min1=0;min2=0;hour1=0;hour2=0;

}
//ISR interrupt 1 to pause the stop watch

ISR(INT1_vect)
{
	TCCR1B &=~(1<<CS12) & (~(1<<CS11)) & (~(1<<CS10));
	TCCR1A =0;
}

//ISR interrupt 2 to resume the stop watch
ISR(INT2_vect)
{

	TCCR1B |=(1<<CS12) | (1<<CS10) ;
	TCCR1A |=(1<<FOC1A);


}

//function to call init timer1 ,INT0,INT1,INT2
void Init()
{
	Timer1_Init();
	INT0_Init();
	INT1_Init();
	INT2_Init();

}
int main()
{
	Init();
	//setting pins
	DDRC |=0x0F;
	PORTC &=0xF0 ;
	DDRB &=~(1<<2);
	PORTB |=(1<<2);
	DDRD &=~(1<<PD3);
	DDRD &=~(1<<2);
	PORTD |=(1<<2);
	DDRA |=0x3F;
	//enable global interrupt
	SREG |=(1<<7);


while(1)	{

//writ to the multiplexed 7-segment
		PORTA=0x01;
		PORTC = sec1;

		_delay_ms(1);
		PORTA=0x02;
		PORTC = sec2;

		_delay_ms(1);
		PORTA=0x04;
		PORTC = min1;

		_delay_ms(1);
		PORTA=0x08;
		PORTC = min2;

		_delay_ms(1);
		PORTA=0x10;
		PORTC = hour1;

		_delay_ms(1);
		PORTA=0x20;
		PORTC = hour2;

		_delay_ms(1);}



		}

