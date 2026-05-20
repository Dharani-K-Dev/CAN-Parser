# CAN Protocol Parser

A bare-metal CAN 2.0A message parser written in C. Parses raw CAN bit streams, calculates CRC-15 checksums, handles bit stuffing/unstuffing, and simulates bus arbitration.

Built as a portfolio project to demonstrate deep understanding of the CAN protocol for automotive embedded roles.

## What this project demonstrates

- CAN frame parsing: extract ID, RTR, IDE, DLC, DATA, and CRC from raw bits
- CRC-15 calculation and verification using polynomial 0x4599 (ISO 11898)
- Bit stuffing and unstuffing per CAN 2.0 specification
- Non-destructive bitwise arbitration simulation
- Clean C code with modular architecture and a full test suite

## Repository layout

    can-parser/
    ├── include/
    │   ├── can_frame.h         # CAN frame struct and parse API
    │   ├── can_crc.h           # CRC-15 calculation API
    │   ├── can_stuff.h         # Bit stuffing/unstuffing API
    │   └── can_arbitration.h   # Arbitration simulation API
    ├── src/
    │   ├── can_frame.c         # Frame parser implementation
    │   ├── can_crc.c           # CRC-15 implementation
    │   ├── can_stuff.c         # Bit stuff/unstuff implementation
    │   └── can_arbitration.c   # Arbitration implementation
    ├── tests/
    │   └── test_can.c          # Test suite (14 tests)
    ├── Makefile
    └── README.md

## Build and test

Prerequisites: GCC on Linux (or WSL Ubuntu).

    make test

Expected output:

    === CAN Parser Test Suite ===

    [Frame Parser]
      PASS: ID is 0x100
      PASS: RTR is 0
      PASS: DLC is 3
      PASS: data[0] is 0xAA
      PASS: data[1] is 0xBB
      PASS: data[2] is 0xCC

    [CRC-15]
      PASS: CRC calculates non-zero
      PASS: CRC verify matches
      PASS: CRC detects corruption

    [Bit Stuffing]
      PASS: Stuffing adds 2 bits
      PASS: Unstuffing recovers length
      PASS: Round-trip data matches

    [Arbitration]
      PASS: Lowest ID wins (0x100 at index 1)
      PASS: 0x000 beats 0x7FF

    === Results: 14/14 passed ===

## What I learned building this

### CRC-15 is XOR long division

The sender calculates a 15-bit checksum using polynomial 0x4599 over the message bits. The receiver recalculates and compares. Any bit flip during transmission changes the CRC, catching the corruption. CAN's CRC-15 can detect all single-bit, double-bit, and odd-number-of-bit errors.

### Bit stuffing keeps the clock synchronized

CAN has no separate clock wire. Receivers synchronize on signal transitions (0-to-1 or 1-to-0). After 5 consecutive same bits, the sender inserts an opposite bit to guarantee a transition. The receiver knows this rule and removes the stuffed bits to recover the original data.

### Arbitration is non-destructive

When multiple nodes transmit simultaneously, each sends its ID bit by bit while reading the bus. Dominant (0) always wins over recessive (1). A node that sends recessive but reads dominant knows it lost and backs off immediately. The winning message is not damaged at all. Lower ID means higher priority, which is why safety-critical ECUs (brakes, airbags) get the lowest IDs.

## Hardware target

This is a software simulation that runs on Linux. The same parsing logic runs inside real automotive ECUs built on STM32, Infineon AURIX, and Renesas RH850 microcontrollers.

## Author

Dharani K — Embedded and Firmware Engineer

- LinkedIn: https://www.linkedin.com/in/dharani-kumaresan-embedded/
- GitHub: https://github.com/Dharani-K-Dev
