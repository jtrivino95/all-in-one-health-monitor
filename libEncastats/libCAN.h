
// CAN operational modes
#define LOOPBACK_MODE	0b010
#define NORMAL_MODE		0b000


void CANinit (unsigned char operationalMode, unsigned char enableRxInt, unsigned char enableTxInt, unsigned int filterSID, unsigned int maskSID);

unsigned char CANrxInt ();
unsigned char CANtxInt ();
void CANclearGlobalInt ();
void CANclearRxInt ();
void CANclearTxInt ();

void CANsendMessage (unsigned int sid, unsigned char * data, unsigned char datalen);
unsigned char CANtransmissionCompleted ();

unsigned int CANreadRxMessageSID (void);
unsigned int CANreadRxMessageDLC (void);
void CANreadRxMessageDATA (unsigned char * data);
void CANclearRxBuffer (void);
unsigned char CANreceptionCompleted ();