/*speedo.h */

#ifndef __speedo_h__
#define __speedo_h__

#define F_CPU 1000000UL
#define TRUE 1
#define FALSE 0

// I/O port definitions
#define OUT_TENS_PORT PORTC
#define OUT_TENS_DDR DDRC
#define OUT_HUNDREDS_PORT PORTA
#define OUT_HUNDREDS_DDR DDRA
#define OUT_BALLCOUNT_PORT PORTB
#define OUT_BALLCOUNT_DDR DDRB

#define IO_CONTROLS_PORT PORTD
#define IO_CONTROLS_DDR DDRD
#define IO_CONTROLS_DDR_VALUE //11110000
#define IO_CONTROLS_INPUT PIND
//NOTE: If we enable the USART receiver, this will be problematic.
//So don't ever set bit 4 of UCSRB
#define BITI_BALLRETURN PD0
//NOTE: If we enable the USART transmitter, this will be problematic.
//So don't ever set bit 3 of UCSRB
#define BITI_TENPOINTS PD1
//NOTE: PD2 is INT0.  Clearing it will generate an INT0
//if INT0 is enabled (Bit 6 of GICR)
//This means that if we want to release the ball, we have to disable INT0 
//first, and presumably enable it after we're done releasing the balls.
//This is also the XCK pin, which will be enabled if bit 6 of UCSRB is set.
//NOTE: PD3 is INT1.  Clearing it will generate an INT1
//if INT1 is enabled (Bit 5 of GICR)
//This means that if we want to  run the ticket dispenser, we have to disable
//INT1 first (and presumably enable it again after)

//FIXME: PD4 is XCK.  What are the effects of reading it?
#define BITO_BALL_RELEASE PD4
//FIXME: PD5 is OC1A.  What are the effects of reading it?
#define BITO_TICKET_DISPENSER PD5

#define BITO_LIGHT_GAMEOVER PD6
#define BITO_LIGHT_WINNER PD7

// I/O port settings. (all but D are set to 00001111)
// a '1' bit indicates an output pin
#define DDR_VALUE 0x0F
//IO port ddr values
#define IO_DDR_VALUE 0xF0 //simulator
#define IO_PULLUP 0x03 //simulator
//#define IO_PULLUP 0x0F //real
#define HUN_IN_MASK 0x80

//Interrupts
#define INT_COIN INT0_vect
#define INT_BUTTON INT1_vect

//game logic constants
#define DEBOUNCE_MS 10
#define POINT_DELAY_MS 100
#define BALL_RELEASE_MS 500
#define BALLS_PER_COIN 9
#define SCORE_PER_BALL 5
//#define LIMIT_SCORE

void sr_init();

#endif
