// ***WARNING***
// THIS FILE IS AUTO-GENERATED
// ANY MANUAL CHANGES HERE WILL BE OVERWRITTEN

#include <stdint.h>
#include <stdbool.h>
#include "CAPP.h"

// Local variables

bool CAPP_parse_bool(uint8_t* buffer, uint8_t buf_len, uint8_t start_bit)
{
   union {
      uint8_t bytes[8];
      uint64_t value;
   } buffer_u;

   union {
      uint8_t bytes[1];
      bool value;
   } data;

   for (uint8_t i = 0; i < buf_len; i++) {
      buffer_u.bytes[i] = buffer[i];
   }

   buffer_u.value = (buffer_u.value >> start_bit) & (MASK(1));

   for (int i = 0; i < 1; i++) {
      data.bytes[i] = buffer_u.bytes[i];
   }

   return data.value;
}


void CAPP_package_bool(uint8_t* buffer, uint8_t buf_len, uint8_t start_bit, bool value)
{
   union {
      uint8_t bytes[8];
      uint64_t value;
   } buffer_u;

   union {
      uint64_t buffer;
      bool value;
   } data = {0};

   for (uint8_t i = 0; i < buf_len; i++) {
      buffer_u.bytes[i] = buffer[i];
   }

   data.value = value;

   buffer_u.value = buffer_u.value | ((data.buffer << start_bit));

   for (int i = 0; i < 1; i++) {
      buffer[i] = buffer_u.bytes[i];
   }
}

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

   for (int i = 0; i < 1; i++) {
      buffer[i] = buffer_u.bytes[i];
   }
}

int8_t CAPP_parse_int8_t(uint8_t* buffer, uint8_t buf_len, uint8_t start_bit)
{
   union {
      uint8_t bytes[8];
      uint64_t value;
   } buffer_u;

   union {
      uint8_t bytes[1];
      int8_t value;
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


void CAPP_package_int8_t(uint8_t* buffer, uint8_t buf_len, uint8_t start_bit, int8_t value)
{
   union {
      uint8_t bytes[8];
      uint64_t value;
   } buffer_u;

   union {
      uint64_t buffer;
      int8_t value;
   } data = {0};

   for (uint8_t i = 0; i < buf_len; i++) {
      buffer_u.bytes[i] = buffer[i];
   }

   data.value = value;

   buffer_u.value = buffer_u.value | ((data.buffer << start_bit));

   for (int i = 0; i < 1; i++) {
      buffer[i] = buffer_u.bytes[i];
   }
}

uint16_t CAPP_parse_uint16_t(uint8_t* buffer, uint8_t buf_len, uint8_t start_bit)
{
   union {
      uint8_t bytes[8];
      uint64_t value;
   } buffer_u;

   union {
      uint8_t bytes[1];
      uint16_t value;
   } data;

   for (uint8_t i = 0; i < buf_len; i++) {
      buffer_u.bytes[i] = buffer[i];
   }

   buffer_u.value = (buffer_u.value >> start_bit) & (MASK(16));

   for (int i = 0; i < 2; i++) {
      data.bytes[i] = buffer_u.bytes[i];
   }

   return data.value;
}


void CAPP_package_uint16_t(uint8_t* buffer, uint8_t buf_len, uint8_t start_bit, uint16_t value)
{
   union {
      uint8_t bytes[8];
      uint64_t value;
   } buffer_u;

   union {
      uint64_t buffer;
      uint16_t value;
   } data = {0};

   for (uint8_t i = 0; i < buf_len; i++) {
      buffer_u.bytes[i] = buffer[i];
   }

   data.value = value;

   buffer_u.value = buffer_u.value | ((data.buffer << start_bit));

   for (int i = 0; i < 2; i++) {
      buffer[i] = buffer_u.bytes[i];
   }
}

int16_t CAPP_parse_int16_t(uint8_t* buffer, uint8_t buf_len, uint8_t start_bit)
{
   union {
      uint8_t bytes[8];
      uint64_t value;
   } buffer_u;

   union {
      uint8_t bytes[1];
      int16_t value;
   } data;

   for (uint8_t i = 0; i < buf_len; i++) {
      buffer_u.bytes[i] = buffer[i];
   }

   buffer_u.value = (buffer_u.value >> start_bit) & (MASK(16));

   for (int i = 0; i < 2; i++) {
      data.bytes[i] = buffer_u.bytes[i];
   }

   return data.value;
}


void CAPP_package_int16_t(uint8_t* buffer, uint8_t buf_len, uint8_t start_bit, int16_t value)
{
   union {
      uint8_t bytes[8];
      uint64_t value;
   } buffer_u;

   union {
      uint64_t buffer;
      int16_t value;
   } data = {0};

   for (uint8_t i = 0; i < buf_len; i++) {
      buffer_u.bytes[i] = buffer[i];
   }

   data.value = value;

   buffer_u.value = buffer_u.value | ((data.buffer << start_bit));

   for (int i = 0; i < 2; i++) {
      buffer[i] = buffer_u.bytes[i];
   }
}

uint32_t CAPP_parse_uint32_t(uint8_t* buffer, uint8_t buf_len, uint8_t start_bit)
{
   union {
      uint8_t bytes[8];
      uint64_t value;
   } buffer_u;

   union {
      uint8_t bytes[1];
      uint32_t value;
   } data;

   for (uint8_t i = 0; i < buf_len; i++) {
      buffer_u.bytes[i] = buffer[i];
   }

   buffer_u.value = (buffer_u.value >> start_bit) & (MASK(32));

   for (int i = 0; i < 4; i++) {
      data.bytes[i] = buffer_u.bytes[i];
   }

   return data.value;
}


void CAPP_package_uint32_t(uint8_t* buffer, uint8_t buf_len, uint8_t start_bit, uint32_t value)
{
   union {
      uint8_t bytes[8];
      uint64_t value;
   } buffer_u;

   union {
      uint64_t buffer;
      uint32_t value;
   } data = {0};

   for (uint8_t i = 0; i < buf_len; i++) {
      buffer_u.bytes[i] = buffer[i];
   }

   data.value = value;

   buffer_u.value = buffer_u.value | ((data.buffer << start_bit));

   for (int i = 0; i < 4; i++) {
      buffer[i] = buffer_u.bytes[i];
   }
}

int32_t CAPP_parse_int32_t(uint8_t* buffer, uint8_t buf_len, uint8_t start_bit)
{
   union {
      uint8_t bytes[8];
      uint64_t value;
   } buffer_u;

   union {
      uint8_t bytes[1];
      int32_t value;
   } data;

   for (uint8_t i = 0; i < buf_len; i++) {
      buffer_u.bytes[i] = buffer[i];
   }

   buffer_u.value = (buffer_u.value >> start_bit) & (MASK(32));

   for (int i = 0; i < 4; i++) {
      data.bytes[i] = buffer_u.bytes[i];
   }

   return data.value;
}


void CAPP_package_int32_t(uint8_t* buffer, uint8_t buf_len, uint8_t start_bit, int32_t value)
{
   union {
      uint8_t bytes[8];
      uint64_t value;
   } buffer_u;

   union {
      uint64_t buffer;
      int32_t value;
   } data = {0};

   for (uint8_t i = 0; i < buf_len; i++) {
      buffer_u.bytes[i] = buffer[i];
   }

   data.value = value;

   buffer_u.value = buffer_u.value | ((data.buffer << start_bit));

   for (int i = 0; i < 4; i++) {
      buffer[i] = buffer_u.bytes[i];
   }
}

uint64_t CAPP_parse_uint64_t(uint8_t* buffer, uint8_t buf_len, uint8_t start_bit)
{
   union {
      uint8_t bytes[8];
      uint64_t value;
   } buffer_u;

   union {
      uint8_t bytes[1];
      uint64_t value;
   } data;

   for (uint8_t i = 0; i < buf_len; i++) {
      buffer_u.bytes[i] = buffer[i];
   }

   buffer_u.value = (buffer_u.value >> start_bit) & (MASK(64));

   for (int i = 0; i < 8; i++) {
      data.bytes[i] = buffer_u.bytes[i];
   }

   return data.value;
}


void CAPP_package_uint64_t(uint8_t* buffer, uint8_t buf_len, uint8_t start_bit, uint64_t value)
{
   union {
      uint8_t bytes[8];
      uint64_t value;
   } buffer_u;

   union {
      uint64_t buffer;
      uint64_t value;
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

int64_t CAPP_parse_int64_t(uint8_t* buffer, uint8_t buf_len, uint8_t start_bit)
{
   union {
      uint8_t bytes[8];
      uint64_t value;
   } buffer_u;

   union {
      uint8_t bytes[1];
      int64_t value;
   } data;

   for (uint8_t i = 0; i < buf_len; i++) {
      buffer_u.bytes[i] = buffer[i];
   }

   buffer_u.value = (buffer_u.value >> start_bit) & (MASK(64));

   for (int i = 0; i < 8; i++) {
      data.bytes[i] = buffer_u.bytes[i];
   }

   return data.value;
}


void CAPP_package_int64_t(uint8_t* buffer, uint8_t buf_len, uint8_t start_bit, int64_t value)
{
   union {
      uint8_t bytes[8];
      uint64_t value;
   } buffer_u;

   union {
      uint64_t buffer;
      int64_t value;
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

float CAPP_parse_float(uint8_t* buffer, uint8_t buf_len, uint8_t start_bit)
{
   union {
      uint8_t bytes[8];
      uint64_t value;
   } buffer_u;

   union {
      uint8_t bytes[1];
      float value;
   } data;

   for (uint8_t i = 0; i < buf_len; i++) {
      buffer_u.bytes[i] = buffer[i];
   }

   buffer_u.value = (buffer_u.value >> start_bit) & (MASK(32));

   for (int i = 0; i < 4; i++) {
      data.bytes[i] = buffer_u.bytes[i];
   }

   return data.value;
}


void CAPP_package_float(uint8_t* buffer, uint8_t buf_len, uint8_t start_bit, float value)
{
   union {
      uint8_t bytes[8];
      uint64_t value;
   } buffer_u;

   union {
      uint64_t buffer;
      float value;
   } data = {0};

   for (uint8_t i = 0; i < buf_len; i++) {
      buffer_u.bytes[i] = buffer[i];
   }

   data.value = value;

   buffer_u.value = buffer_u.value | ((data.buffer << start_bit));

   for (int i = 0; i < 4; i++) {
      buffer[i] = buffer_u.bytes[i];
   }
}

double CAPP_parse_double(uint8_t* buffer, uint8_t buf_len, uint8_t start_bit)
{
   union {
      uint8_t bytes[8];
      uint64_t value;
   } buffer_u;

   union {
      uint8_t bytes[1];
      double value;
   } data;

   for (uint8_t i = 0; i < buf_len; i++) {
      buffer_u.bytes[i] = buffer[i];
   }

   buffer_u.value = (buffer_u.value >> start_bit) & (MASK(64));

   for (int i = 0; i < 8; i++) {
      data.bytes[i] = buffer_u.bytes[i];
   }

   return data.value;
}


void CAPP_package_double(uint8_t* buffer, uint8_t buf_len, uint8_t start_bit, double value)
{
   union {
      uint8_t bytes[8];
      uint64_t value;
   } buffer_u;

   union {
      uint64_t buffer;
      double value;
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

void CAPP_parse_MotorDiagnostics(MotorDiagnostics_t* MotorDiagnostics, uint8_t* data);
{
   MotorDiagnostics->vehicle_speed = CAPP_parse_float(data, 8, 0);
   MotorDiagnostics->drive_state = CAPP_parse_uint8_t(data, 8, 32);
}

void CAPP_parse_MCBDiagnostics(MCBDiagnostics_t* MCBDiagnostics, uint8_t* data);
{
   MCBDiagnostics->vehicle_speed = CAPP_parse_float(data, 8, 0);
   MCBDiagnostics->drive_state = CAPP_parse_uint8_t(data, 8, 32);
}

void CAPP_parse_AMB_Diagnostics(AMB_Diagnostics_t* AMB_Diagnostics, uint8_t* data);
{
   AMB_Diagnostics->temperature1 = CAPP_parse_float(data, 8, 0);
   AMB_Diagnostics->temp_fail1 = CAPP_parse_bool(data, 8, 32);
   AMB_Diagnostics->temp_fail2 = CAPP_parse_bool(data, 8, 33);
}

void CAPP_package_MotorDiagnostics(MotorDiagnostics_t* MotorDiagnostics, uint8_t* data);
{
   CAPP_package_float(data, 8, 0, MotorDiagnostics->vehicle_speed);
   CAPP_package_uint8_t(data, 8, 32, MotorDiagnostics->drive_state);
}

void CAPP_package_MCBDiagnostics(MCBDiagnostics_t* MCBDiagnostics, uint8_t* data);
{
   CAPP_package_float(data, 8, 0, MCBDiagnostics->vehicle_speed);
   CAPP_package_uint8_t(data, 8, 32, MCBDiagnostics->drive_state);
}

void CAPP_package_AMB_Diagnostics(AMB_Diagnostics_t* AMB_Diagnostics, uint8_t* data);
{
   CAPP_package_float(data, 8, 0, AMB_Diagnostics->temperature1);
   CAPP_package_bool(data, 8, 32, AMB_Diagnostics->temp_fail1);
   CAPP_package_bool(data, 8, 33, AMB_Diagnostics->temp_fail2);
}

