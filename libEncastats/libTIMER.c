
#include "libTIMER.h"
#include <p30F4011.h>


void Timer1Init(unsigned int period, unsigned char pscaler, unsigned char intPriority){
    
	// Configure Timer 1

	// T1 control register = 0x0000
	// TON = 0		stop timer
	// TSIDL = 0	continue operation in idle mode
	// TGATE = 0	disable gated time accumulation mode
	// TCKPS = 00	Timer Input Clock Prescale = 00
	// TSYNC = 0	Do not synchronize external clock input
	// TCS = 0		Timer clock source = internal clock = FCY = 1/TCY = TOSC/4; TCY is the instruction cycle period
	T1CON = 0x0000;

	// T1 counter = 0x0000
    TMR1  = 0x0000;

	// T1 period = period
    PR1   = period;

	// T1 control register, TCKPS = 11
	// Set Timer Input Clock Prescale = 11 = 1:256
    T1CONbits.TCKPS = pscaler;	//0b11;

	
	// Configure T1 interrupt

    // Enable T1 interrupt
    IEC0bits.T1IE = 1;

	// Clear T1 interrupt flag	
    IFS0bits.T1IF = 0;

	// T1 interrupt priority = 4
	// Priorities 8-15 are reserved for traps
	// Priorities 0-7 can be used for external/peripheral devices
    IPC0bits.T1IP = intPriority;
}

void Timer1Start(){
    T1CONbits.TON = 1; // Enable timer
}

void Timer1Stop(){

    T1CONbits.TON = 0; // Stop timer
    TMR1 = 0;          // Reset timer counter
}



void Timer1ReInitialize(unsigned int period, unsigned char pscaler, unsigned char intPriority){

	Timer1Stop();
    Timer1Init(period, pscaler, intPriority);
	Timer1Start();
}



void TimerClearInt()
{
	IFS0bits.T1IF = 0;
}