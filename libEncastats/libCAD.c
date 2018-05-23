#include "libCAD.h"

#include <p30F4011.h>


///////////////////////////////////////////////////////////////////
///////////// Prototypes of private functions /////////////////////

void CADinitSimpleConfiguration(unsigned char interactionType);



///////////////////////////////////////////////////////////////////
///////////// Implementation of public functions //////////////////
///////////////////////////////////////////////////////////////////


void CADInit(unsigned char interactionType, unsigned char intPriority){


	CADinitSimpleConfiguration(interactionType);

	if (interactionType == CAD_INTERACTION_BY_INTERRUPT){

		/*
			TAD = Tcy = 33.9 ns
			Duration of sampling  (clocked conversion trigger) = 31 TADs = 31 * 33.9 = 1050.9 ns
			Duration of conversion (it is irrelevant for the rate at which interrupts will be generated)
			Generate interrupt after 16 sequences of sampling/conversion
			-> generate interrupt each 16 * 1050.9 ns = 16814.4 ns = 16.8144 ms 
	
		*/

		// Cambiamos el trigger de muestreo a automatico
		ADCON1bits.ASAM = 1;	// DIFERENTE A CUANDO SE UTILIZA POLLING

		// Cambiar para que se genere interrupcion cada decimo sexta (16) secuencia de muestreo/conversion
		ADCON2bits.SMPI = 0b1111;	// DIFERENTE A CUANDO SE UTILIZA POLLING

		// Cambiar ADCS (AD conversion clock select bits). DIFERENTE A CUANDO SE UTILIZA POLLING
		//ADCON3bits.ADCS = 31;	// 011111, TAD = (TCY/2)*(31 + 1) = 16TCY
		ADCON3bits.ADCS = 63;	// 111111, TAD = (TCY/2)*(63 + 1) = 32TCY (smoother)
	
		// Cambiar SAMC (autosample time bits). DIFERENTE A CUANDO SE UTILIZA POLLING
		ADCON3bits.SAMC = 31;	// 11111, clocked conversion trigger time = TAD * SAMC = 31 TAD


		// Set up interrupts for CAD
		IEC0bits.ADIE = 1; // Enable interrupt
		IFS0bits.ADIF = 0; // Clear ADC interrupt
		IPC2bits.ADIP = intPriority;
	}
	else {
		// Disable and clear interrupts for CAD
		IEC0bits.ADIE = 0; // Enable interrupt
		IFS0bits.ADIF = 0; // Clear ADC interrupt
		IPC2bits.ADIP = 0;
	}
}



void CADStart(unsigned char interactionType){

    ADCON1bits.ADON = 1; // Enable CAD. Must be done AFTER configuration
	if (interactionType == CAD_INTERACTION_BY_INTERRUPT){
		ADCON1bits.SAMP = 1;
	}
}



void CADStop(){

    ADCON1bits.ADON = 0;
}


void CADRequestValue(){
	   ADCON1bits.SAMP = 1;
}


unsigned char CADReadyValue(){

    return ADCON1bits.DONE;
}


unsigned int CADGetValue(){

    return ADCBUF0;
}



///////////////////////////////////////////////////////////////////
///////////// Implementation of private functions /////////////////
///////////////////////////////////////////////////////////////////

void CADinitSimpleConfiguration(unsigned char interactionType){

	// Set PIN B7 (AN7) as input
	TRISBbits.TRISB7 = 1;


	// AD CONFIGURATION REGISTER 1
	// 0000 0000 0000 0000
	// En este caso:
	// ADON = 0, AD converter off
	// ADSIDL = 0, continue operation in idle mode
	// FORM = 00, data output format = integer
	// SSRC (conversion trigger source) = 000, se usa SAMP para finalizar muestreo (lo cual inicia la conversion)
	// SIMSAM = 0, samplea multiples canales S/H secuencialmente
	// ASAM (A/D sample autostart) = 0, no automatic start of sampling. SAMP = 1 inicia el muestreo
	// SAMP = 0, holding (not sampling)
	// DONE = 0, es un bit de status, esta escritura se descarta
	ADCON1 = 0x0000;

	// Cambiamos a trigger de conversion a automatico
	ADCON1bits.SSRC = 0b111;

	// Cambiamos el trigger de muestreo a automatico
	//ADCON1bits.ASAM = 1;	// DIFERENTE A CUANDO SE UTILIZA POLLING


	// AD CONFIGURATION REGISTER 2
	// 0000 0000 0000 0010
	// VCFG (voltage reference configuration pins) = 000, ADVRefH = AVdd, ADVRefL = AVss
	// CSCNA = 0, do not scan inputs for CH0
	// CHPS = 00, convert CH0
	// BUFS (buffer fill status, only valid if BUFM = 1) = 0
	// SMPI (sample/convert sequences per interrupt) = 0000, interrupt at each sample/covert sequence
	// BUFM (buffer mode) = 1, two 8-words buffer
	// ALTS (alternate imput sample MUXs) = 0, use always MUXA
	ADCON2 = 0x0000;

	// Cambiar para que se genere interrupcion cada decimo sexta (16) secuencia de muestreo/conversion
	//ADCON2bits.SMPI = 0b1111;	// DIFERENTE A CUANDO SE UTILIZA POLLING


	// AD CONFIGURATION REGISTER 3
	// 0000 0000 0000 0010
	// SAMC (autosample time bits) = 00000, 0 TAD
	// ADRC = 0, AD conversion clock derived from system clock
	// ADCS (AD conversion clock select bits) = 000010, TAD = (TCY/2)*(2 + 1) = 3TCY/2
	ADCON3 = 0x0002;

	// Cambiar ADCS (AD conversion clock select bits) = 011111, TAD = (TCY/2)*(31 + 1) = 16TCY
	//ADCON3bits.ADCS = 31;	// DIFERENTE A CUANDO SE UTILIZA POLLING

	// Cambiar SAMC (autosample time bits) = 11111, 31 TAD
	//ADCON3bits.SAMC = 31;	// DIFERENTE A CUANDO SE UTILIZA POLLING


	// AD INPUT SELECT REGISTER
	// Especifica como se conectan los pines
	// a cada canal H/S
	// 0000 0000 0000 0111
	// En este caso:
	// Negative input para CH1, CH2 y CH3 = VREF-
	// Positive input para CH1, CH2 y CH3 = AN0, AN1 y AN2 respectivamente
	// Negative input para CH0 = VREF-
	// Positive input para CH0 = AN7
	ADCHS = 0x0007;

	// AD PORT CONFIGURATION REGISTER
	// Establece los pines que se van
	// a utilizar para samplear
	// 111 111 0111 1111
	// En este caso solo se va a utilizar el pin AN7
	ADPCFG = 0xFF7F;	 // all PORTB = Digital; RB7/AN7 = analog

	// AD INPUT SCAN SELECT REGISTER
	// Este registro permite escanear los bits conectados
	// a un canal S/H de forma alterna
	// 0000 0000 0000 0000
	// No escanear de forma alterna ningun input
	ADCSSL = 0;
}



void CADClearInt()
{
	IFS0bits.ADIF = 0;
}