/*
 ============================================================================
 Name        : Ali_Mamdouh_Mini_Project_2.c
 Author      : Ali Mamdouh
 Description : Stop Watch System Project
 Date        : 19/08/2022
 ============================================================================
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
unsigned char i, sec = 0, min = 0, hour = 0;


ISR(INT0_vect)
{
	sec = 0, min = 0, hour = 0;
}

ISR(INT2_vect)
{
	TIMSK |= (1<<OCIE1A); //Enable timer1 interrupt
}


ISR(INT1_vect)
{
	TIMSK &= ~(1<<OCIE1A); //Disable timer1 interrupt
}


void interrupt(void)
{
	MCUCR |= (1<<ISC01) | (1<<ISC11) | (1<<ISC10);
	MCUCR &= ~(1<<ISC00);
	MCUCSR &= ~(1<<ISC2); /*ISC11, ISC10 are 1 for rising edge for INT1
		                   *ISC01 is 1 and ISC00 is 0 for falling edge for INT0
		                   *ISC2 is 0 for falling edge foe INT2
		                   */
	GICR |= (1<<INT0) | (1<<INT1) | (1<<INT2); //enable interrupts 0,1,2 (this line must be written at the end of this function to don`t crash timer1 interrupt)
}


ISR(TIMER1_COMPA_vect)
{
	sec++;
	if(sec == 60)
	{
		sec = 0;
		min++;
	}

	if(min == 60)
	{
		min = 0;
		hour++;
	}
}


void Timer1(void)
{
	TCNT0 = 1;
	OCR1A = 977; // overflow occurs every (977 * 1.024*10^-3) = 1000.488ms (timer1 can reach 2^16, TIMER0 2^8)

	TCCR1A = (1<<FOC1A) | (1<<FOC1B);
	TCCR1B = (1<<CS12) | (1<<CS10) | (1<<WGM12);
	/*WGM10, WGM11, WGM13 are zero for compare mode
     *FOC1A , FOC1B are 1 to cancel PWM mode
     *COM1A1, COM1A0, COM1B1, COM1B1 are 0 for normal mode
     *CS10, CS12 are 1 and CS11 is 0 for scale 1024 -> frequency of timer1 = (10^6 / 1024) {timer1 increment every 1/(10^6 / 1024) = 1.024ms}
     */
	TIMSK |= (1<<OCIE1A); //Output Compare A Match Interrupt Enable to request interrupt every overflow(1000.488ms)
	SREG |= (1<<7); //Global Interrupt Enable to request interrupt every overflow(1000.488ms)

}






int main(void)
{
	DDRA |= 0X3F;
	PORTA &= 0XC0;

	DDRC |= 0X0F;
	PORTC &= 0XF0;

	DDRB &= ~(1<<PB2);
	PORTB |= (1<<PB2); /*internal pull-up resistor (internal source series with resistor connected to PINB2)
                       *PINB2 will always be high(due to internal source) so when button is pressed to connect pinb2 with ground FALLING EDGE OCCURS
                       */

	DDRD &= ~(1<<PD2) & ~(1<<PD3);
	PORTD |= (1<<PD2); /*internal pull-up resistor (internal source series with resistor connected to PIND2)
	                    *PIND2 will always be high(due to internal source) so when button is pressed to connect pind2 with ground FALLING EDGE OCCURS
	                    */

	interrupt();
	Timer1();

	while(1)
	{
		for(i = 0; i < 6; i++)
		{
			switch (i)
			{
			case 0:
				PORTA = (PORTA & 0XC0) | (1<<i); //turn off the 6 pins except pin i (turn off the 6 pins then turn on pin i)
				PORTC = (PORTC & 0XF0) | (sec % 10); //set 0 to the four pins to be able to put sec in the four pins (%10 t0 take unit_digis)
				break;

			case 1:
				PORTA = (PORTA & 0XC0) | (1<<i);
			    PORTC = (PORTC & 0XF0) | (sec / 10); /*set 0 to the four pins to be able to put sec in the four pins (%10 t0 take tenth_digis)
			                                          *(sec must be integer to apply this algorithm)*/
				break;

			case 2:
				PORTA = (PORTA & 0XC0) | (1<<i);
				PORTC = (PORTC & 0XF0) | (min % 10);
				break;

			case 3:
				PORTA = (PORTA & 0XC0) | (1<<i);
				PORTC = (PORTC & 0XF0) | (min / 10);
				break;

			case 4:
				PORTA = (PORTA & 0XC0) | (1<<i);
				PORTC = (PORTC & 0XF0) | (hour % 10);
				break;

			case 5:
				PORTA = (PORTA & 0XC0) | (1<<i);
				PORTC = (PORTC & 0XF0) | (hour / 10);
				break;
			}


			_delay_ms(1); /*display every segment for 1ms while all other segments are off
			               *every segment will be on for 1ms and off for 7ms
			               *due to turning off for 7ms is very small portion of time..every segment appears that it is always on due to the persistence of vision
			               *if we delete this line there is no problem with persistence of vision, but 7segment crash due to very high frequency
			               */
		}

	}
	return 0;
}
