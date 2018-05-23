#include "libLCD.h"
#include "delay.h"

#include <p30f4011.h>

/******************************************************************************/
/* Pre-processor directives                                                   */
/******************************************************************************/

#define LCD_EN_TRIS  TRISBbits.TRISB6
#define LCD_EN       LATBbits.LATB6

#define LCD_RS_TRIS  TRISCbits.TRISC15
#define LCD_RS       LATCbits.LATC15
#define LCD_RW_TRIS  TRISEbits.TRISE8
#define LCD_RW       LATEbits.LATE8

#define LCD_DB4_TRIS TRISDbits.TRISD0
#define LCD_DB4      LATDbits.LATD0
#define LCD_DB5_TRIS TRISDbits.TRISD1
#define LCD_DB5      LATDbits.LATD1
#define LCD_DB6_TRIS TRISDbits.TRISD2
#define LCD_DB6      LATDbits.LATD2
#define LCD_DB7_TRIS TRISDbits.TRISD3
#define LCD_DB7_R    PORTDbits.RD3
#define LCD_DB7      LATDbits.LATD3


/******************************************************************************/
/* Prototypes of additional functions										 */
/******************************************************************************/

void waitLCD();

void LCDWriteCmd(char cmd);
void LCDWriteNibbleCmd(char cmd);

void LCDWriteData(char data);


/******************************************************************************/
/* Functions																  */
/******************************************************************************/

// Initialize LCD
// This function forces a reset and initialize the LCD (16x2)
void LCDInit()
{
	// Set main pins as outputs
	LCD_EN_TRIS = 0;
	LCD_RS_TRIS = 0;
	LCD_RW_TRIS = 0;

	// Reset
	Delay15ms();
	LCDWriteNibbleCmd(0x30);
	Delay5ms();
	LCDWriteNibbleCmd(0x30);
	Delay150us();
	LCDWriteNibbleCmd(0x30);
	waitLCD();

	// End reset
	LCDWriteNibbleCmd(0x20);  // Set 4-bits interface
	waitLCD();

	LCDWriteCmd(0x28);
	waitLCD();

	// Display OFF
	LCDWriteCmd(0x08);
	waitLCD();

	// Clear display
	LCDWriteCmd(0x01);
	waitLCD();

	// Entry mode set incr. address and no shift
	LCDWriteCmd(0x06);
	waitLCD();

	/* End Initialization */
	waitLCD();

	// Display ON
	LCDWriteCmd(0x0E);
	waitLCD();
}

// Clear LCD
void LCDClear(){
	LCDWriteCmd(0x01);
	waitLCD();
}

// Move the cursor of the LCD to home
void LCDMoveHome(){
	LCDWriteCmd(0x02);
	waitLCD();
}

// Move LCD cursor to first line
void LCDMoveFirstLine(){
	LCDWriteCmd(0x80);
	waitLCD();
}

// Move LCD cursor to second line
void LCDMoveSecondLine(){
	LCDWriteCmd(0xC0);
	waitLCD();
}

// Writes a string to the LCD
void LCDPrint(char *str){
	while(*str != '\0'){
		waitLCD();
		LCDWriteData(*str);
		str++;
	}
}

// Move LCD cursor left
void LCDMoveLeft(){
	LCDWriteCmd(0x10);
	waitLCD();
}

// Move LCD cursor right
void LCDMoveRight(){
	LCDWriteCmd(0x14);
	waitLCD();
}

// Scroll LCD Screen left
void LCDScrollLeft(){
	LCDWriteCmd(0x18);
	waitLCD();
}

// Scroll LCD right
void LCDScrollRight(){
	LCDWriteCmd(0x1E);
	waitLCD();
}

// Hide LCD cursor
void LCDHideCursor(){
	LCDWriteCmd(0x0C);
	waitLCD();
}

// Turn on underline LCD cursor
void LCDTurnOnUnderline(){
	LCDWriteCmd(0x0E);
	waitLCD();
}

// Turn on blinking-block LCD cursor
void LCDTurnOnBlinking(){
	LCDWriteCmd(0x0F);
	waitLCD();
}


/******************************************************************************/
/* Additional functions                                                       */
/******************************************************************************/

void waitLCD()
{
	char busyValue = 1;

	// Set LCD data pins as inputs
	LCD_DB7_TRIS = 1;
	LCD_DB6_TRIS = 1;
	LCD_DB5_TRIS = 1;
	LCD_DB4_TRIS = 1;

	Delay20us();

	// Read Busy Flag and Address Counter
	LCD_RS = 0;
	LCD_RW = 1;

	// Setup time
	Delay20us();
	Delay20us();

	while(busyValue){
		LCD_EN = 1;

		// Data delay time
		Delay20us();
		Delay20us();

		busyValue = LCD_DB7_R;

		Delay20us();
		Delay20us();

		// Restore default state
		LCD_EN = 0;

		// Hold on time
		Delay20us();
		Delay20us();
	}
	/* Restore default state */
	LCD_RW = 0;
}

void LCDWriteCmd(char cmd)
{
	// Send command to Instruction Register
	LCD_RS = 0;
	LCD_RW = 0;

	// Set LCD data pins as outputs
	LCD_DB7_TRIS = 0;
	LCD_DB6_TRIS = 0;
	LCD_DB5_TRIS = 0;
	LCD_DB4_TRIS = 0;

	// Send first nibble
	LCD_DB7 = (cmd & 0x80) >> 7;
	LCD_DB6 = (cmd & 0x40) >> 6;
	LCD_DB5 = (cmd & 0x20) >> 5;
	LCD_DB4 = (cmd & 0x10) >> 4;

	// Wait until nibble is processed
	Delay20us(); // min 20us
	LCD_EN = 1;
	Delay20us(); // min 230n
	LCD_EN = 0;
	Delay20us();

	// Send command to Instruction Register
	LCD_RS = 0;
	LCD_RW = 0;

	// Send second nibble
	LCD_DB7 = (cmd & 0x08) >> 3;
	LCD_DB6 = (cmd & 0x04) >> 2;
	LCD_DB5 = (cmd & 0x02) >> 1;
	LCD_DB4 = (cmd & 0x01) >> 0;

	// Wait until nibble is processed
	Delay20us(); // min 20us
	LCD_EN = 1;
	Delay20us(); // min 230n
	LCD_EN = 0;
	Delay20us();

	/* Restore default state */
	// Set LCD data pins as inputs
	LCD_DB7_TRIS = 1;
	LCD_DB6_TRIS = 1;
	LCD_DB5_TRIS = 1;
	LCD_DB4_TRIS = 1;
}

void LCDWriteNibbleCmd(char cmd)
{
	// Send command to Instruction Register
	LCD_RS = 0;
	LCD_RW = 0;

	// Set higher LCD data pins as outputs
	LCD_DB7_TRIS = 0;
	LCD_DB6_TRIS = 0;
	LCD_DB5_TRIS = 0;
	LCD_DB4_TRIS = 0;

	// Send nibble
	LCD_DB7 = (cmd & 0x80) >> 7;
	LCD_DB6 = (cmd & 0x40) >> 6;
	LCD_DB5 = (cmd & 0x20) >> 5;
	LCD_DB4 = (cmd & 0x10) >> 4;

	// Wait until nibble is processed
	Delay20us(); // min 20us
	LCD_EN = 1;
	Delay20us(); // min 230n
	LCD_EN = 0;
	Delay20us();

	/* Restore default state */
	// Set higher LCD data pins as inputs
	LCD_DB7_TRIS = 1;
	LCD_DB6_TRIS = 1;
	LCD_DB5_TRIS = 1;
	LCD_DB4_TRIS = 1;
}

void LCDWriteData(char data)
{
	// Write data to (DD|CG)RAM
	LCD_RS = 1;
	LCD_RW = 0;

	// Set LCD pins as outputs
	LCD_DB7_TRIS = 0;
	LCD_DB6_TRIS = 0;
	LCD_DB5_TRIS = 0;
	LCD_DB4_TRIS = 0;

	// Send first nibble
	LCD_DB7 = (data & 0x80) >> 7;
	LCD_DB6 = (data & 0x40) >> 6;
	LCD_DB5 = (data & 0x20) >> 5;
	LCD_DB4 = (data & 0x10) >> 4;

	// Wait until nibble is processed
	Delay20us(); // min 20us
	LCD_EN = 1;
	Delay20us(); // min 230n
	LCD_EN = 0;
	Delay20us();

	// Write data to (DD|CG)RAM
	LCD_RS = 1;
	LCD_RW = 0;

	// Send second nibble
	LCD_DB7 = (data & 0x08) >> 3;
	LCD_DB6 = (data & 0x04) >> 2;
	LCD_DB5 = (data & 0x02) >> 1;
	LCD_DB4 = (data & 0x01) >> 0;

	// Wait until nibble is processed
	Delay20us(); // min 20us
	LCD_EN = 1;
	Delay20us(); // min 230n
	LCD_EN = 0;
	Delay20us();

	/* Restore default state */
	// Set LCD pins as inputs
	LCD_DB7_TRIS = 1;
	LCD_DB6_TRIS = 1;
	LCD_DB5_TRIS = 1;
	LCD_DB4_TRIS = 1;

	LCD_RW = 0;
	LCD_RS = 0;
}
