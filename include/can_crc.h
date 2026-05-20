#ifndef CAN_CRC_H
#define CAN_CRC_H

#include <stdint.h>

#define CAN_CRC_POLY  0x4599

uint16_t can_crc_calculate(const uint8_t *bits, int num_bits);
int can_crc_verify(const uint8_t *bits, int num_bits, uint16_t expected_crc);

#endif
