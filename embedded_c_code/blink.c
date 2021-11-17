#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

int main(void)
{
	DDRB |= (1<<DDB1);
    while (true) 
    {
	    PORTB |= (1<<PORTB1);
	    _delay_ms(1000);
	    PORTB &= ~ (1<<PORTB1);
	    _delay_ms(1000);
    }
}