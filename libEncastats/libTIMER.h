/*
 * Timer library
 *
 * Timer is fed with system clock which runs at 29.491.200 Hz (almost 30 MHz).
 * The timer clock can be slowed by means of the prescaler.
 * 
 */

//////////////////////////////////////////////////////////////////////////////////
// Calculo del valor del preescaler y del periodo para diferentes velocidades
//////////////////////////////////////////////////////////////////////////////////

#define TIMER_PSCALER_1_1   0b00
#define TIMER_PSCALER_1_8   0b01
#define TIMER_PSCALER_1_64  0b10
#define TIMER_PSCALER_1_256 0b11


// La frecuencia del ciclo de instrucción (FCY) es 29.491.200 Hz
// Queremos que el Timer cuente 500ms
//
// Con pre-escalador a 1:1
// FclkTimer = 29.491.200 Hz -> TclkTimer = 33,9084 ns
// TclkTimer * periodoTimer = 500 ms -> periodoTimer = 14.745.608,76
// Como el timer es de 16 bits, solo puede contar hasta 65535
// Por tanto, periodoTimer no cabría en el registro
// del timer que se utiliza para programar su periodo
//
// Configuración para contar 500 ms
//
//		Pre-escalador a 1:256
//		FclkTimer = 115.200 Hz -> TclkTimer = 8,6806 us = 0,0086806 ms
//		TclkTimer * periodoTimer = 500 ms -> periodoTimer = 57.600
//
#define TIMER_PSCALER_FOR_500ms		TIMER_PSCALER_1_256
#define TIMER_PERIOD_FOR_500ms		57600

// Configuración para contar 250 ms
//
//		Pre-escalador a 1:256
//		FclkTimer = 115.200 Hz -> TclkTimer = 8,6806 us
//		TclkTimer * periodoTimer = 250 ms -> periodoTimer = 28.800
//
#define TIMER_PSCALER_FOR_250ms		TIMER_PSCALER_1_256
#define TIMER_PERIOD_FOR_250ms		28800


// Configuración para contar 125 ms
//
//		Pre-escalador a 1:256
//		FclkTimer = 115.200 Hz -> TclkTimer = 8,6806 us
//		TclkTimer * periodoTimer = 150 ms -> periodoTimer = 14.400
//
#define TIMER_PSCALER_FOR_125ms		TIMER_PSCALER_1_256
#define TIMER_PERIOD_FOR_125ms		14400


// Configuración para contar 62,5 ms
//
//		Pre-escalador a 1:256
//		FclkTimer = 115.200 Hz -> TclkTimer = 8,6806 us
//		TclkTimer * periodoTimer = 62,5 ms -> periodoTimer = 14.400
//
#define TIMER_PSCALER_FOR_62_5ms	TIMER_PSCALER_1_256
#define TIMER_PERIOD_FOR_62_5ms		7200


// Configuración para contar 1,25 ms
//
//		Pre-escalador a 1:256
//		FclkTimer = 115.200 Hz -> TclkTimer = 8,6806 us
//		TclkTimer * periodoTimer = 1,25 -> periodoTimer = 144
//
#define TIMER_PSCALER_FOR_1_25ms	TIMER_PSCALER_1_256
#define TIMER_PERIOD_FOR_1_25ms		144


// Configuración para contar 10 ms
//
//		Pre-escalador a 1:256
//		FclkTimer = 115.200 Hz -> TclkTimer = 8,6806 us
//		TclkTimer * periodoTimer = 10 -> periodoTimer = 1152
//
#define TIMER_PSCALER_FOR_10ms	TIMER_PSCALER_1_256
#define TIMER_PERIOD_FOR_10ms	1152


// Configuración para contar 50 ms
//
//		Pre-escalador a 1:256
//		FclkTimer = 115.200 Hz -> TclkTimer = 8,6806 us
//		TclkTimer * periodoTimer = 50 -> periodoTimer = 5760
//
#define TIMER_PSCALER_FOR_50ms	TIMER_PSCALER_1_256
#define TIMER_PERIOD_FOR_50ms	5760




void Timer1Init(unsigned int period, unsigned char pscaler, unsigned char intPriority);

void Timer1Start();
void Timer1Stop();

void Timer1ReInitialize(unsigned int period, unsigned char pscaler, unsigned char intPriority);

void TimerClearInt();