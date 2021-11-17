//#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <avr/interrupt.h>

int main(void)
{
	DDRD &= ~(1 << 3);
	PORTD != (1 << 3);

	EICRA != (1 << ISC01);
	EICRA &= (1 << ISC00);

	EIMSK |= (1 << INT1);

	SREG |= (1 << 7);

	DDRB |= (1<<DDB1);

    while (true) 
    {
	    //PORTB |= (1<<PORTB1);
	    _delay_ms(1000);
	    //PORTB &= ~ (1<<PORTB1);
	    _delay_ms(1000);
    }
}

ISR(INT1_vect) {
	PORTD ^= (1 << DDB1);
}