//-----------------------------------------------------------------------------
//	ATtiny26L
//	Internal 8 MHz RC Osc.
//	(1/8M = 125 ns)
//-----------------------------------------------------------------------------

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/atomic.h>
#include "speedo.h"

#define INIT_VAL    0    // 10ms (128usX80=10.24ms)
#define CTC_MATCH_OVERFLOW ((F_CPU/1000) / 8) // overflow every millisecond

volatile unsigned char flag = 0;

volatile unsigned long speed = 0;

volatile unsigned long timer1_millis = 0;
volatile unsigned long last_interval = 0;
volatile unsigned long last_pulse = 0;
//volatile unsigned long counter = 0;
//volatile unsigned long interval = 2;

unsigned int pulses = 0;

float tire_circumference = 85.113;
float inches_per_pulse = 8.6263;
float miles_in_inches = 63360;
float k_ipm_2_mph;



//-----------------------------------------------------------------------------
//  Timer/Counter1 Initialization
//-----------------------------------------------------------------------------
void init_timer1()
{
    TCCR1B = 0x00;       // Stop Timer/Counter1
    TCCR1B |= _BV(CTC1) | _BV(CS12); // CTC: clear timer on compare; CS12: clock/8
    OCR1C = (CTC_MATCH_OVERFLOW);
    TIMSK |= _BV(OCIE1A); // compare match interrupt enable
}

void init_ext()
{
    GIMSK = 0x00;
    PORTB |= _BV(PB6);
    MCUCR =  _BV(ISC01) | _BV(ISC00);
    GIMSK |= _BV(INT0);
}

ISR (TIMER1_CMPA_vect)
{
	timer1_millis++;
}

unsigned long millis()
{
	unsigned long millis_return;
	ATOMIC_BLOCK(ATOMIC_FORCEON) {
		millis_return = timer1_millis;
	}
	return millis_return;
}

//-----------------------------------------------------------------------------
//  Timer/Counter0 Overflow Handler
//-----------------------------------------------------------------------------
/*ISR(TIMER1_OVF1_vect)
{
    //262.144ms have passed
    count_interval++;
    TCNT1 = INIT_VAL;  // Clear Time/Counter0
}
*/
ISR(INT0_vect)
{
/*    if (flag) PORTB |= _BV(PB5);
    else PORTB &= ~_BV(PB5);

    flag = ~flag;
*/
	pulses++;
	last_interval = millis() - last_pulse;
   	last_pulse = millis();
}

void calculate_speed()
{
	float inches_per_millisecond = (1/inches_per_pulse) * (float)last_interval; // gets ms per inch
	last_interval = 1/last_interval; // convert to in per ms
	speed = inches_per_millisecond * k_ipm_2_mph;

}

void displayspeed(unsigned long displayed_speed) 
{
    PORTA = (0b11110000 & ((displayed_speed/10)%10 << 4)) | (0b00001111 & (displayed_speed % 10));
/*    if (displayed_speed > 200) {
        PORTB &= ~_BV(PB4);
        PORTB |= _BV(PB5);
    } else 

if (displayed_speed > 100) {
        PORTB &= ~_BV(PB5);
        PORTB |= _BV(PB4);
    } else {
        PORTB &= ~( _BV(PB4));

    }
*/
}

void setup()
{
	k_ipm_2_mph = 3600000 / miles_in_inches;
}


//-----------------------------------------------------------------------------
//	Main
//-----------------------------------------------------------------------------
int main()
{
    DDRB  |= _BV(PB4) | _BV(PB5); // turn on high digit
//    PORTB |= _BV(PB4);
    DDRA = 0b11111111;
        displayspeed(42);

	setup();

    init_ext();
    init_timer1();

    sei();

    while (1) {
//	counter++;
//	if (counter > interval) {
//	calculate_speed();
		speed = 450/last_interval;
        	displayspeed(speed);
//		counter = 0;
//	}
    }

    return 0;
}
