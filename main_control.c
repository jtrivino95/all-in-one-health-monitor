/******************************************************************************/
/*                                                                            */
/*  Description: Main correspondiente a la placa Control                      */ 
/*  Author: Luis Arjona y Javier Triviño                                      */                           
/*                                                                            */
/******************************************************************************/

#include "main_control.h"

#include <p30f4011.h>
#include <salvo.h>
#include <stdio.h>
#include <uart.h>

#include "common.h"
#include "libLEDs.h"
#include "libTIMER.h"
#include "libLCD.h"
#include "libCAD.h"
#include "libKEYB.h"
#include "libCAN.h"
#include "Term.h"
#include "delay.h"

/******************************************************************************/
/* Salvo elements declarations                                                */
/******************************************************************************/

// Tasks TCBs
#define TASK_TEMP_AND_OXYGEN_MONITOR_P  OSTCBP(1)
#define TASK_USER_INTERFACE_P           OSTCBP(2)
#define TASK_CONTROL_P                  OSTCBP(3)

// Tasks priorities
#define PRIO_TEMP_AND_OXYGEN_MONITOR    0
#define PRIO_USER_INTERFACE             0
#define PRIO_CONTROL                    0


/******************************************************************************/
/* Global Variable and macros declaration                                     */
/******************************************************************************/

typedef struct PacientInfo {
    char *name, *age, *sex, *height, *weight, *smoker, *diabetic;
} pacient_info_t;

#define TENSION_LED         0
#define GLYCEMIA_LED        2
#define TEMPERATURE_LED     1
#define OXYGEN_SAT_LED      3

#define TENSION_KEY         0
#define GLYCEMIA_KEY        3
#define TEMPERATURE_KEY     2
#define OXYGEN_SAT_KEY      5

static float tension = 0.0, glycemia = 0.0, temperature = 36.0, oxygen_sat = 99.0;
static unsigned char glycemia_monitor_activated = 0,
        tension_monitor_activated = 0,
        temperature_monitor_activated = 0,
        oxygen_sat_monitor_activated = 0;
pacient_info_t pacientInfo;


/******************************************************************************/
/* Procedures declaration                                                     */
/******************************************************************************/
void readMicrocontrollerKeyboard(void);
void printHyperterminalMenu(void);
char readHyperterminalKeyboard(char cursor_min_row, char num_options,
                               char cursor_col, char selected_opt);
void printSelectedOption(char selected_option);

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
    static temp_ox_pkt_t CAN_msg;
    CAN_msg.magnitude_order = 10;
    static int rand_value_expiration_temp = 0, rand_value_expiration_oxygen = 0;
    static float rand_value = 0.0;
    
    while(1){
        if(temperature_monitor_activated){
            if(rand_value_expiration_temp > 0){
                temperature += rand_value;
                rand_value_expiration_temp -= 1;
            } else {
                rand_value_expiration_temp = 7;
                rand_value = (float)(rand() % 2 - rand() % 2)
                             / (CAN_msg.magnitude_order);
            }
            CAN_msg.temperature_raw = (int)(temperature * CAN_msg.magnitude_order);
        }
        
        if(oxygen_sat_monitor_activated){
            if(rand_value_expiration_oxygen > 0){
                oxygen_sat += rand_value;
                if(oxygen_sat > 100) oxygen_sat = 100;
                rand_value_expiration_oxygen -= 1;
            } else {
                rand_value_expiration_oxygen = 3;
                rand_value = (float)(rand() % 2 - rand() % 2)
                                    / (CAN_msg.magnitude_order);
            }
            CAN_msg.oxygen_sat_raw = (int) (oxygen_sat * CAN_msg.magnitude_order);
        }
        
        if(temperature_monitor_activated || oxygen_sat_monitor_activated){
            if(CANtransmissionCompleted()){ // TODO poner este if en todos?
                CANsendMessage(EXTERNAL_MONITORS_DATA_SID,
                        &CAN_msg,
                        sizeof(temp_ox_pkt_t));
            }
        }
        
        OS_Delay(MONITORS_SAMPLING_PERIOD);
	}
}

void TaskUserInterface(void){
    static char printed = 0;
    static int cursor_min_row = 6, num_options = 3, cursor_col = 2,
               selected_option = 0, prev_sel_opt = -1;
    while(1){
        readMicrocontrollerKeyboard();
        if(!printed){
            printHyperterminalMenu();
            printed = 1;
        }
        prev_sel_opt = selected_option;
        selected_option = readHyperterminalKeyboard(cursor_min_row, num_options, cursor_col, selected_option);  
        if(selected_option != prev_sel_opt){
            printSelectedOption(selected_option);
        }
        OS_Delay(INPUT_SCAN_PERIOD);
	}
}

void TaskControl(void){
    tens_glyc_act_pkt_t actuators;
    while(1){
        actuators.glycemia_act_raw = -10;
        actuators.tension_act_raw = -10;
        actuators.magnitude_order = 1;
        CANsendMessage(CONTROL_DATA_SID, &actuators, sizeof(tens_glyc_act_pkt_t));
        OS_Delay(CONTROL_PERIOD);
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
                tension = (float)((tens_glyc_pkt_t*) rxMsgData)->tension_raw /
                                ((temp_ox_pkt_t*) rxMsgData)->magnitude_order;
                glycemia = (float)((tens_glyc_pkt_t*) rxMsgData)->glycemia_raw /
                                  ((temp_ox_pkt_t*) rxMsgData)->magnitude_order;
                break;
        }
	}
}

/******************************************************************************/
/* Procedures implementation                                                  */
/******************************************************************************/
void readMicrocontrollerKeyboard(void){
    char key = getKeyNotBlocking();
    switch(key){
        case TENSION_KEY:
            tension_monitor_activated = !tension_monitor_activated;
            toggleLED(TENSION_LED);

    //                while not can tx complete
    //                    osyield

            CANsendMessage(
                    TENSION_MONITOR_STATUS_MSG_SID,
                    &tension_monitor_activated,
                    sizeof(unsigned char));
            break;

        case GLYCEMIA_KEY:
            glycemia_monitor_activated = !glycemia_monitor_activated;
            toggleLED(GLYCEMIA_LED);
            CANsendMessage(
                    GLYCEMIA_MONITOR_STATUS_MSG_SID,
                    &glycemia_monitor_activated,
                    sizeof(unsigned char));
            break;

        case TEMPERATURE_KEY:
            temperature_monitor_activated = !temperature_monitor_activated;
            toggleLED(TEMPERATURE_LED);
            CANsendMessage(
                    TEMPERATURE_MONITOR_STATUS_MSG_SID,
                    &temperature_monitor_activated,
                    sizeof(unsigned char));
            break;

        case OXYGEN_SAT_KEY:
            oxygen_sat_monitor_activated = !oxygen_sat_monitor_activated;
            toggleLED(OXYGEN_SAT_LED);
            CANsendMessage(
                    OXYGEN_SAT_MONITOR_STATUS_MSG_SID,
                    &oxygen_sat_monitor_activated,
                    sizeof(unsigned char));
            break;
    }
}

void printHyperterminalMenu(void){
    TermClear();
    TermPrint("All-in-one Health Monitor v1.0.2");TermNewLine();
    TermPrint("-------------------------------------------------");TermNewLine();
    TermNewLine();
    TermPrint("----- Mueve el cursor arriba o abajo ------------");TermNewLine();
    TermPrint("----- Pulsa ENTER para seleccionar la opcion ----");TermNewLine();
    TermPrint("[ ] Informacion del paciente");TermNewLine();
    TermPrint("[ ] Constantes vitales");TermNewLine();
    TermPrint("[ ] Estado de los actuadores");TermNewLine();
    TermPrint("-------------------------------------------------");TermNewLine();
    while(BusyUART1());
}

char readHyperterminalKeyboard(char cursor_min_row, char num_options, char cursor_col, char selected_opt){
    TermMove(cursor_min_row + selected_opt, cursor_col);
    TermPrint(" ");
    

    char key = TermGetCharNotBlocking();
    while(TermGetCharNotBlocking() != NO_CHAR_RX_UART);
    
    if(key == 'w'){
        if(selected_opt > 0){
            selected_opt -= 1;
        }
    } else if(key == 's'){
        if(selected_opt < num_options-1){
            selected_opt += 1;
        }
    }
      
    TermMove(cursor_min_row + selected_opt, cursor_col);
    TermPrint("*");
    
    return selected_opt;
}

void printSelectedOption(char selected_option){
    TermMove(12,0);
    int i;
    for (i = 0; i < 10; i++) {
        TermPrint("                                        ");
        TermNewLine();
    }
    while(BusyUART1());
    TermMove(12,0);

    char buff[50];
    switch(selected_option){
        case 0:
            sprintf(buff,"Nombre:\t\t%s", pacientInfo.name);
            TermPrint(buff);TermNewLine();
            sprintf(buff,"Edad:\t\t%s", pacientInfo.age);
            TermPrint(buff);TermNewLine();
            sprintf(buff,"Sexo:\t\t%s", pacientInfo.sex);
            TermPrint(buff);TermNewLine();
            sprintf(buff,"Altura:\t\t%s", pacientInfo.height);
            TermPrint(buff);TermNewLine();
            sprintf(buff,"Peso:\t\t%s", pacientInfo.weight);
            TermPrint(buff);TermNewLine();
            sprintf(buff,"Fumador:\t%s", pacientInfo.smoker);
            TermPrint(buff);TermNewLine();
            sprintf(buff,"Diabetico:\t%s", pacientInfo.diabetic);
            TermPrint(buff);TermNewLine();
            break;
        case 1:
            sprintf(buff,"Tension:\t%.2f mmHg", tension);
            TermPrint(buff);TermNewLine();
            sprintf(buff,"Glucemia:\t%.2f g/mL", glycemia);
            TermPrint(buff);TermNewLine();
            sprintf(buff,"Temperatura:\t%.2f C", temperature);
            TermPrint(buff);TermNewLine();
            sprintf(buff,"Sat. oxigeno:\t%.2f %%", oxygen_sat);
            TermPrint(buff);TermNewLine();
            break;
        case 2:
            TermPrint("Opcion 3");TermNewLine();
            break;
        case 3:
            TermPrint("Opcion 4");TermNewLine();
            break;
    }
}

void main_control(void){
	// ===================
	// Init peripherals
	// ===================
    TermInit();
    CANinit(NORMAL_MODE, TRUE, TRUE, 0, 0);
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
    OSCreateTask(TaskControl, TASK_CONTROL_P, PRIO_CONTROL);
    OSCreateTask(TaskUserInterface, TASK_USER_INTERFACE_P, PRIO_USER_INTERFACE);
    

	// =============================================
	// Enable peripherals that trigger interrupts
	// =============================================
	Timer1Init(TIMER_PERIOD_FOR_125ms, TIMER_PSCALER_FOR_125ms, 4);
	Timer1Start();
    
    pacientInfo.name = "Manuel";
    pacientInfo.age = "22";
    pacientInfo.sex = "Hombre";
    pacientInfo.height = "1.85";
    pacientInfo.weight = "90";
    pacientInfo.smoker = "Si";
    pacientInfo.diabetic = "Si";
    
	// =============================================
	// Enter multitasking environment
	// =============================================
	while(1){
        OSSched();
	}
}
