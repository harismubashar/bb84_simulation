# Makefile

TARGET = bb84_sim

SOURCE = bb84_sim.c

CC = gcc

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) -o $(TARGET) $(SOURCE)

clean:
	rm -f $(TARGET)

.PHONY: all clean
