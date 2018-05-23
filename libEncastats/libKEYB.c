#include "libKEYB.h"

#include <p30f4011.h>

#define COL0_TRIS TRISBbits.TRISB0
#define COL0      LATBbits.LATB0 
#define COL1_TRIS TRISBbits.TRISB1
#define COL1      LATBbits.LATB1
#define COL2_TRIS TRISBbits.TRISB2
#define COL2      LATBbits.LATB2

#define ROW0_TRIS TRISDbits.TRISD0
#define ROW0      PORTDbits.RD0
#define ROW1_TRIS TRISDbits.TRISD1
#define ROW1      PORTDbits.RD1
#define ROW2_TRIS TRISDbits.TRISD2
#define ROW2      PORTDbits.RD2
#define ROW3_TRIS TRISDbits.TRISD3
#define ROW3      PORTDbits.RD3

void KeybInit(){
	// Set columns as outputs
	COL0_TRIS = 0;
	COL1_TRIS = 0;
	COL2_TRIS = 0;
	
	// Set rows as inputs
	ROW0_TRIS = 1;
	ROW1_TRIS = 1;
	ROW2_TRIS = 1;
	ROW3_TRIS = 1;
}

unsigned char getKey(){
	// Set rows as inputs
	ROW0_TRIS = 1;
	ROW1_TRIS = 1;
	ROW2_TRIS = 1;
	ROW3_TRIS = 1;

	while(1){
		// Inspect COL 0
		COL0_TRIS = 0;
		COL1_TRIS = 1;
		COL2_TRIS = 1;
		COL0 = 0;
		COL1 = 1;
		COL2 = 1;

		Delay10us();
		
		if(ROW0 == 0) return 0;
		if(ROW1 == 0) return 3;
		if(ROW2 == 0) return 6;
		if(ROW3 == 0) return 9;
		
		// Inspect COL 1
		COL0_TRIS = 1;
		COL1_TRIS = 0;
		COL2_TRIS = 1;
		COL0 = 1;
		COL1 = 0;
		COL2 = 1;

		Delay10us();
		
		if(ROW0 == 0) return  1;
		if(ROW1 == 0) return  4;
		if(ROW2 == 0) return  7;
		if(ROW3 == 0) return 10;
		
		// Inspect COL 2
		COL0_TRIS = 1;
		COL1_TRIS = 1;
		COL2_TRIS = 0;
		COL0 = 1;
		COL1 = 1;
		COL2 = 0;

		Delay10us();
		
		if(ROW0 == 0) return  2;
		if(ROW1 == 0) return  5;
		if(ROW2 == 0) return  8;
		if(ROW3 == 0) return 11;
	}

	COL0_TRIS = 1;
	COL1_TRIS = 1;
	COL2_TRIS = 1;
}


char getKeyNotBlocking(){
	// Set rows as inputs
	ROW0_TRIS = 1;
	ROW1_TRIS = 1;
	ROW2_TRIS = 1;
	ROW3_TRIS = 1;

	// Inspect COL 0
	COL0_TRIS = 0;
	COL1_TRIS = 1;
	COL2_TRIS = 1;
	COL0 = 0;
	COL1 = 1;
	COL2 = 1;

	Delay10us();
	
	if(ROW0 == 0) return 0;
	if(ROW1 == 0) return 3;
	if(ROW2 == 0) return 6;
	if(ROW3 == 0) return 9;
	
	// Inspect COL 1
	COL0_TRIS = 1;
	COL1_TRIS = 0;
	COL2_TRIS = 1;
	COL0 = 1;
	COL1 = 0;
	COL2 = 1;

	Delay10us();
	
	if(ROW0 == 0) return  1;
	if(ROW1 == 0) return  4;
	if(ROW2 == 0) return  7;
	if(ROW3 == 0) return 10;
	
	// Inspect COL 2
	COL0_TRIS = 1;
	COL1_TRIS = 1;
	COL2_TRIS = 0;
	COL0 = 1;
	COL1 = 1;
	COL2 = 0;

	Delay10us();
	
	if(ROW0 == 0) return  2;
	if(ROW1 == 0) return  5;
	if(ROW2 == 0) return  8;
	if(ROW3 == 0) return 11;

	COL0_TRIS = 1;
	COL1_TRIS = 1;
	COL2_TRIS = 1;

	return NO_BUTTON_PRESSED;
}