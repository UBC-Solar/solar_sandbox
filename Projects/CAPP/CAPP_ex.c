// ***WARNING***
// THIS FILE IS AUTO-GENERATED
// ANY MANUAL CHANGES HERE WILL BE OVERWRITTEN

// Includes
#include <stdint.h>
#include "CAPP.h"

// Local copies
CAPP_MotorDiagnostics_t CAPP_MotorDiagnostics;

float extract_float(uint8_t* data, uint8_t start_byte) 
{
   union {
      uint8_t bytes[4];
      float value;
   } converter;

   for (int i = 0; i < 4; i++) {
      converter.bytes[i] = data[start_byte + i];
   }

   return converter.value;
}

void CAPP_Parse_CAPP_MotorDiagnostics(uint8_t* CAN_data)
{
   CAPP_MotorDiagnostics.vehicle_speed = extract_float(CAN_data, 0);
}


void CAPP_Rx_CAN_Message(uint8_t CAN_ID, uint8_t* CAN_data)
{
   switch (CAN_ID) 
   {
      case(CAPP_MOTORDIAGNOSTICS_CAN_ID):
         CAPP_Parse_MotorDiagnostics(CAN_data);
   }
}
