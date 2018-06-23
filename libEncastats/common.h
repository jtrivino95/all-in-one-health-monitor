typedef struct TempAndOxygenPacket {
    int temperature_raw, oxygen_sat_raw;
    int magnitude_order;
} temp_ox_pkt_t;

typedef struct TensAndGlycPacket {
    int tension_raw, glycemia_raw;
    int magnitude_order;
} tens_glyc_pkt_t;

typedef struct TensAndGlycActuatorValues {
    int tension_act_raw, glycemia_act_raw;
    int magnitude_order;
} tens_glyc_act_pkt_t;

#define MONITORS_SAMPLING_PERIOD            4 // ticks (1 tick = 125 ms)
#define INPUT_SCAN_PERIOD                   4 // ticks (1 tick = 125 ms)

// CAN package identifiers
#define EXTERNAL_MONITORS_DATA_SID             0
#define TENSION_MONITOR_STATUS_MSG_SID         1
#define GLYCEMIA_MONITOR_STATUS_MSG_SID        2
#define TEMPERATURE_MONITOR_STATUS_MSG_SID     3
#define OXYGEN_SAT_MONITOR_STATUS_MSG_SID      4
#define CONTROL_DATA_SID                       5
