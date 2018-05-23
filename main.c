/******************************************************************************/
/*                                                                            */
/*  Description:                                                              */
/*                                                                            */
/*  Author: Yo                                                                  */
/*                                                                            */
/******************************************************************************/

#include <p30f4011.h>
#include <stdio.h>
#include <salvo.h>

#include <string.h>
#include <stdlib.h>
#include <time.h>


#include "libLEDs.h"
#include "libTIMER.h"
#include "libLCD.h"
#include "libCAD.h"
#include "delay.h"
#include "libKEYB.h"


/******************************************************************************/
/* Configuration words                                                        */
/******************************************************************************/
_FOSC(CSW_FSCM_OFF & EC_PLL16);
_FWDT(WDT_OFF);
_FBORPOR(MCLR_EN & PBOR_OFF & PWRT_OFF);
_FGS(CODE_PROT_OFF);

/******************************************************************************/
/* Hardware                                                                   */
/******************************************************************************/


/******************************************************************************/
/* Salvo elements declarations                                                */
/******************************************************************************/


// Tasks TCBs
#define TASK_TENSION_MONITOR_P 		OSTCBP(1) /* task #1 */
#define TASK_GLYCEMIA_MONITOR_P 	OSTCBP(2) /* task #2 */
#define TASK_PACIENT_STATUS_P       OSTCBP(3) /* task #3 */
#define TASK_SHOW_OUTPUT_P          OSTCBP(4) /* task #4 */

// Tasks priorities
#define PRIO_TENSION_MONITOR		0
#define PRIO_GLYCEMIA_MONITOR		0
#define PRIO_PACIENT_STATUS         0
#define PRIO_SHOW_OUTPUT			0

// OS events control blocks (number of OS EVENT)
// Recall that the number of OS event must range from 1 to OSEVENTS (defined in salvocfg.h)
#define MSG_FOR_SHOW_OUTPUT            OSECBP(2)
#define EFLAG_FOR_PACIENT_STATUS       OSECBP(3)
#define EFLAG_FOR_PACIENT_STATUS_EFCB  OSEFCBP(1)

#define FLAG_TENSION        0b0001
#define FLAG_GLYCEMIA       0b0010
#define FLAG_TEMPERATURE    0b0100
#define FLAG_OXYGEN_SAT     0b1000
 

/******************************************************************************/
/* Global Variable and macros declaration                                     */
/******************************************************************************/

typedef struct PacientStatus {
    unsigned int tension, glycemia, temperature, oxygen_sat;
} pacient_status_t ;

#define CAD_INTERRUPT_PRIO                  1
#define MONITORS_SAMPLING_PERIOD            4 // ticks (1 tick = 125 ms)
#define iniValueEventShowPacientStatus      0x0
#define maskEventForPacientStatus           0xF

unsigned int cad_value, tension, glycemia, temperature, oxygen_sat;
unsigned char
        glycemia_monitor_activated = 0,
        tension_monitor_activated = 0,
        temperature_monitor_activated = 0,
        oxygen_sat_monitor_activated = 0;


/******************************************************************************/
/* Procedures declaration                                                     */
/******************************************************************************/


/******************************************************************************/
/* TASKS declaration and implementation                                       */
/******************************************************************************/

 /* Task states

	DESTROYED / uninitalized
	STOPPED
	WAITING
	DELAYED
	ELIGIBLE /READY
	RUNNING

*/


void TaskTensionMonitor(void){
	while(1){
        if(getKeyNotBlocking() == 0){
            tension_monitor_activated = !tension_monitor_activated;
            toggleLED(0);
        }
        if(tension_monitor_activated){
            tension = (cad_value / 8) + 25;
            OSSetEFlag(EFLAG_FOR_PACIENT_STATUS, FLAG_TENSION);
        }
        OS_Delay(MONITORS_SAMPLING_PERIOD);
	}
}

void TaskGlycemiaMonitor(void){
	while(1){
        if(getKeyNotBlocking() == 2){
            glycemia_monitor_activated = !glycemia_monitor_activated;
            toggleLED(1);
        }
        if(glycemia_monitor_activated){ 
            glycemia = ((cad_value % 40) + 70) + (rand() % 10);
            OSSetEFlag(EFLAG_FOR_PACIENT_STATUS, FLAG_GLYCEMIA);
        }
		OS_Delay(MONITORS_SAMPLING_PERIOD);
	}
}

void TaskPacientStatus(void){
    static pacient_status_t pacientStatus;
    char eFlag;
    
    while(1){
        OSClrEFlag(EFLAG_FOR_PACIENT_STATUS, maskEventForPacientStatus);
        OS_WaitEFlag(EFLAG_FOR_PACIENT_STATUS, maskEventForPacientStatus, OSANY_BITS, OSNO_TIMEOUT);
        eFlag = OSReadEFlag(EFLAG_FOR_PACIENT_STATUS);
		if (eFlag & FLAG_TENSION)   pacientStatus.tension = tension;
		if (eFlag & FLAG_GLYCEMIA)  pacientStatus.glycemia = glycemia;
        if (eFlag & FLAG_TEMPERATURE)  pacientStatus.temperature = temperature;
        if (eFlag & FLAG_OXYGEN_SAT)  pacientStatus.oxygen_sat = oxygen_sat;
        
        OSSignalMsg(MSG_FOR_SHOW_OUTPUT, (OStypeMsgP) &pacientStatus);
        // enviar CAN
    }
}

void TaskShowOutput(void){
    static char msg[20];
    OStypeMsgP msgP;
	pacient_status_t *pacientStatusP;
    while(1){
        OS_WaitMsg(MSG_FOR_SHOW_OUTPUT, &msgP, OSNO_TIMEOUT);
        pacientStatusP = (pacient_status_t *) msgP;

        // TODO activar o desactivar zumbador con led para indicar de quien es la alarma
        LCDClear();
        if(tension_monitor_activated){
            sprintf(msg, "t:%dmmHg", pacientStatusP->tension);
            LCDMoveHome();
            LCDMoveFirstLine();
            LCDPrint(msg);
        }
        if(glycemia_monitor_activated){
            sprintf(msg, "G:%dg/ml", pacientStatusP->glycemia);
            LCDMoveHome();
            LCDMoveSecondLine();
            LCDPrint(msg);
        }
        if(temperature_monitor_activated){
            sprintf(msg, "T:%dßC", pacientStatusP->temperature);
            LCDMoveHome();
            LCDMoveFirstLine();
            int i;
            for (i = 0; i < 10; i++) {
                LCDMoveRight();
            }
            LCDPrint(msg);
        }
        if(oxygen_sat_monitor_activated){
            sprintf(msg, "Ox:%d%%", pacientStatusP->oxygen_sat);
            LCDMoveHome();
            LCDMoveSecondLine();
            int i;
            for (i = 0; i < 10; i++) {
                LCDMoveRight();
            }
            LCDPrint(msg);
        }
    }
}



/******************************************************************************/
/* Interrupts                                                                 */
/******************************************************************************/

void _ISR _T1Interrupt(void){
    TimerClearInt();
	OSTimer();
}

/////////////
// ADC ISR
/////////////

void _ISR _ADCInterrupt(void){
	// Get value from CAD
	cad_value = CADGetValue();

	// Clear interrupt
	IFS0bits.ADIF = 0;
}




/******************************************************************************/
/* Main                                                                       */
/******************************************************************************/

int main(void){

	// ===================
	// Init peripherals
	// ===================
    srand(time(NULL));
	initLEDs();
	LCDInit();
    KeybInit();
	CADInit(CAD_INTERACTION_BY_INTERRUPT, CAD_INTERRUPT_PRIO);
	CADStart(CAD_INTERACTION_BY_INTERRUPT);
    
	// =========================
	// Create Salvo structures
	// =========================
	OSInit();

	// Create tasks (name, tcb, priority) and push them to ELIGIBLE STATE
	// From 1 up to OSTASKS tcbs available
	// Priorities from 0 (highest) down to 15 (lowest)
	OSCreateTask(TaskTensionMonitor, TASK_TENSION_MONITOR_P, PRIO_TENSION_MONITOR);
    OSCreateTask(TaskGlycemiaMonitor, TASK_GLYCEMIA_MONITOR_P, PRIO_GLYCEMIA_MONITOR);
    OSCreateTask(TaskPacientStatus, TASK_PACIENT_STATUS_P, PRIO_PACIENT_STATUS);
    OSCreateTask(TaskShowOutput, TASK_SHOW_OUTPUT_P, PRIO_SHOW_OUTPUT);
    
    // Create event flag (ecbP, efcbP, initial value)
	OSCreateEFlag(EFLAG_FOR_PACIENT_STATUS, EFLAG_FOR_PACIENT_STATUS_EFCB, iniValueEventShowPacientStatus);
    
    // Create mailbox
	OSCreateMsg(MSG_FOR_SHOW_OUTPUT, (OStypeMsgP) 0);

	// =============================================
	// Enable peripherals that trigger interrupts
	// =============================================
	Timer1Init(TIMER_PERIOD_FOR_125ms, TIMER_PSCALER_FOR_125ms, 4);
	Timer1Start();

	// =============================================
	// Enter multitasking environment
	// =============================================
	while(1){
        OSSched();
	}
}


/******************************************************************************/
/* Procedures implementation                                                  */
/******************************************************************************/