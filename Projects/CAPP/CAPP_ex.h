


#define CAPP_MOTORDIAGNOSTICS_CAN_ID (0x100)
typedef struct {
   float vehicle_speed;
   uint8_t drive_state;
} CAPP_MotorDiagnostics_t;


// Function Prototypes
void CAPP_Tx_MotorDiagnostics(CAPP_MotorDiagnostics_t* CAPP_MotorDiagnostics);
void CAPP_Rx_MotorDiagnostics(CAPP_MotorDiagnostics_t* CAPP_MotorDiagnostics);

