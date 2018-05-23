

#include <p30f4011.h>
#include "libLEDs.h"


// El registro TRIS por cada puerto sirve para configurar si es I/O
#define LED0_TRIS	TRISEbits.TRISE0
#define LED1_TRIS	TRISEbits.TRISE1
#define LED2_TRIS	TRISEbits.TRISE2
#define LED3_TRIS	TRISEbits.TRISE3
#define LED4_TRIS	TRISEbits.TRISE4
#define LED5_TRIS	TRISEbits.TRISE5

// El registro LAT por cada puerto es para escribir un valor
#define LED0		LATEbits.LATE0
#define LED1		LATEbits.LATE1
#define LED2		LATEbits.LATE2
#define LED3		LATEbits.LATE3
#define LED4		LATEbits.LATE4
#define LED5		LATEbits.LATE5

// El registro PORT por cada puerto es para leer un valor


// LEDs states
unsigned char ledsState = 0;


void initLEDs()
{
	LED0_TRIS = 0;	// Configuramos el pin 0 del puerto E (LEDs) como salida
	LED1_TRIS = 0;
	LED2_TRIS = 0;
	LED3_TRIS = 0;
	LED4_TRIS = 0;
	LED5_TRIS = 0;
}


void onLED(unsigned char numLED)
{
	switch (numLED)
	{
		case 0 : LED0 = 1; ledsState = ledsState | 0b00000001; break;
		case 1 : LED1 = 1; ledsState = ledsState | 0b00000010; break;
		case 2 : LED2 = 1; ledsState = ledsState | 0b00000100; break;
		case 3 : LED3 = 1; ledsState = ledsState | 0b00001000; break;
		case 4 : LED4 = 1; ledsState = ledsState | 0b00010000; break;
		case 5 : LED5 = 1; ledsState = ledsState | 0b00100000; break;
		default:;
	}
}


void offLED(unsigned char numLED)
{
	switch (numLED)
	{
		case 0 : LED0 = 0; ledsState = ledsState & 0b11111110; break;
		case 1 : LED1 = 0; ledsState = ledsState & 0b11111101; break;
		case 2 : LED2 = 0; ledsState = ledsState & 0b11111011; break;
		case 3 : LED3 = 0; ledsState = ledsState & 0b11110111; break;
		case 4 : LED4 = 0; ledsState = ledsState & 0b11101111; break;
		case 5 : LED5 = 0; ledsState = ledsState & 0b11011111; break;
		default:;
	}
}


void toggleLED(unsigned char numLED)
{
	unsigned char maskLed;

	if (numLED > 5) return;

	maskLed = 0b00000001 << numLED;

	if ( (maskLed & ledsState) ) // Led is switched on
	{
		offLED(numLED);
		ledsState = ledsState & (~maskLed);
	}
	else
	{
		onLED(numLED);
		ledsState = ledsState | maskLed;
	}

}


void printNumInLED(unsigned char number)
{
	unsigned char maskLed = 0x01;
	unsigned char led;

	maskLed=0x01;
	for (led=0; led <= 5; led++)
	{
		if (number & maskLed)
		{
			onLED(led);
		}
		else
		{
			offLED(led);
		}
		maskLed = maskLed << 1;
	}
}