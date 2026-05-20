#include <stdio.h>
#include "../include/can_stuff.h"

int main(void)
{
    /* Input: 6 ones followed by 6 zeros followed by 1 */
    uint8_t input[] = {1,1,1,1,1,1,0,0,0,0,0,0,1};
    uint8_t stuffed[CAN_STUFF_MAX_BITS];
    uint8_t unstuffed[CAN_STUFF_MAX_BITS];

    /* Stuff */
    int stuffed_len = can_stuff_bits(input, 13, stuffed);
    printf("Original (%2d bits): ", 13);
    for (int i = 0; i < 13; i++) printf("%d", input[i]);
    printf("\n");

    printf("Stuffed  (%2d bits): ", stuffed_len);
    for (int i = 0; i < stuffed_len; i++) printf("%d", stuffed[i]);
    printf("\n");

    /* Unstuff */
    int unstuffed_len = can_unstuff_bits(stuffed, stuffed_len, unstuffed);
    printf("Unstuffed(%2d bits): ", unstuffed_len);
    for (int i = 0; i < unstuffed_len; i++) printf("%d", unstuffed[i]);
    printf("\n");

    /* Check: unstuffed should match original */
    int match = 1;
    if (unstuffed_len != 13) match = 0;
    for (int i = 0; i < 13 && match; i++) {
        if (unstuffed[i] != input[i]) match = 0;
    }
    printf("Round-trip test: %s\n", match ? "PASS" : "FAIL");

    return 0;
}
