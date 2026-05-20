#include "../include/can_arbitration.h"

int can_arbitrate(const uint16_t *ids, int count)
{
    uint8_t alive[CAN_MAX_NODES];
    int alive_count = count;

    for (int i = 0; i < count; i++) {
        alive[i] = 1;
    }

    for (int bit = 10; bit >= 0; bit--) {
        int bus = 1;

        for (int i = 0; i < count; i++) {
            if (alive[i]) {
                int node_bit = (ids[i] >> bit) & 1;
                if (node_bit == 0) {
                    bus = 0;
                }
            }
        }

        for (int i = 0; i < count; i++) {
            if (alive[i]) {
                int node_bit = (ids[i] >> bit) & 1;
                if (node_bit == 1 && bus == 0) {
                    alive[i] = 0;
                    alive_count--;
                }
            }
        }

        if (alive_count == 1) {
            break;
        }
    }

    for (int i = 0; i < count; i++) {
        if (alive[i]) {
            return i;
        }
    }

    return 0;
}
