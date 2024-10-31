/* Functions for I2C communication
 * Description of functions in comm.h
 */
#include "string.h"
#include "comm.h"
#include "platform.h"

uint8_t recv_buffer[32]; // Bytes received
uint8_t send_buffer[32]; // Bytes to send

static uint8_t n_bytes_to_send {};
static bool received {false}; // True if new data received from master
static bool available {false};// True if new sensor data available to send

#ifdef USE_I2C
// Passed to Wire.onReceive()
void I2C_SlaveReceive(int16_t n) {
  uint8_t r_bytes;
  Platform::slave_receive(recv_buffer, n);
  received = true;
}
// Passed to Wire.onRequest()
void I2C_MasterRequest(void) {
  if (available) {
    Platform::slave_send(send_buffer, n_bytes_to_send);
    memset(send_buffer, 0, 32);
    available = false;
  } else {
    // Send clear bogus data for master to know not to use
    memset(send_buffer, 0xFF, 32); // Notably: collision will be 0xFF
    Platform::slave_send(send_buffer, n_bytes_to_send);
    memset(send_buffer, 0, 32);
  }
}

uint8_t master_receive(uint8_t n) {
  received = true;
  return Platform::master_receive(recv_buffer, n);
}

void receive_bytes(uint8_t* dest, size_t n) {
  if (n > 32) {
    n = 32;
  }
  memcpy(dest, recv_buffer, n);
  received = false;
}

void send_bytes(uint8_t* src, uint8_t n) {
  // Slave only. `master_send` can be called directly.
  if (n > 32) {
    n = 32;
  }
  memcpy(send_buffer, src, n);
  n_bytes_to_send = n;
  available = true;
}

bool Received(void) {
  return received;
}
#endif
// vim: ft=cpp
