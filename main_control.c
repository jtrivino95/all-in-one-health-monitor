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
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

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
#define TASK_SHOW_ACTUATORS_STATUS_P    OSTCBP(4)

// Tasks priorities
#define PRIO_TEMP_AND_OXYGEN_MONITOR    0
#define PRIO_USER_INTERFACE             0
#define PRIO_CONTROL                    0
#define PRIO_SHOW_ACTUATORS_STATUS      0

/******************************************************************************/
/* Global Variable and macros declaration                                     */
/******************************************************************************/

typedef struct PacientInfo {
    int pacient_id, age, genre, height, weight, smoker, diabetic;
} pacient_info_t;

typedef struct Actuators {
    float tension, glycemia, temperature, oxygen_sat;
} actuators_t;

#define MALE 1
#define FEMALE 0

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
actuators_t actuators = {0.0, 0.0, 0.0, 0.0};


/******************************************************************************/
/* Procedures declaration                                                     */
/******************************************************************************/
void readMicrocontrollerKeyboard(void);
void printHyperterminalMenu(void);
void readHyperterminalKeyboard(int *selected_opt, char *update, char *reset,
        char cursor_min_row, char num_options, char cursor_col);
void printSelectedOption(char selected_option);
void readPacientInfoWithHyperterminal(pacient_info_t *pacientInfo);
void readField(char prompt[], int* field);

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
            // The random value is the same for $rand_value_expiration_temp cycles
            if(rand_value_expiration_temp > 0){
                temperature += rand_value + actuators.temperature;
                rand_value_expiration_temp -= 1;
            } else {
                rand_value_expiration_temp = 7;
                rand_value = (float)(rand() % 2 - rand() % 2)
                             / (CAN_msg.magnitude_order);
            }
            CAN_msg.temperature_raw = (int)(temperature * CAN_msg.magnitude_order);
        }
        
        if(oxygen_sat_monitor_activated){
            // The random value is the same for $rand_value_expiration_oxygen cycles
            if(rand_value_expiration_oxygen > 0){
                oxygen_sat += rand_value + actuators.oxygen_sat;
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
    static char printed = 0, update = 0, reset = 0;
    static int cursor_min_row = 8, num_options = 3, cursor_col = 2,
               selected_option = 0, prev_sel_opt = -1;
    while(1){
        readMicrocontrollerKeyboard();
        if(!printed || reset){
            printHyperterminalMenu();
            printed = 1;
        }
        prev_sel_opt = selected_option;
        readHyperterminalKeyboard(&selected_option, &update, &reset, cursor_min_row, num_options, cursor_col);  
        if(update || selected_option != prev_sel_opt){
            printSelectedOption(selected_option);
        }
        OS_Delay(INPUT_SCAN_PERIOD);
	}
}

void TaskControl(void){
    static tens_glyc_act_pkt_t actuators_pkt;
    actuators_pkt.magnitude_order = 10;
    
    while(1){
        if(tension_monitor_activated && (tension < TENSION_LOWER_LIMIT || tension > TENSION_UPPER_LIMIT)){
            actuators.tension += tension > TENSION_UPPER_LIMIT  ? -0.1 : 0.1;
            
        } else {
            actuators.tension = 0;
        }
        
        if(glycemia_monitor_activated && (glycemia < GLYCEMIA_LOWER_LIMIT || glycemia > GLYCEMIA_UPPER_LIMIT)){
            actuators.glycemia += glycemia > GLYCEMIA_UPPER_LIMIT  ? -0.1 : 0.1;
            
        } else {
            actuators.glycemia = 0;
        }
        
        if(temperature_monitor_activated && (temperature < TEMPERATURE_LOWER_LIMIT || temperature > TEMPERATURE_UPPER_LIMIT)){
            actuators.temperature += temperature > TEMPERATURE_UPPER_LIMIT  ? -0.1 : 0.1;
            
        } else {
            actuators.temperature = 0;
        }
        
        if(oxygen_sat_monitor_activated && (oxygen_sat < OXYGEN_SAT_LOWER_LIMIT || oxygen_sat > OXYGEN_SAT_UPPER_LIMIT)){
            actuators.oxygen_sat += oxygen_sat > OXYGEN_SAT_UPPER_LIMIT  ? -0.1 : 0.1;
            
        } else {
            actuators.oxygen_sat = 0;
        }
        
        actuators_pkt.tension_act_raw = (int) (actuators.tension * actuators_pkt.magnitude_order);
        actuators_pkt.glycemia_act_raw = (int) (actuators.glycemia * actuators_pkt.magnitude_order);
        
        CANsendMessage(CONTROL_DATA_SID, &actuators_pkt, sizeof(tens_glyc_act_pkt_t));
        OS_Delay(CONTROL_PERIOD);
    }
}

void TaskShowActuatorsStatus(void){
    while(1){
        LCDClear();
		if (actuators.tension){
            LCDPrint(" Suministrando ");
            LCDMoveSecondLine();
            LCDPrint(" enalapril...");
        }
		else if (actuators.glycemia){
            LCDPrint(" Suministrando ");
            LCDMoveSecondLine();
            LCDPrint(" insulina...");
        }
        else if (actuators.temperature){
            LCDPrint(" Suministrando ");
            LCDMoveSecondLine();
            LCDPrint(" paracetamol...");
        }
        else if (actuators.oxygen_sat){
            LCDPrint(" Activando ");
            LCDMoveSecondLine();
            LCDPrint(" gafa nasal...");
        }
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
    TermNewLine();
    TermPrint("-------------------------------------------------");TermNewLine();
    TermPrint("-----          Mueve el cursor         ----------");TermNewLine();
    TermPrint("-----     con las teclas [w] y [s].    ----------");TermNewLine();
    TermPrint("----- Pulsa [ESPACIO] para actualizar. ----------");TermNewLine();
    TermPrint("-------------------------------------------------");TermNewLine();
    TermPrint("[ ] Informacion del paciente");TermNewLine();
    TermPrint("[ ] Constantes vitales");TermNewLine();
    TermPrint("[ ] Estado de los actuadores");TermNewLine();
    TermPrint("-------------------------------------------------");TermNewLine();
    while(BusyUART1());
}

void readHyperterminalKeyboard(int *selected_opt, char *update, char *reset,
                               char cursor_min_row, char num_options, char cursor_col){
    TermMove(cursor_min_row + *selected_opt, cursor_col);
    TermPrint(" ");

    char key = TermGetCharNotBlocking();
    while(TermGetCharNotBlocking() != NO_CHAR_RX_UART);
    
    *update = FALSE;
    *reset = FALSE;
    if(key == 'w'){
        if(*selected_opt > 0){
            *selected_opt -= 1;
        }
    } else if(key == 's'){
        if(*selected_opt < num_options-1){
            *selected_opt += 1;
        }
    } else if(key == ' '){
        *update = TRUE;
    } else if(key == 'p'){
        *reset = TRUE;
    }
      
    TermMove(cursor_min_row + *selected_opt, cursor_col);
    TermPrint("*");
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

    char buff[30];
    switch(selected_option){
        case 0:
            sprintf(buff,"ID Paciente:\t%d", pacientInfo.pacient_id);
            TermPrint(buff);TermNewLine();
            sprintf(buff,"Edad:\t\t%d", pacientInfo.age);
            TermPrint(buff);TermNewLine();
            sprintf(buff,"Sexo:\t\t%s", pacientInfo.genre == MALE ? "Hombre" : "Mujer");
            TermPrint(buff);TermNewLine();
            sprintf(buff,"Altura:\t\t%d", pacientInfo.height);
            TermPrint(buff);TermNewLine();
            sprintf(buff,"Peso:\t\t%d", pacientInfo.weight);
            TermPrint(buff);TermNewLine();
            sprintf(buff,"Fumador:\t%s", pacientInfo.smoker ? "Si" : "No");
            TermPrint(buff);TermNewLine();
            sprintf(buff,"Diabetico:\t%s", pacientInfo.diabetic ? "Si" : "No");
            TermPrint(buff);TermNewLine();
            break;
            
        case 1:
            if(tension_monitor_activated){
                sprintf(buff,"Tension:\t%.2f mmHg", tension);
                TermPrint(buff); TermNewLine();
            }
            if(glycemia_monitor_activated){
                sprintf(buff,"Glucemia:\t%.2f g/mL", glycemia);
                TermPrint(buff); TermNewLine();   
            }
            if(temperature_monitor_activated){
                sprintf(buff,"Temperatura:\t%.2f C", temperature);
                TermPrint(buff); TermNewLine();
            }
            if(oxygen_sat_monitor_activated){
                sprintf(buff,"Sat. oxigeno:\t%.2f %%", oxygen_sat);
                TermPrint(buff); TermNewLine();
            }
            break;
            
        case 2:
            sprintf(buff,"Enalapril:\t%s", actuators.tension ? "Suministrando medicamento" : "Desactivado");
            TermPrint(buff);TermNewLine();
            sprintf(buff,"Insulina:\t%s", actuators.glycemia ? "Suministrando medicamento" : "Desactivado");
            TermPrint(buff);TermNewLine();
            sprintf(buff,"Paracetamol:\t%s", actuators.temperature ? "Suministrando medicamento" : "Desactivado");
            TermPrint(buff);TermNewLine();
            sprintf(buff,"Gafa nasal:\t%s", actuators.oxygen_sat ? "Suministrando oxigeno" : "Desactivada");
            TermPrint(buff);TermNewLine();
    }
}

void readField(char prompt[], int* field){
    char key, buff_string[15], buff_char[2];
    buff_string[0] = '\0';
    buff_char[1] = '\0';
    
    TermPrint(prompt);
    do{
        key = TermGetChar();
        if(isprint(key)){
            buff_char[0] = key;
            TermPrint(buff_char);
            strcat(buff_string, buff_char);
        } else if(key == 127){ // Return
            TermMoveLeft();
            TermPrint(" ");
            TermMoveLeft();
            buff_string[strlen(buff_string)-1] = '\0';
        }
    }while(key != 13); // Enter
    TermNewLine();
    
    *field = atoi(buff_string);
}

void readPacientInfoWithHyperterminal(pacient_info_t *pacientInfo){
    TermClear();
    TermPrint("All-in-one Health Monitor v1.0.2");TermNewLine();
    TermNewLine();
    readField("Introduce ID: ", &pacientInfo->pacient_id);
    readField("Introduce edad: ", &pacientInfo->age);
    readField("Introduce genero [1-Hombre, 0-Mujer]: ", &pacientInfo->genre);
    readField("Introduce altura: ", &pacientInfo->height);
    readField("Introduce peso: ", &pacientInfo->weight);
    readField("Fumador? [1-Si, 0-No]: ", &pacientInfo->smoker);
    readField("Diabetico? [1-Si, 0-No]: ", &pacientInfo->diabetic);
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
    OSCreateTask(TaskShowActuatorsStatus, TASK_SHOW_ACTUATORS_STATUS_P, PRIO_SHOW_ACTUATORS_STATUS);
    OSCreateTask(TaskControl, TASK_CONTROL_P, PRIO_CONTROL);
    OSCreateTask(TaskUserInterface, TASK_USER_INTERFACE_P, PRIO_USER_INTERFACE);

	// =============================================
	// Enable peripherals that trigger interrupts
	// =============================================
	Timer1Init(TIMER_PERIOD_FOR_125ms, TIMER_PSCALER_FOR_125ms, 4);
	Timer1Start();
    
    LCDClear();
    LCDPrint(" Introduce datos");
    LCDMoveSecondLine();
    LCDPrint(" para continuar...");
    readPacientInfoWithHyperterminal(&pacientInfo);
//    pacientInfo.pacient_id = 3525;
//    pacientInfo.age = 22;
//    pacientInfo.genre = 0;
//    pacientInfo.height = 185;
//    pacientInfo.weight = 90;
//    pacientInfo.smoker = 1;
//    pacientInfo.diabetic = 1;
//    LCDClear();

	// =============================================
	// Enter multitasking environment
	// =============================================
	while(1){
        OSSched();
	}
}
