CC = gcc
CFLAGS = -Wall -Iinclude

SRC = src/can_frame.c src/can_crc.c src/can_stuff.c src/can_arbitration.c
TEST = tests/test_can.c

all: test_can

test_can: $(SRC) $(TEST)
	$(CC) $(CFLAGS) -o test_can $(TEST) $(SRC)

test: test_can
	./test_can

clean:
	rm -f test_can

.PHONY: all test clean
