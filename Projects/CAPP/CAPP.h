// ***WARNING***
// THIS FILE IS AUTO-GENERATED
// ANY MANUAL CHANGES HERE WILL BE OVERWRITTEN

#define MOTORDIAGNOSTICS_CAN_ID (0x100)
typedef struct {
   float vehicle_speed;
   uint8_t drive_state;
} CAPP_MotorDiagnostics_t;

#define MCBDIAGNOSTICS_CAN_ID (0x101)
typedef struct {
   float vehicle_speed;
   uint8_t drive_state;
} CAPP_MCBDiagnostics_t;

#define AMB_DIAGNOSTICS_CAN_ID (0x300)
typedef struct {
   float temperature1;
   bool temp_fail1;
   bool temp_fail2;
} CAPP_AMB_Diagnostics_t;

