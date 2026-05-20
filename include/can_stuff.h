#ifndef CAN_STUFF_H
#define CAN_STUFF_H

#include <stdint.h>

#define CAN_STUFF_MAX_BITS  256

/*
 * Stuff bits: insert opposite bit after 5 consecutive same bits.
 * Returns number of bits in output array.
 */
int can_stuff_bits(const uint8_t *in, int in_len, uint8_t *out);

/*
 * Unstuff bits: remove stuffed bits.
 * Returns number of bits in output array.
 * Returns -1 if stuff error detected (6+ same bits in a row).
 */
int can_unstuff_bits(const uint8_t *in, int in_len, uint8_t *out);

#endif
