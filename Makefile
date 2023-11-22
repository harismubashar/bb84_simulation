# Makefile

TARGET = bb84_sim

SOURCE = bb84_sim.c

CC = gcc
CFLAGS = -Wall -Wextra

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

clean:
	rm -f $(TARGET)

.PHONY: all clean
