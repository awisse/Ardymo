#pragma once
#include "defines.h"
#include "stdint.h"

#ifdef USE_I2C
void I2C_SlaveReceive(int16_t n);
void I2C_MasterRequest(void);
#endif

uint8_t master_receive(uint8_t n);
void receive_bytes(uint8_t*, size_t n);
void send_bytes(uint8_t*, uint8_t n);

bool Received(void);

// vim: ft=cpp
