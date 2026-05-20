#ifndef CAN_FRAME_H
#define CAN_FRAME_H

#include <stdint.h>

#define CAN_MAX_DATA_BYTES  8
#define CAN_ID_MAX          0x7FF

typedef struct {
    uint16_t id;
    uint8_t  rtr;
    uint8_t  ide;
    uint8_t  dlc;
    uint8_t  data[CAN_MAX_DATA_BYTES];
    uint16_t crc;
} can_frame_t;

int can_frame_parse(const uint8_t *bits, int num_bits, can_frame_t *frame);
void can_frame_print(const can_frame_t *frame);

#endif
