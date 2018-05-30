typedef struct PacientStatus {
    unsigned int tension, glycemia, temperature, oxygen_sat;
} pacient_status_t;

#define MONITORS_SAMPLING_PERIOD            4 // ticks (1 tick = 125 ms)

// CAN package identifiers
#define EXTERNAL_MONITORS_DATA_SID     0
