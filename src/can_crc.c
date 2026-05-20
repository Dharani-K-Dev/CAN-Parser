#include "../include/can_crc.h"

uint16_t can_crc_calculate(const uint8_t *bits, int num_bits)
{
    uint16_t crc = 0;

    for (int i = 0; i < num_bits; i++) {
        int top_bit = (crc >> 14) & 1;
        crc = crc << 1;
        crc = crc | bits[i];
        if (top_bit) {
            crc = crc ^ CAN_CRC_POLY;
        }
        crc = crc & 0x7FFF;
    }

    return crc;
}

int can_crc_verify(const uint8_t *bits, int num_bits, uint16_t expected_crc)
{
    uint16_t calculated = can_crc_calculate(bits, num_bits);

    if (calculated == expected_crc) {
        return 0;
    } else {
        return -1;
    }
}
