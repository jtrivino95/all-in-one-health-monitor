/******************************************************************************/
/*                                                                            */
/*  Description: Punto de entrada del programa. Se muestra un menu interactivo*/ 
/*               al usuario para elegir el rol de la placa y se deriva el     */  
/*               flujo del programa al correspondiente main.                  */
/*               Tambien se inicializan los dispositivos comunes.             */
/*  Author: Luis Arjona y Javier Triviño                                      */                           
/*                                                                            */
/******************************************************************************/

//TODO controlar activacion y desactivacion de monitores desde tarea interfaz desde placa control y enviar senales a planta

#include <time.h>

#include <p30f4011.h>
#include <salvo.h>

#include "libLCD.h"
#include "libKEYB.h"
#include "libCAN.h"
#include "libLEDs.h"
#include "delay.h"


/******************************************************************************/
/* Configuration words                                                        */
/******************************************************************************/
_FOSC(CSW_FSCM_OFF & EC_PLL16);
_FWDT(WDT_OFF);
_FBORPOR(MCLR_EN & PBOR_OFF & PWRT_OFF);
_FGS(CODE_PROT_OFF);

/******************************************************************************/
/* Interrupts                                                                 */
/******************************************************************************/
#define PLANTA 0
#define CONTROL 1
char DEVICE_TYPE;

void _ISR _T1Interrupt(void){
    switch(DEVICE_TYPE){
        case CONTROL:
            control_ISR_T1Interrupt();
            break;
        case PLANTA:
            planta_ISR_T1Interrupt();
            break;
    }
}

void _ISR _ADCInterrupt(void){
    switch(DEVICE_TYPE){
        case CONTROL:
            control_ISR_ADCInterrupt();
            break;
        case PLANTA:
            planta_ISR_ADCInterrupt();
            break;
    }
}

void _ISR _C1Interrupt(void){
	switch(DEVICE_TYPE){
        case CONTROL:
            control_ISR_C1Interrupt();
            break;
        case PLANTA:
            planta_ISR_C1Interrupt();
            break;
    }
}

/******************************************************************************/
/* Main                                                                       */
/******************************************************************************/

int main(void) {
    srand(time(NULL));
    initLEDs();
	LCDInit();
    KeybInit();

    // Seleccion del rol de la placa
    LCDClear();
    LCDMoveHome();
    LCDPrint("Control  Planta");
    LCDMoveSecondLine();
    LCDPrint("0        1");

    short opt = -1;
    while(opt != 0 && opt != 1){
        opt = getKey();
        LCDClear();
        LCDMoveHome();
        switch(opt){
            case 0:
                LCDPrint("Control");
                DEVICE_TYPE = CONTROL;
                main_control();
                break;
            case 1:
                LCDPrint("Planta");
                DEVICE_TYPE = PLANTA;
                main_planta();
                break;
        }
    }
    return 0;
}
