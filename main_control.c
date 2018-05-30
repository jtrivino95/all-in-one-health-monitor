/******************************************************************************/
/*                                                                            */
/*  Description: Main correspondiente a la placa Control                      */ 
/*  Author: Luis Arjona y Javier Triviño                                      */                           
/*                                                                            */
/******************************************************************************/

#include "main_control.h"

#include <p30f4011.h>
#include <salvo.h>

#include "common.h"
#include "libLEDs.h"
#include "libTIMER.h"
#include "libLCD.h"
#include "libCAD.h"
#include "libKEYB.h"
#include "libCAN.h"

/******************************************************************************/
/* Salvo elements declarations                                                */
/******************************************************************************/

// Tasks TCBs
#define TASK_TEMP_AND_OXYGEN_MONITOR_P      OSTCBP(1)

// Tasks priorities
#define PRIO_TEMP_AND_OXYGEN_MONITOR    0


/******************************************************************************/
/* Global Variable and macros declaration                                     */
/******************************************************************************/

#define CAD_INTERRUPT_PRIO                  1

#define TENSION_LED         0
#define GLYCEMIA_LED        2
#define TEMPERATURE_LED     1
#define OXYGEN_SAT_LED      3

#define TENSION_KEY         0
#define GLYCEMIA_KEY        3
#define TEMPERATURE_KEY     2
#define OXYGEN_SAT_KEY      5

static unsigned int cad_value, tension, glycemia, temperature, oxygen_sat;
static unsigned char glycemia_monitor_activated = 1,
        tension_monitor_activated = 1,
        temperature_monitor_activated = 1,
        oxygen_sat_monitor_activated = 1;


/******************************************************************************/
/* Procedures declaration                                                     */
/******************************************************************************/


/******************************************************************************/
/* TASKS declaration and implementation for PLANTA                            */
/******************************************************************************/

 /* Task states

	DESTROYED / uninitalized
	STOPPED
	WAITING
	DELAYED
	ELIGIBLE /READY
	RUNNING

*/

/******************************************************************************/
/* TASKS declaration and implementation                                       */
/******************************************************************************/

void TaskTempAndOxygenMonitor(void){
    static pacient_status_t pacientStatus;
    while(1){
        if(temperature_monitor_activated){ // TODO en vez de este if, deshabilitar la tarea
//            pacientStatus.temperature = (cad_value / 25) + 33;
            pacientStatus.temperature = rand() % 10;
        }
        
        if(oxygen_sat_monitor_activated){ // TODO en vez de este if, deshabilitar la tarea
//            pacientStatus.oxygen_sat = (cad_value / 25) + 90 + (rand() % 10);
            pacientStatus.oxygen_sat = rand() % 10;
        }
        
        if(CANtransmissionCompleted()){
            toggleLED(0);
            CANsendMessage(EXTERNAL_MONITORS_DATA_SID, &pacientStatus, sizeof(pacient_status_t)); // TODO crear struct que envie los dos datos
        }
        
        OS_Delay(MONITORS_SAMPLING_PERIOD);
	}
}


/******************************************************************************/
/* Interrupts                                                                 */
/******************************************************************************/

void control_ISR_T1Interrupt(void){
    TimerClearInt();
	OSTimer();
}
void control_ISR_ADCInterrupt(void){
    // Get value from CAD
	cad_value = CADGetValue();

	// Clear interrupt
	IFS0bits.ADIF = 0;
}

void control_ISR_C1Interrupt(void){
    unsigned int rxMsgSID;
	unsigned char rxMsgData[8];
	unsigned char rxMsgDLC;

	// Clear CAN global interrupt
	CANclearGlobalInt();

	if (CANrxInt()){
		// Clear RX interrupt
		CANclearRxInt ();

		// Read SID, DLC and DATA
		rxMsgSID = CANreadRxMessageSID();
		rxMsgDLC = CANreadRxMessageDLC();
		CANreadRxMessageDATA (rxMsgData);

		// Clear RX buffer
		CANclearRxBuffer();

		// Process data
        switch(rxMsgSID){
            case EXTERNAL_MONITORS_DATA_SID:
                break;
        }
	}
}

/******************************************************************************/
/* Procedures implementation                                                  */
/******************************************************************************/

void main_control(void){
	// ===================
	// Init peripherals
	// ===================
	CADInit(CAD_INTERACTION_BY_INTERRUPT, CAD_INTERRUPT_PRIO);
	CADStart(CAD_INTERACTION_BY_INTERRUPT);
    
	// =========================
	// Create Salvo structures
	// =========================
	OSInit();

	// Create tasks (name, tcb, priority) and push them to ELIGIBLE STATE
	// From 1 up to OSTASKS tcbs available
	// Priorities from 0 (highest) down to 15 (lowest)
	OSCreateTask(TaskTempAndOxygenMonitor, TASK_TEMP_AND_OXYGEN_MONITOR_P, PRIO_TEMP_AND_OXYGEN_MONITOR);
//    OSCreateTask(TaskShowActuatorsStatus, TASK_SHOW_ACTUATORS_STATUS_P, PRIO_SHOW_ACTUATORS_STATUS);
//    OSCreateTask(TaskControl, TASK_CONTROL_P, PRIO_CONTROL);
//    OSCreateTask(TaskUserInterface, TASK_USER_INTERFACE_P, PRIO_USER_INTERFACE);
    

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
