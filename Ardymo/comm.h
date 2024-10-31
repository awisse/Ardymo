/* Functions for I2C communication */
#pragma once
#include "defines.h"
#include "stdint.h"

#ifdef USE_I2C
void I2C_SlaveReceive(int16_t n); // Slave: Passed to Wire.onReceive()
void I2C_MasterRequest(void); // Slave: Passed to Wire.onRequest()
uint8_t master_receive(uint8_t n); // Master: Receive from slave into buffer
#endif

void receive_bytes(uint8_t*, size_t n); // Read data from recv_buffer
void send_bytes(uint8_t*, uint8_t n); // Put data into send_buffer

bool Received(void); // Return true if new data received via I2C

// vim: ft=cpp
