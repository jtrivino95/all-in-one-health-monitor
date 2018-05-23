
/* ================================ */
/*	Libreria LEDs					*/
/* ================================ */

#define NUMLEDS	6

void initLEDs();
void onLED(unsigned char numLED);
void offLED(unsigned char numLED);
void toggleLED(unsigned char numLED);
void printNumInLED(unsigned char number);