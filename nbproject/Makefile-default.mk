#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=cof
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/all-in-one-health-monitor.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/all-in-one-health-monitor.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=main.c libEncastats/libCAD.c libEncastats/libCAN.c libEncastats/libKEYB.c libEncastats/libLCD.c libEncastats/libLEDs.c libEncastats/libTIMER.c libSalvo/salvohook_interrupt_MCC30_IRQ.c libSalvo/salvohook_wdt_MCC30_clrwdt.c libSalvo/salvomem.c libEncastats/delay.s main_planta.c main_control.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/main.o ${OBJECTDIR}/libEncastats/libCAD.o ${OBJECTDIR}/libEncastats/libCAN.o ${OBJECTDIR}/libEncastats/libKEYB.o ${OBJECTDIR}/libEncastats/libLCD.o ${OBJECTDIR}/libEncastats/libLEDs.o ${OBJECTDIR}/libEncastats/libTIMER.o ${OBJECTDIR}/libSalvo/salvohook_interrupt_MCC30_IRQ.o ${OBJECTDIR}/libSalvo/salvohook_wdt_MCC30_clrwdt.o ${OBJECTDIR}/libSalvo/salvomem.o ${OBJECTDIR}/libEncastats/delay.o ${OBJECTDIR}/main_planta.o ${OBJECTDIR}/main_control.o
POSSIBLE_DEPFILES=${OBJECTDIR}/main.o.d ${OBJECTDIR}/libEncastats/libCAD.o.d ${OBJECTDIR}/libEncastats/libCAN.o.d ${OBJECTDIR}/libEncastats/libKEYB.o.d ${OBJECTDIR}/libEncastats/libLCD.o.d ${OBJECTDIR}/libEncastats/libLEDs.o.d ${OBJECTDIR}/libEncastats/libTIMER.o.d ${OBJECTDIR}/libSalvo/salvohook_interrupt_MCC30_IRQ.o.d ${OBJECTDIR}/libSalvo/salvohook_wdt_MCC30_clrwdt.o.d ${OBJECTDIR}/libSalvo/salvomem.o.d ${OBJECTDIR}/libEncastats/delay.o.d ${OBJECTDIR}/main_planta.o.d ${OBJECTDIR}/main_control.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/main.o ${OBJECTDIR}/libEncastats/libCAD.o ${OBJECTDIR}/libEncastats/libCAN.o ${OBJECTDIR}/libEncastats/libKEYB.o ${OBJECTDIR}/libEncastats/libLCD.o ${OBJECTDIR}/libEncastats/libLEDs.o ${OBJECTDIR}/libEncastats/libTIMER.o ${OBJECTDIR}/libSalvo/salvohook_interrupt_MCC30_IRQ.o ${OBJECTDIR}/libSalvo/salvohook_wdt_MCC30_clrwdt.o ${OBJECTDIR}/libSalvo/salvomem.o ${OBJECTDIR}/libEncastats/delay.o ${OBJECTDIR}/main_planta.o ${OBJECTDIR}/main_control.o

# Source Files
SOURCEFILES=main.c libEncastats/libCAD.c libEncastats/libCAN.c libEncastats/libKEYB.c libEncastats/libLCD.c libEncastats/libLEDs.c libEncastats/libTIMER.c libSalvo/salvohook_interrupt_MCC30_IRQ.c libSalvo/salvohook_wdt_MCC30_clrwdt.c libSalvo/salvomem.c libEncastats/delay.s main_planta.c main_control.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/all-in-one-health-monitor.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=30F4011
MP_LINKER_FILE_OPTION=,--script="gld\p30F4011.gld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/libEncastats/delay.o: libEncastats/delay.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/libEncastats" 
	@${RM} ${OBJECTDIR}/libEncastats/delay.o.d 
	@${RM} ${OBJECTDIR}/libEncastats/delay.o.ok ${OBJECTDIR}/libEncastats/delay.o.err 
	@${RM} ${OBJECTDIR}/libEncastats/delay.o 
	@${FIXDEPS} "${OBJECTDIR}/libEncastats/delay.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_AS} $(MP_EXTRA_AS_PRE)  libEncastats/delay.s -o ${OBJECTDIR}/libEncastats/delay.o -omf=coff -p=$(MP_PROCESSOR_OPTION) --defsym=__MPLAB_BUILD=1 --defsym=__MPLAB_DEBUG=1 --defsym=__ICD2RAM=1 --defsym=__DEBUG=1 --defsym=__MPLAB_DEBUGGER_ICD3=1 -g  -MD "${OBJECTDIR}/libEncastats/delay.o.d"$(MP_EXTRA_AS_POST)
	
else
${OBJECTDIR}/libEncastats/delay.o: libEncastats/delay.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/libEncastats" 
	@${RM} ${OBJECTDIR}/libEncastats/delay.o.d 
	@${RM} ${OBJECTDIR}/libEncastats/delay.o.ok ${OBJECTDIR}/libEncastats/delay.o.err 
	@${RM} ${OBJECTDIR}/libEncastats/delay.o 
	@${FIXDEPS} "${OBJECTDIR}/libEncastats/delay.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_AS} $(MP_EXTRA_AS_PRE)  libEncastats/delay.s -o ${OBJECTDIR}/libEncastats/delay.o -omf=coff -p=$(MP_PROCESSOR_OPTION) --defsym=__MPLAB_BUILD=1 -g  -MD "${OBJECTDIR}/libEncastats/delay.o.d"$(MP_EXTRA_AS_POST)
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o.ok ${OBJECTDIR}/main.o.err 
	@${RM} ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"libSalvo" -I"libEncastats" -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    
	
${OBJECTDIR}/libEncastats/libCAD.o: libEncastats/libCAD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/libEncastats" 
	@${RM} ${OBJECTDIR}/libEncastats/libCAD.o.d 
	@${RM} ${OBJECTDIR}/libEncastats/libCAD.o.ok ${OBJECTDIR}/libEncastats/libCAD.o.err 
	@${RM} ${OBJECTDIR}/libEncastats/libCAD.o 
	@${FIXDEPS} "${OBJECTDIR}/libEncastats/libCAD.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"libSalvo" -I"libEncastats" -MMD -MF "${OBJECTDIR}/libEncastats/libCAD.o.d" -o ${OBJECTDIR}/libEncastats/libCAD.o libEncastats/libCAD.c    
	
${OBJECTDIR}/libEncastats/libCAN.o: libEncastats/libCAN.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/libEncastats" 
	@${RM} ${OBJECTDIR}/libEncastats/libCAN.o.d 
	@${RM} ${OBJECTDIR}/libEncastats/libCAN.o.ok ${OBJECTDIR}/libEncastats/libCAN.o.err 
	@${RM} ${OBJECTDIR}/libEncastats/libCAN.o 
	@${FIXDEPS} "${OBJECTDIR}/libEncastats/libCAN.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"libSalvo" -I"libEncastats" -MMD -MF "${OBJECTDIR}/libEncastats/libCAN.o.d" -o ${OBJECTDIR}/libEncastats/libCAN.o libEncastats/libCAN.c    
	
${OBJECTDIR}/libEncastats/libKEYB.o: libEncastats/libKEYB.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/libEncastats" 
	@${RM} ${OBJECTDIR}/libEncastats/libKEYB.o.d 
	@${RM} ${OBJECTDIR}/libEncastats/libKEYB.o.ok ${OBJECTDIR}/libEncastats/libKEYB.o.err 
	@${RM} ${OBJECTDIR}/libEncastats/libKEYB.o 
	@${FIXDEPS} "${OBJECTDIR}/libEncastats/libKEYB.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"libSalvo" -I"libEncastats" -MMD -MF "${OBJECTDIR}/libEncastats/libKEYB.o.d" -o ${OBJECTDIR}/libEncastats/libKEYB.o libEncastats/libKEYB.c    
	
${OBJECTDIR}/libEncastats/libLCD.o: libEncastats/libLCD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/libEncastats" 
	@${RM} ${OBJECTDIR}/libEncastats/libLCD.o.d 
	@${RM} ${OBJECTDIR}/libEncastats/libLCD.o.ok ${OBJECTDIR}/libEncastats/libLCD.o.err 
	@${RM} ${OBJECTDIR}/libEncastats/libLCD.o 
	@${FIXDEPS} "${OBJECTDIR}/libEncastats/libLCD.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"libSalvo" -I"libEncastats" -MMD -MF "${OBJECTDIR}/libEncastats/libLCD.o.d" -o ${OBJECTDIR}/libEncastats/libLCD.o libEncastats/libLCD.c    
	
${OBJECTDIR}/libEncastats/libLEDs.o: libEncastats/libLEDs.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/libEncastats" 
	@${RM} ${OBJECTDIR}/libEncastats/libLEDs.o.d 
	@${RM} ${OBJECTDIR}/libEncastats/libLEDs.o.ok ${OBJECTDIR}/libEncastats/libLEDs.o.err 
	@${RM} ${OBJECTDIR}/libEncastats/libLEDs.o 
	@${FIXDEPS} "${OBJECTDIR}/libEncastats/libLEDs.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"libSalvo" -I"libEncastats" -MMD -MF "${OBJECTDIR}/libEncastats/libLEDs.o.d" -o ${OBJECTDIR}/libEncastats/libLEDs.o libEncastats/libLEDs.c    
	
${OBJECTDIR}/libEncastats/libTIMER.o: libEncastats/libTIMER.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/libEncastats" 
	@${RM} ${OBJECTDIR}/libEncastats/libTIMER.o.d 
	@${RM} ${OBJECTDIR}/libEncastats/libTIMER.o.ok ${OBJECTDIR}/libEncastats/libTIMER.o.err 
	@${RM} ${OBJECTDIR}/libEncastats/libTIMER.o 
	@${FIXDEPS} "${OBJECTDIR}/libEncastats/libTIMER.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"libSalvo" -I"libEncastats" -MMD -MF "${OBJECTDIR}/libEncastats/libTIMER.o.d" -o ${OBJECTDIR}/libEncastats/libTIMER.o libEncastats/libTIMER.c    
	
${OBJECTDIR}/libSalvo/salvohook_interrupt_MCC30_IRQ.o: libSalvo/salvohook_interrupt_MCC30_IRQ.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/libSalvo" 
	@${RM} ${OBJECTDIR}/libSalvo/salvohook_interrupt_MCC30_IRQ.o.d 
	@${RM} ${OBJECTDIR}/libSalvo/salvohook_interrupt_MCC30_IRQ.o.ok ${OBJECTDIR}/libSalvo/salvohook_interrupt_MCC30_IRQ.o.err 
	@${RM} ${OBJECTDIR}/libSalvo/salvohook_interrupt_MCC30_IRQ.o 
	@${FIXDEPS} "${OBJECTDIR}/libSalvo/salvohook_interrupt_MCC30_IRQ.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"libSalvo" -I"libEncastats" -MMD -MF "${OBJECTDIR}/libSalvo/salvohook_interrupt_MCC30_IRQ.o.d" -o ${OBJECTDIR}/libSalvo/salvohook_interrupt_MCC30_IRQ.o libSalvo/salvohook_interrupt_MCC30_IRQ.c    
	
${OBJECTDIR}/libSalvo/salvohook_wdt_MCC30_clrwdt.o: libSalvo/salvohook_wdt_MCC30_clrwdt.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/libSalvo" 
	@${RM} ${OBJECTDIR}/libSalvo/salvohook_wdt_MCC30_clrwdt.o.d 
	@${RM} ${OBJECTDIR}/libSalvo/salvohook_wdt_MCC30_clrwdt.o.ok ${OBJECTDIR}/libSalvo/salvohook_wdt_MCC30_clrwdt.o.err 
	@${RM} ${OBJECTDIR}/libSalvo/salvohook_wdt_MCC30_clrwdt.o 
	@${FIXDEPS} "${OBJECTDIR}/libSalvo/salvohook_wdt_MCC30_clrwdt.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"libSalvo" -I"libEncastats" -MMD -MF "${OBJECTDIR}/libSalvo/salvohook_wdt_MCC30_clrwdt.o.d" -o ${OBJECTDIR}/libSalvo/salvohook_wdt_MCC30_clrwdt.o libSalvo/salvohook_wdt_MCC30_clrwdt.c    
	
${OBJECTDIR}/libSalvo/salvomem.o: libSalvo/salvomem.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/libSalvo" 
	@${RM} ${OBJECTDIR}/libSalvo/salvomem.o.d 
	@${RM} ${OBJECTDIR}/libSalvo/salvomem.o.ok ${OBJECTDIR}/libSalvo/salvomem.o.err 
	@${RM} ${OBJECTDIR}/libSalvo/salvomem.o 
	@${FIXDEPS} "${OBJECTDIR}/libSalvo/salvomem.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"libSalvo" -I"libEncastats" -MMD -MF "${OBJECTDIR}/libSalvo/salvomem.o.d" -o ${OBJECTDIR}/libSalvo/salvomem.o libSalvo/salvomem.c    
	
${OBJECTDIR}/main_planta.o: main_planta.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main_planta.o.d 
	@${RM} ${OBJECTDIR}/main_planta.o.ok ${OBJECTDIR}/main_planta.o.err 
	@${RM} ${OBJECTDIR}/main_planta.o 
	@${FIXDEPS} "${OBJECTDIR}/main_planta.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"libSalvo" -I"libEncastats" -MMD -MF "${OBJECTDIR}/main_planta.o.d" -o ${OBJECTDIR}/main_planta.o main_planta.c    
	
${OBJECTDIR}/main_control.o: main_control.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main_control.o.d 
	@${RM} ${OBJECTDIR}/main_control.o.ok ${OBJECTDIR}/main_control.o.err 
	@${RM} ${OBJECTDIR}/main_control.o 
	@${FIXDEPS} "${OBJECTDIR}/main_control.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"libSalvo" -I"libEncastats" -MMD -MF "${OBJECTDIR}/main_control.o.d" -o ${OBJECTDIR}/main_control.o main_control.c    
	
else
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o.ok ${OBJECTDIR}/main.o.err 
	@${RM} ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"libSalvo" -I"libEncastats" -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    
	
${OBJECTDIR}/libEncastats/libCAD.o: libEncastats/libCAD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/libEncastats" 
	@${RM} ${OBJECTDIR}/libEncastats/libCAD.o.d 
	@${RM} ${OBJECTDIR}/libEncastats/libCAD.o.ok ${OBJECTDIR}/libEncastats/libCAD.o.err 
	@${RM} ${OBJECTDIR}/libEncastats/libCAD.o 
	@${FIXDEPS} "${OBJECTDIR}/libEncastats/libCAD.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"libSalvo" -I"libEncastats" -MMD -MF "${OBJECTDIR}/libEncastats/libCAD.o.d" -o ${OBJECTDIR}/libEncastats/libCAD.o libEncastats/libCAD.c    
	
${OBJECTDIR}/libEncastats/libCAN.o: libEncastats/libCAN.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/libEncastats" 
	@${RM} ${OBJECTDIR}/libEncastats/libCAN.o.d 
	@${RM} ${OBJECTDIR}/libEncastats/libCAN.o.ok ${OBJECTDIR}/libEncastats/libCAN.o.err 
	@${RM} ${OBJECTDIR}/libEncastats/libCAN.o 
	@${FIXDEPS} "${OBJECTDIR}/libEncastats/libCAN.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"libSalvo" -I"libEncastats" -MMD -MF "${OBJECTDIR}/libEncastats/libCAN.o.d" -o ${OBJECTDIR}/libEncastats/libCAN.o libEncastats/libCAN.c    
	
${OBJECTDIR}/libEncastats/libKEYB.o: libEncastats/libKEYB.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/libEncastats" 
	@${RM} ${OBJECTDIR}/libEncastats/libKEYB.o.d 
	@${RM} ${OBJECTDIR}/libEncastats/libKEYB.o.ok ${OBJECTDIR}/libEncastats/libKEYB.o.err 
	@${RM} ${OBJECTDIR}/libEncastats/libKEYB.o 
	@${FIXDEPS} "${OBJECTDIR}/libEncastats/libKEYB.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"libSalvo" -I"libEncastats" -MMD -MF "${OBJECTDIR}/libEncastats/libKEYB.o.d" -o ${OBJECTDIR}/libEncastats/libKEYB.o libEncastats/libKEYB.c    
	
${OBJECTDIR}/libEncastats/libLCD.o: libEncastats/libLCD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/libEncastats" 
	@${RM} ${OBJECTDIR}/libEncastats/libLCD.o.d 
	@${RM} ${OBJECTDIR}/libEncastats/libLCD.o.ok ${OBJECTDIR}/libEncastats/libLCD.o.err 
	@${RM} ${OBJECTDIR}/libEncastats/libLCD.o 
	@${FIXDEPS} "${OBJECTDIR}/libEncastats/libLCD.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"libSalvo" -I"libEncastats" -MMD -MF "${OBJECTDIR}/libEncastats/libLCD.o.d" -o ${OBJECTDIR}/libEncastats/libLCD.o libEncastats/libLCD.c    
	
${OBJECTDIR}/libEncastats/libLEDs.o: libEncastats/libLEDs.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/libEncastats" 
	@${RM} ${OBJECTDIR}/libEncastats/libLEDs.o.d 
	@${RM} ${OBJECTDIR}/libEncastats/libLEDs.o.ok ${OBJECTDIR}/libEncastats/libLEDs.o.err 
	@${RM} ${OBJECTDIR}/libEncastats/libLEDs.o 
	@${FIXDEPS} "${OBJECTDIR}/libEncastats/libLEDs.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"libSalvo" -I"libEncastats" -MMD -MF "${OBJECTDIR}/libEncastats/libLEDs.o.d" -o ${OBJECTDIR}/libEncastats/libLEDs.o libEncastats/libLEDs.c    
	
${OBJECTDIR}/libEncastats/libTIMER.o: libEncastats/libTIMER.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/libEncastats" 
	@${RM} ${OBJECTDIR}/libEncastats/libTIMER.o.d 
	@${RM} ${OBJECTDIR}/libEncastats/libTIMER.o.ok ${OBJECTDIR}/libEncastats/libTIMER.o.err 
	@${RM} ${OBJECTDIR}/libEncastats/libTIMER.o 
	@${FIXDEPS} "${OBJECTDIR}/libEncastats/libTIMER.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"libSalvo" -I"libEncastats" -MMD -MF "${OBJECTDIR}/libEncastats/libTIMER.o.d" -o ${OBJECTDIR}/libEncastats/libTIMER.o libEncastats/libTIMER.c    
	
${OBJECTDIR}/libSalvo/salvohook_interrupt_MCC30_IRQ.o: libSalvo/salvohook_interrupt_MCC30_IRQ.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/libSalvo" 
	@${RM} ${OBJECTDIR}/libSalvo/salvohook_interrupt_MCC30_IRQ.o.d 
	@${RM} ${OBJECTDIR}/libSalvo/salvohook_interrupt_MCC30_IRQ.o.ok ${OBJECTDIR}/libSalvo/salvohook_interrupt_MCC30_IRQ.o.err 
	@${RM} ${OBJECTDIR}/libSalvo/salvohook_interrupt_MCC30_IRQ.o 
	@${FIXDEPS} "${OBJECTDIR}/libSalvo/salvohook_interrupt_MCC30_IRQ.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"libSalvo" -I"libEncastats" -MMD -MF "${OBJECTDIR}/libSalvo/salvohook_interrupt_MCC30_IRQ.o.d" -o ${OBJECTDIR}/libSalvo/salvohook_interrupt_MCC30_IRQ.o libSalvo/salvohook_interrupt_MCC30_IRQ.c    
	
${OBJECTDIR}/libSalvo/salvohook_wdt_MCC30_clrwdt.o: libSalvo/salvohook_wdt_MCC30_clrwdt.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/libSalvo" 
	@${RM} ${OBJECTDIR}/libSalvo/salvohook_wdt_MCC30_clrwdt.o.d 
	@${RM} ${OBJECTDIR}/libSalvo/salvohook_wdt_MCC30_clrwdt.o.ok ${OBJECTDIR}/libSalvo/salvohook_wdt_MCC30_clrwdt.o.err 
	@${RM} ${OBJECTDIR}/libSalvo/salvohook_wdt_MCC30_clrwdt.o 
	@${FIXDEPS} "${OBJECTDIR}/libSalvo/salvohook_wdt_MCC30_clrwdt.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"libSalvo" -I"libEncastats" -MMD -MF "${OBJECTDIR}/libSalvo/salvohook_wdt_MCC30_clrwdt.o.d" -o ${OBJECTDIR}/libSalvo/salvohook_wdt_MCC30_clrwdt.o libSalvo/salvohook_wdt_MCC30_clrwdt.c    
	
${OBJECTDIR}/libSalvo/salvomem.o: libSalvo/salvomem.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/libSalvo" 
	@${RM} ${OBJECTDIR}/libSalvo/salvomem.o.d 
	@${RM} ${OBJECTDIR}/libSalvo/salvomem.o.ok ${OBJECTDIR}/libSalvo/salvomem.o.err 
	@${RM} ${OBJECTDIR}/libSalvo/salvomem.o 
	@${FIXDEPS} "${OBJECTDIR}/libSalvo/salvomem.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"libSalvo" -I"libEncastats" -MMD -MF "${OBJECTDIR}/libSalvo/salvomem.o.d" -o ${OBJECTDIR}/libSalvo/salvomem.o libSalvo/salvomem.c    
	
${OBJECTDIR}/main_planta.o: main_planta.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main_planta.o.d 
	@${RM} ${OBJECTDIR}/main_planta.o.ok ${OBJECTDIR}/main_planta.o.err 
	@${RM} ${OBJECTDIR}/main_planta.o 
	@${FIXDEPS} "${OBJECTDIR}/main_planta.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"libSalvo" -I"libEncastats" -MMD -MF "${OBJECTDIR}/main_planta.o.d" -o ${OBJECTDIR}/main_planta.o main_planta.c    
	
${OBJECTDIR}/main_control.o: main_control.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main_control.o.d 
	@${RM} ${OBJECTDIR}/main_control.o.ok ${OBJECTDIR}/main_control.o.err 
	@${RM} ${OBJECTDIR}/main_control.o 
	@${FIXDEPS} "${OBJECTDIR}/main_control.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=coff -x c -c -mcpu=$(MP_PROCESSOR_OPTION) -I"libSalvo" -I"libEncastats" -MMD -MF "${OBJECTDIR}/main_control.o.d" -o ${OBJECTDIR}/main_control.o main_control.c    
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/all-in-one-health-monitor.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  libSalvo/MCC30/libsalvofmcc30s-t.a  gld/p30F4011.gld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -omf=coff -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -o dist/${CND_CONF}/${IMAGE_TYPE}/all-in-one-health-monitor.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}   libSalvo\MCC30\libsalvofmcc30s-t.a      -Wl,--defsym=__MPLAB_BUILD=1,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1
else
dist/${CND_CONF}/${IMAGE_TYPE}/all-in-one-health-monitor.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  libSalvo/MCC30/libsalvofmcc30s-t.a gld/p30F4011.gld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -omf=coff -mcpu=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/all-in-one-health-monitor.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}   libSalvo\MCC30\libsalvofmcc30s-t.a      -Wl,--defsym=__MPLAB_BUILD=1,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION)
	${MP_CC_DIR}\\pic30-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/all-in-one-health-monitor.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -omf=coff
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
