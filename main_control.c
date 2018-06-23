/******************************************************************************/
/*                                                                            */
/*  Description: Main correspondiente a la placa Control                      */ 
/*  Author: Luis Arjona y Javier Triviño                                      */                           
/*                                                                            */
/******************************************************************************/

#include "main_control.h"

#include <p30f4011.h>
#include <salvo.h>
#include <ctype.h>

#include "common.h"
#include "libLEDs.h"
#include "libTIMER.h"
#include "libLCD.h"
#include "libCAD.h"
#include "libKEYB.h"
#include "libCAN.h"
#include "Term.h"

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

// OS events control blocks (number of OS EVENT)
// Recall that the number of OS event must range from 1 to OSEVENTS (defined in salvocfg.h)
#define MSG_FOR_CONTROL                         OSECBP(1)
#define EFLAG_FOR_SHOW_ACTUATORS_STATUS         OSECBP(2)
#define BINSEM_SEND_CAN_MSG                     OSECBP(3)

#define EFLAG_FOR_SHOW_ACTUATORS_STATUS_EFCB    OSEFCBP(2)

#define FLAG_TENSION_ACTUATOR       0b0001
#define FLAG_GLYCEMIA_ACTUATOR      0b0010
#define FLAG_TEMPERATURE_ACTUATOR   0b0100
#define FLAG_OXYGEN_SAT_ACTUATOR    0b1000

/******************************************************************************/
/* Global Variable and macros declaration                                     */
/******************************************************************************/

typedef struct PacientInfo {
    int pacient_id, age, genre, height, weight, smoker, diabetic;
} pacient_info_t;

typedef struct Monitors {
    float tension, glycemia, temperature, oxygen_sat;
} monitors_t;

typedef struct Actuators {
    float tension, glycemia, temperature, oxygen_sat;
} actuators_t;

typedef struct Thresholds {
    float tension_upper_limit, tension_lower_limit, glycemia_upper_limit,
        glycemia_lower_limit, temperature_upper_limit, temperature_lower_limit,
        oxygen_sat_upper_limit, oxygen_sat_lower_limit;
} thresholds_t;

#define iniValueEventShowActuatorsStatus    0x0
#define maskEventForShowActuatorsStatus     0xF

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

static unsigned char glycemia_monitor_activated = 0,
        tension_monitor_activated = 0,
        temperature_monitor_activated = 0,
        oxygen_sat_monitor_activated = 0;
pacient_info_t pacientInfo;
monitors_t monitors = {0.0, 0.0, 26.0, 97.0};
actuators_t actuators = {0.0, 0.0, 0.0, 0.0};
thresholds_t thresholds;


/******************************************************************************/
/* Procedures declaration                                                     */
/******************************************************************************/
void readMicrocontrollerKeyboard(void);
void printHyperterminalMenu(void);
void readHyperterminalKeyboard(int *selected_opt, char *update, char *reset,
        char cursor_min_row, char num_options, char cursor_col);
void printSelectedOption(char selected_option);
void readPacientInfoWithHyperterminal(void);
void readField(char prompt[], int* field);
void computeThresholds(void);

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
                monitors.temperature += rand_value + actuators.temperature;
                rand_value_expiration_temp -= 1;
            } else {
                rand_value_expiration_temp = 7;
                rand_value = (float)(rand() % 2 - rand() % 2)
                             / (CAN_msg.magnitude_order);
            }
            CAN_msg.temperature_raw = (int)(monitors.temperature * CAN_msg.magnitude_order);
        }
        
        if(oxygen_sat_monitor_activated){
            // The random value is the same for $rand_value_expiration_oxygen cycles
            if(rand_value_expiration_oxygen > 0){
                monitors.oxygen_sat += rand_value + actuators.oxygen_sat;
                if(monitors.oxygen_sat > 100) monitors.oxygen_sat = 100;
                rand_value_expiration_oxygen -= 1;
            } else {
                rand_value_expiration_oxygen = 3;
                rand_value = (float)(rand() % 2 - rand() % 2)
                                    / (CAN_msg.magnitude_order);
            }
            CAN_msg.oxygen_sat_raw = (int) (monitors.oxygen_sat * CAN_msg.magnitude_order);
        }
        
        if(temperature_monitor_activated || oxygen_sat_monitor_activated){
            char *null;
            OSSignalMsg(MSG_FOR_CONTROL, (OStypeMsgP) null);
            
            while(!CANtransmissionCompleted()){
                OS_WaitBinSem(BINSEM_SEND_CAN_MSG, OSNO_TIMEOUT);
            }
            CANsendMessage(EXTERNAL_MONITORS_DATA_SID,
                           (unsigned char *) &CAN_msg,
                           sizeof(temp_ox_pkt_t));
        }
        
        OS_Delay(MONITORS_SAMPLING_PERIOD);
	}
}

void TaskUserInterface(void){
    static char printed = 0, update = 0, reset = 0;
    static int cursor_min_row = 8, num_options = 4, cursor_col = 2,
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
        OSSignalBinSem(BINSEM_SEND_CAN_MSG);
        OS_Delay(INPUT_SCAN_PERIOD);
	}
}

void TaskControl(void){
    static tens_glyc_act_pkt_t actuators_pkt;
    actuators_pkt.magnitude_order = 10;
    OStypeMsgP msgP;
    
    while(1){
        OS_WaitMsg(MSG_FOR_CONTROL, &msgP, OSNO_TIMEOUT);
        
        if(tension_monitor_activated && (monitors.tension < thresholds.tension_lower_limit || monitors.tension > thresholds.tension_upper_limit)){
            actuators.tension += monitors.tension > thresholds.tension_upper_limit  ? -0.1 : 0.1;
            OSSetEFlag(EFLAG_FOR_SHOW_ACTUATORS_STATUS, FLAG_TENSION_ACTUATOR);
        
        } else {
            actuators.tension = 0;
        }
            
        if(glycemia_monitor_activated && (monitors.glycemia < thresholds.glycemia_lower_limit || monitors.glycemia > thresholds.glycemia_upper_limit)){
            actuators.glycemia += monitors.glycemia > thresholds.glycemia_upper_limit  ? -0.1 : 0.1;
            OSSetEFlag(EFLAG_FOR_SHOW_ACTUATORS_STATUS, FLAG_GLYCEMIA_ACTUATOR);
            
        } else {
            actuators.glycemia = 0;
        }
        
        if(temperature_monitor_activated && (monitors.temperature < thresholds.temperature_lower_limit || monitors.temperature > thresholds.temperature_upper_limit)){
            actuators.temperature += monitors.temperature > thresholds.temperature_upper_limit  ? -0.1 : 0.1;
            OSSetEFlag(EFLAG_FOR_SHOW_ACTUATORS_STATUS, FLAG_TEMPERATURE_ACTUATOR);
            
        } else {
            actuators.temperature = 0;
        }
        
        if(oxygen_sat_monitor_activated && (monitors.oxygen_sat < thresholds.oxygen_sat_lower_limit || monitors.oxygen_sat > thresholds.oxygen_sat_upper_limit)){
            actuators.oxygen_sat += monitors.oxygen_sat > thresholds.oxygen_sat_upper_limit  ? -0.1 : 0.1;
            OSSetEFlag(EFLAG_FOR_SHOW_ACTUATORS_STATUS, FLAG_OXYGEN_SAT_ACTUATOR);
            
        } else {
            actuators.oxygen_sat = 0;
        }
        
        LCDClear();
        
        actuators_pkt.tension_act_raw = (int) (actuators.tension * actuators_pkt.magnitude_order);
        actuators_pkt.glycemia_act_raw = (int) (actuators.glycemia * actuators_pkt.magnitude_order);
        
        while(!CANtransmissionCompleted()){
            OS_WaitBinSem(BINSEM_SEND_CAN_MSG, OSNO_TIMEOUT);
        }
        CANsendMessage(CONTROL_DATA_SID, (unsigned char *) &actuators_pkt, sizeof(tens_glyc_act_pkt_t));
    }
}

void TaskShowActuatorsStatus(void){
    char eFlag;
    
    while(1){
        OSClrEFlag(EFLAG_FOR_SHOW_ACTUATORS_STATUS, maskEventForShowActuatorsStatus);
        OS_WaitEFlag(EFLAG_FOR_SHOW_ACTUATORS_STATUS, maskEventForShowActuatorsStatus, OSANY_BITS, OSNO_TIMEOUT);
        eFlag = OSReadEFlag(EFLAG_FOR_SHOW_ACTUATORS_STATUS);
        
        LCDClear();
        LCDMoveHome();
        LCDPrint("Suministrando...");
        LCDMoveSecondLine();
		if (eFlag & FLAG_TENSION_ACTUATOR){
            LCDPrint("enal. ");
        }
		if (eFlag & FLAG_GLYCEMIA_ACTUATOR){
            LCDPrint("ins. ");
        }
        if (eFlag & FLAG_TEMPERATURE_ACTUATOR){
            LCDPrint("pcmol. ");
        }
        if (eFlag & FLAG_OXYGEN_SAT_ACTUATOR){
            LCDPrint("oxyg.");
        }
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
                monitors.tension = (float)((tens_glyc_pkt_t*) rxMsgData)->tension_raw /
                                ((temp_ox_pkt_t*) rxMsgData)->magnitude_order;
                monitors.glycemia = (float)((tens_glyc_pkt_t*) rxMsgData)->glycemia_raw /
                                  ((temp_ox_pkt_t*) rxMsgData)->magnitude_order;
                char *null;
                OSSignalMsg(MSG_FOR_CONTROL, (OStypeMsgP) null);
                break;
        }
	}
}

/******************************************************************************/
/* Procedures implementation                                                  */
/******************************************************************************/
void readMicrocontrollerKeyboard(void){
    char key = getKeyNotBlocking();
    
    if(key != NO_BUTTON_PRESSED) while(!CANtransmissionCompleted());
    switch(key){
        case TENSION_KEY:
            tension_monitor_activated = !tension_monitor_activated;
            toggleLED(TENSION_LED);
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
    TermPrint("[ ] Umbrales");TermNewLine();
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
    TermMove(13,0);
    int i;
    for (i = 0; i < 10; i++) {
        TermPrint("                                             ");
        TermNewLine();
    }
    while(BusyUART1());
    TermMove(13,0);

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
                sprintf(buff,"Tension:\t%.2f mmHg", monitors.tension);
                TermPrint(buff); TermNewLine();
            }
            if(glycemia_monitor_activated){
                sprintf(buff,"Glucemia:\t%.2f g/mL", monitors.glycemia);
                TermPrint(buff); TermNewLine();   
            }
            if(temperature_monitor_activated){
                sprintf(buff,"Temperatura:\t%.2f C", monitors.temperature);
                TermPrint(buff); TermNewLine();
            }
            if(oxygen_sat_monitor_activated){
                sprintf(buff,"Sat. oxigeno:\t%.2f %%", monitors.oxygen_sat);
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
            break;
        case 3:
            sprintf(buff,"Maxima tension:\t\t\t%.1f", thresholds.tension_upper_limit);
            TermPrint(buff);TermNewLine();
            sprintf(buff,"Minima tension:\t\t\t%.1f", thresholds.tension_lower_limit);
            TermPrint(buff);TermNewLine();
            sprintf(buff,"Maxima glucemia:\t\t%.1f", thresholds.glycemia_upper_limit);
            TermPrint(buff);TermNewLine();
            sprintf(buff,"Minima glucemia:\t\t%.1f", thresholds.glycemia_lower_limit);
            TermPrint(buff);TermNewLine();
            sprintf(buff,"Maxima temperatura:\t\t%.1f", thresholds.temperature_upper_limit);
            TermPrint(buff);TermNewLine();
            sprintf(buff,"Minima temperatura:\t\t%.1f", thresholds.temperature_lower_limit);
            TermPrint(buff);TermNewLine();
            sprintf(buff,"Maxima saturacion de oxigeno:\t%.1f", thresholds.oxygen_sat_upper_limit);
            TermPrint(buff);TermNewLine();
            sprintf(buff,"Minima saturacion de oxigeno:\t%.1f", thresholds.oxygen_sat_lower_limit);
            TermPrint(buff);TermNewLine();
            break;
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

void readPacientInfoWithHyperterminal(void){
    TermClear();
    TermPrint("All-in-one Health Monitor v1.0.2");TermNewLine();
    TermNewLine();
    readField("Introduce ID: ", &pacientInfo.pacient_id);
    readField("Introduce edad: ", &pacientInfo.age);
    readField("Introduce genero [1-Hombre, 0-Mujer]: ", &pacientInfo.genre);
    readField("Introduce altura: ", &pacientInfo.height);
    readField("Introduce peso: ", &pacientInfo.weight);
    readField("Fumador? [1-Si, 0-No]: ", &pacientInfo.smoker);
    readField("Diabetico? [1-Si, 0-No]: ", &pacientInfo.diabetic);
}

void computeThresholds(void){
    float tension_thresholds_ponderation, glycemia_thresholds_ponderation;
    tension_thresholds_ponderation =
            0.2 * ((float)pacientInfo.age/120) +
            0.2 * ((float)pacientInfo.height/230) +
            0.3 * ((float)pacientInfo.weight/200) +
            0.2 * ((float)pacientInfo.smoker) +
            0.1 * ((float)pacientInfo.diabetic);
    
    glycemia_thresholds_ponderation =
            0.15 * ((float)pacientInfo.age/120) +
            0.05 * ((float)pacientInfo.height/230) +
            0.2  * ((float)pacientInfo.weight/200) +
            0.1  * ((float)pacientInfo.smoker) +
            0.5  * ((float)pacientInfo.diabetic);
    
    thresholds.tension_upper_limit = 180 * tension_thresholds_ponderation;
    thresholds.tension_lower_limit = 120 * tension_thresholds_ponderation;
    
    thresholds.glycemia_upper_limit = 400 * glycemia_thresholds_ponderation;
    thresholds.glycemia_lower_limit = 100 * glycemia_thresholds_ponderation;
    
    thresholds.oxygen_sat_upper_limit = 100;
    if (pacientInfo.smoker) thresholds.oxygen_sat_lower_limit = 95;
    else thresholds.oxygen_sat_lower_limit = 98;
    
    thresholds.temperature_upper_limit = 38;
    thresholds.temperature_lower_limit = 35;
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
    
    // Create event flag (ecbP, efcbP, initial value)
    OSCreateEFlag(EFLAG_FOR_SHOW_ACTUATORS_STATUS, EFLAG_FOR_SHOW_ACTUATORS_STATUS_EFCB, iniValueEventShowActuatorsStatus);
    
    // Create mailbox
	OSCreateMsg(MSG_FOR_CONTROL, (OStypeMsgP) 0);
    
    // Create binary semaphores (pointer, initial value)
	OSCreateBinSem(BINSEM_SEND_CAN_MSG, 1);
    
	// =============================================
	// Enable peripherals that trigger interrupts
	// =============================================
	Timer1Init(TIMER_PERIOD_FOR_125ms, TIMER_PSCALER_FOR_125ms, 4);
	Timer1Start();
    
    LCDClear();
    LCDMoveHome();
    LCDPrint("Introduce datos");
    LCDMoveSecondLine();
    LCDPrint("para continuar...");
    readPacientInfoWithHyperterminal();
    LCDClear();
    
//    pacientInfo.pacient_id = 3525;
//    pacientInfo.age = 85;
//    pacientInfo.genre = 0;
//    pacientInfo.height = 190;
//    pacientInfo.weight = 65;
//    pacientInfo.smoker = 1;
//    pacientInfo.diabetic = 0;
    
    computeThresholds();

	// =============================================
	// Enter multitasking environment
	// =============================================
	while(1){
        OSSched();
	}
}
