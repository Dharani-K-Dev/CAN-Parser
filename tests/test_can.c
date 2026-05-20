#include <stdio.h>
#include <string.h>
#include "../include/can_frame.h"
#include "../include/can_crc.h"
#include "../include/can_stuff.h"
#include "../include/can_arbitration.h"

static int tests_passed = 0;
static int tests_total = 0;

static void check(const char *name, int condition)
{
    tests_total++;
    if (condition) {
        printf("  PASS: %s\n", name);
        tests_passed++;
    } else {
        printf("  FAIL: %s\n", name);
    }
}

static void test_frame_parse(void)
{
    printf("\n[Frame Parser]\n");
    uint8_t bits[] = {
        0,
        0,0,1,0,0,0,0,0,0,0,0,
        0,
        0,
        0,0,1,1,
        1,0,1,0,1,0,1,0,
        1,0,1,1,1,0,1,1,
        1,1,0,0,1,1,0,0,
        0,1,0,0,1,1,0,0,1,1,0,1,1,0,0
    };

    can_frame_t frame;
    can_frame_parse(bits, sizeof(bits), &frame);

    check("ID is 0x100", frame.id == 0x100);
    check("RTR is 0", frame.rtr == 0);
    check("DLC is 3", frame.dlc == 3);
    check("data[0] is 0xAA", frame.data[0] == 0xAA);
    check("data[1] is 0xBB", frame.data[1] == 0xBB);
    check("data[2] is 0xCC", frame.data[2] == 0xCC);
}

static void test_crc(void)
{
    printf("\n[CRC-15]\n");
    uint8_t bits[] = {
        0,
        0,0,1,0,0,0,0,0,0,0,0,
        0,
        0,
        0,0,1,1,
        1,0,1,0,1,0,1,0,
        1,0,1,1,1,0,1,1,
        1,1,0,0,1,1,0,0
    };

    uint16_t crc = can_crc_calculate(bits, sizeof(bits));
    check("CRC calculates non-zero", crc != 0);
    check("CRC verify matches", can_crc_verify(bits, sizeof(bits), crc) == 0);

    bits[5] = !bits[5];
    check("CRC detects corruption", can_crc_verify(bits, sizeof(bits), crc) != 0);
}

static void test_stuffing(void)
{
    printf("\n[Bit Stuffing]\n");
    uint8_t input[] = {1,1,1,1,1,1,0,0,0,0,0,0,1};
    uint8_t stuffed[256];
    uint8_t unstuffed[256];

    int slen = can_stuff_bits(input, 13, stuffed);
    check("Stuffing adds 2 bits", slen == 15);

    int ulen = can_unstuff_bits(stuffed, slen, unstuffed);
    check("Unstuffing recovers length", ulen == 13);

    int match = 1;
    for (int i = 0; i < 13; i++) {
        if (unstuffed[i] != input[i]) match = 0;
    }
    check("Round-trip data matches", match == 1);
}

static void test_arbitration(void)
{
    printf("\n[Arbitration]\n");
    uint16_t ids[] = {0x200, 0x100, 0x500};
    check("Lowest ID wins (0x100 at index 1)", can_arbitrate(ids, 3) == 1);

    uint16_t ids2[] = {0x7FF, 0x000};
    check("0x000 beats 0x7FF", can_arbitrate(ids2, 2) == 1);
}

int main(void)
{
    printf("=== CAN Parser Test Suite ===\n");

    test_frame_parse();
    test_crc();
    test_stuffing();
    test_arbitration();

    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_total);

    return (tests_passed == tests_total) ? 0 : 1;
}
