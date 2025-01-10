// ***WARNING***
// THIS FILE IS AUTO-GENERATED
// ANY MANUAL CHANGES HERE WILL BE OVERWRITTEN

#include <stdint.h>
#include "CAPP.h"

// Local variables
MotorDiagnostics_t MotorDiagnostics = {0};
MCBDiagnostics_t MCBDiagnostics = {0};

uint8_t extract_uint8_t(uint8_t* data, uint8_t start_byte)
{
   union {
      uint8_t bytes[1];
      uint8_t value;
   } converter;

   for (int i = 0; i < 1; i++) {
      converter.bytes[i] = data[start_byte + i];
   }

   return converter.value;
}

int8_t extract_int8_t(uint8_t* data, uint8_t start_byte)
{
   union {
      uint8_t bytes[1];
      int8_t value;
   } converter;

   for (int i = 0; i < 1; i++) {
      converter.bytes[i] = data[start_byte + i];
   }

   return converter.value;
}

uint16_t extract_uint16_t(uint8_t* data, uint8_t start_byte)
{
   union {
      uint8_t bytes[2];
      uint16_t value;
   } converter;

   for (int i = 0; i < 2; i++) {
      converter.bytes[i] = data[start_byte + i];
   }

   return converter.value;
}

int16_t extract_int16_t(uint8_t* data, uint8_t start_byte)
{
   union {
      uint8_t bytes[2];
      int16_t value;
   } converter;

   for (int i = 0; i < 2; i++) {
      converter.bytes[i] = data[start_byte + i];
   }

   return converter.value;
}

uint32_t extract_uint32_t(uint8_t* data, uint8_t start_byte)
{
   union {
      uint8_t bytes[4];
      uint32_t value;
   } converter;

   for (int i = 0; i < 4; i++) {
      converter.bytes[i] = data[start_byte + i];
   }

   return converter.value;
}

int32_t extract_int32_t(uint8_t* data, uint8_t start_byte)
{
   union {
      uint8_t bytes[4];
      int32_t value;
   } converter;

   for (int i = 0; i < 4; i++) {
      converter.bytes[i] = data[start_byte + i];
   }

   return converter.value;
}

uint64_t extract_uint64_t(uint8_t* data, uint8_t start_byte)
{
   union {
      uint8_t bytes[8];
      uint64_t value;
   } converter;

   for (int i = 0; i < 8; i++) {
      converter.bytes[i] = data[start_byte + i];
   }

   return converter.value;
}

int64_t extract_int64_t(uint8_t* data, uint8_t start_byte)
{
   union {
      uint8_t bytes[8];
      int64_t value;
   } converter;

   for (int i = 0; i < 8; i++) {
      converter.bytes[i] = data[start_byte + i];
   }

   return converter.value;
}

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

double extract_double(uint8_t* data, uint8_t start_byte)
{
   union {
      uint8_t bytes[8];
      double value;
   } converter;

   for (int i = 0; i < 8; i++) {
      converter.bytes[i] = data[start_byte + i];
   }

   return converter.value;
}

void CAPP_Parse_MotorDiagnostics(uint8_t* data)
{
   MotorDiagnostics.signal = extract_float(data, 0);
   MotorDiagnostics.signal = extract_uint8_t(data, 4);
}

void CAPP_Parse_MCBDiagnostics(uint8_t* data)
{
   MCBDiagnostics.signal = extract_float(data, 0);
   MCBDiagnostics.signal = extract_uint8_t(data, 4);
}

void CAPP_Rx_CAN_Message(uint8_t CAN_ID, uint8_t* data)
{
   switch(CAN_ID)
   {
      case(MOTORDIAGNOSTICS_CAN_ID):
         CAPP_Parse_MotorDiagnostics(data);
         break;
      case(MCBDIAGNOSTICS_CAN_ID):
         CAPP_Parse_MCBDiagnostics(data);
         break;
   }
}
