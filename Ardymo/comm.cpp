#include "string.h"
#include "comm.h"
#include "platform.h"

uint8_t recv_buffer[32]; // Bytes received
uint8_t send_buffer[32]; // Bytes to send

bool received = false;

#ifdef USE_I2C
void I2C_SlaveReceive(int16_t n) {
  uint8_t r_bytes;
  Platform::slave_receive(recv_buffer, n);
  received = true;
}
void I2C_Request(void);
#endif

void read_bytes(uint8_t* dest, uint8_t n) {
  if (n > 32) {
    n = 32;
  }
  memcpy(dest, recv_buffer, n);
  received = false;
}

void send_bytes(uint8_t* src, uint8_t n) {
  if (n > 32) {
    n = 32;
  }
}

bool Received(void) {
  return received;
}
// vim: ft=cpp
