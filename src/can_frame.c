#include <stdio.h>
#include "../include/can_frame.h"

int can_frame_parse(const uint8_t *bits, int num_bits, can_frame_t *frame)
{
    int pos = 0;

    /* Step 1: Skip SOF (1 bit) */
    pos = 1;

    /* Step 2: Extract ID (11 bits) */
    frame->id = 0;
    for (int i = 0; i < 11; i++) {
        frame->id = (frame->id << 1) | bits[pos];
        pos++;
    }

    /* Step 3: Extract RTR (1 bit) */
    frame->rtr = bits[pos];
    pos++;

    /* Step 4: Extract IDE (1 bit) */
    frame->ide = bits[pos];
    pos++;

    /* Step 5: Extract DLC (4 bits) */
    frame->dlc = 0;
    for (int i = 0; i < 4; i++) {
        frame->dlc = (frame->dlc << 1) | bits[pos];
        pos++;
    }

    /* Step 6: Extract DATA (dlc x 8 bits) */
    for (int i = 0; i < frame->dlc; i++) {
        frame->data[i] = 0;
        for (int j = 0; j < 8; j++) {
            frame->data[i] = (frame->data[i] << 1) | bits[pos];
            pos++;
        }
    }

    /* Step 7: Extract CRC (15 bits) */
    frame->crc = 0;
    for (int i = 0; i < 15; i++) {
        frame->crc = (frame->crc << 1) | bits[pos];
        pos++;
    }

    return 0;
}

void can_frame_print(const can_frame_t *frame)
{

    printf("ID:  0x%03X\n", frame->id);
    printf("RTR: %u\n", frame->rtr);
    printf("IDE: %u\n", frame->ide);
    printf("DLC: %u\n", frame->dlc);
    printf("Data:");
    for (int i = 0; i < frame->dlc; i++) {
        printf(" 0x%02X", frame->data[i]);
    }
    printf("\n");
    printf("CRC: 0x%04X\n", frame->crc);
}
