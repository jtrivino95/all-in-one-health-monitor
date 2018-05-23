/*
 * Analog to digital converter library
 *
 * It works with the first potentiometer, which is connected to the 10-bit ADC.
 * Consequently the values that can be obtained are between 0 and 1023.
 *
 */

#define CAD_INTERACTION_BY_POLLING		0
#define CAD_INTERACTION_BY_INTERRUPT	1

void CADInit(unsigned char interactionType, unsigned char intPriority);

void CADStart(unsigned char interactionType);

void CADStop();

void CADRequestValue();

unsigned char CADReadyValue();

unsigned int CADGetValue();

void CADClearInt();