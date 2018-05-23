
#include <p30f4011.h>
#include "libCAN.h"


// ================================================================
//				INITIALIZATION FUNCTIONS
// ================================================================


void CANinit(unsigned char operationalMode, unsigned char enableRxInt, unsigned char enableTxInt, unsigned int filterSID, unsigned int maskSID){
	C1CTRLbits.REQOP = 0b100;          // Set configuration mode

	while(C1CTRLbits.OPMODE != 0b100); // Wait until configuration mode

	//C1CTRLbits.CANCAP = 0;
	C1CTRLbits.CANCKS = 1; // FCAN = FCY

	/* BTR */
	// BTR1
	C1CFG1bits.BRP = 0;
	C1CFG1bits.SJW = 0b00; // 1 TQ

	// BTR2
	C1CFG2bits.PRSEG  = 0; // 1 TQs
	C1CFG2bits.SEG1PH = 3; // 4 TQs
	C1CFG2bits.SEG2PH = 1; // 2 TQs
	
	/* Tx buffer 0 */
	// Clear
	C1TX0CONbits.TXREQ = 0; // buffers 1 y 2?
	//C1TX0CONbits.TXPRI = 3; // max priority
	
	/* Rx buffer 0 and 1 */
	// Clear
	C1RX0CONbits.RXFUL = 0;
//	C1RX1CONbits.RXFUL = 0;
	
	// Disable double buffer
	C1RX0CONbits.DBEN = 0;
	C1RX0CONbits.RXB0DBEN = 0;
	
	// Configure acceptance masks of buffer 0
	C1RXM0SIDbits.SID	 = maskSID;
	//C1RXM1SIDbits.SID	 = maskSID;
					   //= 0b11111111111;	

	C1RXM0SIDbits.MIDE   = 1; // Determine as EXIDE in filter
	//C1RXM1SIDbits.MIDE   = 1; // Determine as EXIDE in filter
	
	// Link to acceptance filter 0
	//C1RX0CONbits.FILHIT0 = 0;		// BE CAREFUL! POSSIBLE BUG! IF FILHIT0 ALIAS IS USED, THEN RX0BUF ALSO USES FILTER 1!
	C1RX0CONbits.FILHIT = 0;

	// Configure acceptance filter 0 (standar identifier is assumed)
	C1RXF0SIDbits.SID	= filterSID;
	//C1RXF1SIDbits.SID	= filterSID;
					  //= 0b11111111111;

	C1RXF0SIDbits.EXIDE	= 0; 		// Enable filter for standard identifier
	//C1RXF1SIDbits.EXIDE	= 0;
	
	// Configure interrupts
	if (enableRxInt || enableTxInt)
	{
		IEC1bits.C1IE = 1; // Enable CAN interrupt
		IFS1bits.C1IF = 0; // Clear CAN interrupt flag

		C1INTE = 0;
		if (enableRxInt)
		{
			C1INTEbits.RX0IE = 1; // Enable interrupt associated to rx buffer 0
			C1INTFbits.RX0IF = 0; // Clear interrupt flag associated to rx buffer 0
		}
			
		if (enableTxInt)
		{
			C1INTEbits.TX0IE = 1; // Enable interrupt associated to tx buffer 0
			C1INTFbits.TX0IF = 0; // Clear interrupt flag associated to tx buffer 0
		}

	}

	// Set requested operational mode
	C1CTRLbits.REQOP = operationalMode;          // Set loopback/normal mode

	while(C1CTRLbits.OPMODE != operationalMode); // Wait until loopback/normal mode
	
}



// ================================================================
//				INTERRUPT MANAGEMENT FUNCTIONS
// ================================================================


unsigned char CANrxInt ()
{
	return (C1INTFbits.RX0IF == 1);	
}



unsigned char CANtxInt ()
{
	return (C1INTFbits.TX0IF == 1);	
}


void CANclearGlobalInt ()
{
	// Clear CAN global interrupt
	IFS1bits.C1IF = 0; // Clear CAN1 interrupt flag before returning
}


void CANclearRxInt ()
{
	// Clear CAN RX buffer 0 interrupt
	C1INTFbits.RX0IF = 0;
}


void CANclearTxInt ()
{
	// Clear CAN TX buffer 0 interrupt
	C1INTFbits.TX0IF = 0;
}



// ================================================================
//				TRANSMISSION FUNCTIONS
// ================================================================


void CANsendMessage(unsigned int sid, unsigned char * data, unsigned char datalen)
{
	unsigned char i;

	// Set identifier (standar identifier is assumed)
	C1TX0SIDbits.TXIDE = 0;	// Standard identifier
	C1TX0SIDbits.SRR = 0; 	// No remote frame
	
	C1TX0SIDbits.SID5_0 =	sid & 0b0000000000111111;
	C1TX0SIDbits.SID10_6 =	(sid & 0b0000011111000000) >> 6;

	// Set DLC
	C1TX0DLCbits.DLC = datalen;

	// Fill up TX0 buffer
    for(i = 0; i < datalen; i++)
    {
		*((unsigned char *)&C1TX0B1+i)= data[i];
    }

	// Request transmission
	C1TX0CONbits.TXREQ = 1;

	// Wait until transmission is completed
	//while(C1TX0CONbits.TXREQ != 0); // Wait for transmission
}


unsigned char CANtransmissionCompleted()
{
	if (C1TX0CONbits.TXREQ == 0)
	{
		return 1;	// TRUE
	}
	else
	{
		return 0;	// FALSE
	}
}


// ================================================================
//				RECEPTION FUNCTIONS
// ================================================================

unsigned int CANreadRxMessageSID (void)
{
	return C1RX0SIDbits.SID;
}

unsigned int CANreadRxMessageDLC (void)
{
	return C1RX0DLCbits.DLC;
}


void CANreadRxMessageDATA (unsigned char * data)
{
	unsigned char i;
	unsigned char dlc;

	dlc = C1RX0DLCbits.DLC;

	for(i=0; i < dlc; i++){
		switch(i){
			case 0: data[0] =  C1RX0B1 & 0x00FF; break;
			case 1: data[1] = (C1RX0B1 & 0xFF00) >> 8; break;
			case 2: data[2] =  C1RX0B2 & 0x00FF; break;
			case 3: data[3] = (C1RX0B2 & 0xFF00) >> 8; break;
			case 4: data[4] =  C1RX0B3 & 0x00FF; break;
			case 5: data[5] = (C1RX0B3 & 0xFF00) >> 8; break;
			case 6: data[6] =  C1RX0B4 & 0x00FF; break;
			case 7: data[7] = (C1RX0B4 & 0xFF00) >> 8; break;
		}
	}
}



void CANclearRxBuffer ()
{
	// Clear rx buffer 0
	C1RX0CONbits.RXFUL = 0;
}



unsigned char CANreceptionCompleted ()
{
	if (C1RX0CONbits.RXFUL == 1)
	{
		return 1;	// TRUE
	}
	else
	{
		return 0;	// FALSE
	}
}