#include <stdio.h>
#include "../include/can_frame.h"
#include "../include/can_crc.h"

int main(void)
{
    /* CAN frame bits: ID=0x100, RTR=0, IDE=0, DLC=3, Data=AA BB CC */
    uint8_t bits[] = {
        0,                                  /* SOF */
        0,0,1,0,0,0,0,0,0,0,0,             /* ID = 0x100 */
        0,                                  /* RTR */
        0,                                  /* IDE */
        0,0,1,1,                            /* DLC = 3 */
        1,0,1,0,1,0,1,0,                    /* data[0] = 0xAA */
        1,0,1,1,1,0,1,1,                    /* data[1] = 0xBB */
        1,1,0,0,1,1,0,0                     /* data[2] = 0xCC */
    };

    /* Calculate CRC over SOF + ID + RTR + IDE + DLC + DATA */
    int num_bits = sizeof(bits);
    uint16_t crc = can_crc_calculate(bits, num_bits);
    printf("Calculated CRC: 0x%04X\n", crc);

    /* Verify: should match */
    if (can_crc_verify(bits, num_bits, crc) == 0) {
        printf("CRC verify: PASS\n");
    } else {
        printf("CRC verify: FAIL\n");
    }

    /* Flip one bit to simulate corruption */
    bits[5] = !bits[5];
    uint16_t bad_crc = can_crc_calculate(bits, num_bits);
    printf("Corrupted CRC: 0x%04X\n", bad_crc);

    if (can_crc_verify(bits, num_bits, crc) == 0) {
        printf("Corruption test: FAIL (should have caught it!)\n");
    } else {
        printf("Corruption test: PASS (error detected!)\n");
    }

    return 0;
}
