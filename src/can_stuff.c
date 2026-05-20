#include "../include/can_stuff.h"

int can_stuff_bits(const uint8_t *in, int in_len, uint8_t *out)
{
    int out_len = 0;
    int count = 1;

    /* Copy first bit directly */
    out[out_len] = in[0];
    out_len++;

    /* Process remaining bits */
    for (int i = 1; i < in_len; i++) {
        /* Check: is this bit same as previous? */
        if (in[i] == in[i - 1]) {
            count++;
        } else {
            count = 1;
        }

        /* Copy current bit to output */
        out[out_len] = in[i];
        out_len++;

        /* If 5 consecutive same bits, stuff opposite bit */
        if (count == 5) {
            out[out_len] = !in[i];
            out_len++;
            count = 1;
        }
    }

    return out_len;
}

int can_unstuff_bits(const uint8_t *in, int in_len, uint8_t *out)
{
    int out_len = 0;
    int count = 1;

    /* Copy first bit directly */
    out[out_len] = in[0];
    out_len++;

    for (int i = 1; i < in_len; i++) {
        if (in[i] == in[i - 1]) {
            count++;
        } else {
            count = 1;
        }

        /* If count reaches 6, that's a stuff error! */
        if (count >= 6) {
            return -1;
        }

        /* Copy current bit to output */
        out[out_len] = in[i];
        out_len++;

        /* If 5 consecutive same bits, next bit is stuffed - skip it */
        if (count == 5) {
            i++;
            count = 1;
        }
    }

    return out_len;
}
