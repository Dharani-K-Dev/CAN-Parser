#ifndef CAN_ARBITRATION_H
#define CAN_ARBITRATION_H

#include <stdint.h>

#define CAN_MAX_NODES  8

int can_arbitrate(const uint16_t *ids, int count);

#endif
