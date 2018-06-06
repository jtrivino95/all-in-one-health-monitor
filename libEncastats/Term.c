#include "Term.h"

#include <uart.h>

#define FXT 7372800 // Oscillator frequency
#define PLL 16
#define FCY (FXT * PLL) / 4 // Real system frequency

#define BAUD_RATE 115200 // UART bitrate
#define BRG (FCY / (16L * BAUD_RATE)) - 1L

#define ESC 27 // Escape
#define BEL  7 // Bell


void TermInit()
{
	U1MODE = 0;                     // Clear UART config - to avoid problems with bootloader

    // Config UART
	OpenUART1(
		UART_EN &             // Enable UART
		UART_DIS_LOOPBACK &   // Disable loopback mode
		UART_NO_PAR_8BIT &    // 8bits / No parity 
		UART_1STOPBIT,        // 1 Stop bit

		UART_TX_PIN_NORMAL &  // Tx break bit normal
		UART_TX_ENABLE,       // Enable Transmition
              
		BRG);                 // Baudrate

	TermClear();
}


void TermPrint(char * str){
	putsUART1((unsigned int*)str);
}

char TermGetChar(){
	while(!DataRdyUART1());
	return ReadUART1();
}

char TermGetCharNotBlocking(){
	if(DataRdyUART1())
	{
		return ReadUART1();
	}
	else
	{
		return NO_CHAR_RX_UART;
	}
}

void TermClear()
{
	WriteUART1(12); while(BusyUART1());
}

void TermMove(char row, char col)
{
	char row1, row2;
	char col1, col2;

	if(row < 0 || row > TERM_ROWS) return;
	if(col < 0 || col > TERM_COLS) return;

	// Row
	row1 = (row/10) + 48;
	row2 = (row%10) + 48;

	// Col
	col1 = (col/10) + 48;
	col2 = (col%10) + 48;

	WriteUART1(ESC);  while(BusyUART1());
	WriteUART1('[');  while(BusyUART1());
	WriteUART1(row1); while(BusyUART1());
	WriteUART1(row2); while(BusyUART1());
	WriteUART1(';');  while(BusyUART1());
	WriteUART1(col1); while(BusyUART1());
	WriteUART1(col2); while(BusyUART1());
	WriteUART1('H');  while(BusyUART1());
}

void TermMoveUp()
{
	WriteUART1(ESC); while(BusyUART1());
	WriteUART1('['); while(BusyUART1());
	WriteUART1('A'); while(BusyUART1());
}

void TermMoveDown()
{
	WriteUART1(ESC); while(BusyUART1());
	WriteUART1('['); while(BusyUART1());
	WriteUART1('B'); while(BusyUART1());
}

void TermMoveRight()
{
	WriteUART1(ESC); while(BusyUART1());
	WriteUART1('['); while(BusyUART1());
	WriteUART1('C'); while(BusyUART1());
}

void TermMoveLeft(){
	WriteUART1(ESC); while(BusyUART1());
	WriteUART1('['); while(BusyUART1());
	WriteUART1('D'); while(BusyUART1());
}

void TermBell()
{
	WriteUART1(7); while(BusyUART1());
}

void TermNewLine()
{
	putsUART1((unsigned int*)"\n\r");
}

void TermGetCurPos()
{
	char c;
	char row = 0;
	char col = 0;

	/* Send command */
	WriteUART1(ESC); while(BusyUART1());
	WriteUART1('['); while(BusyUART1());
	WriteUART1('6'); while(BusyUART1());
	WriteUART1('n'); while(BusyUART1());

	/* Get response ESC[{ROW};{COL}R */ 
	while(!DataRdyUART1()); c = ReadUART1(); // ESC
	while(!DataRdyUART1()); c = ReadUART1(); // [

	// Row
	while(!DataRdyUART1()); c = ReadUART1();
	while(c != ';'){
		row *= 10; row += c-48;
		while(!DataRdyUART1()); c = ReadUART1();
	}

	// Col
	while(!DataRdyUART1()); c = ReadUART1();
	while(c != 'R'){
		col *= 10; col += c-48;
		while(!DataRdyUART1()); c = ReadUART1();
	}

	/* Print response */
	WriteUART1((row/10) + 48);
	WriteUART1((row%10) + 48);

	WriteUART1((col/10) + 48);
	WriteUART1((col%10) + 48);
}
