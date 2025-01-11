// ***WARNING***
// THIS FILE IS AUTO-GENERATED
// ANY MANUAL CHANGES HERE WILL BE OVERWRITTEN

// Includes
#include <stdint.h>
#include <stdbool.h>
#include "CAPP.h"

#define MASK(bits) ((1ULL << (bits)) - 1)

uint8_t CAPP_parse_uint8_t(uint8_t* buffer, uint8_t buf_len, uint8_t start_bit)
{
   union {
      uint8_t bytes[8];
      uint64_t value;
   } buffer_u;

   union {
      uint8_t bytes[1];
      uint8_t value;
   } data;

   for (uint8_t i = 0; i < buf_len; i++) {
      buffer_u.bytes[i] = buffer[i];
   }

   buffer_u.value = (buffer_u.value >> start_bit) & (MASK(8));

   for (int i = 0; i < 1; i++) {
      data.bytes[i] = buffer_u.bytes[i];
   }

   return data.value;
}

void CAPP_package_uint8_t(uint8_t* buffer, uint8_t buf_len, uint8_t start_bit, uint8_t value)
{
   union {
      uint8_t bytes[8];
      uint64_t value;
   } buffer_u;

   union {
      uint64_t buffer;
      uint8_t value;
   } data = {0};

   for (uint8_t i = 0; i < buf_len; i++) {
      buffer_u.bytes[i] = buffer[i];
   }

   data.value = value;

   buffer_u.value = buffer_u.value | ((data.buffer << start_bit));

   for (int i = 0; i < 8; i++) {
      buffer[i] = buffer_u.bytes[i];
   }
}


void CAPP_Parse_MotorDiagnostics(MotorDiagnostics_t* MotorDiagnostics, uint8_t* CAN_data)
{
   MotorDiagnostics->vehicle_speed = parse_float(CAN_data, 0);
}