BUILD = src/main.cpp src/read.cpp
TARGET = build/mm
CFLAGS = -std=c++11 -Wall -Wextra -Werror -pedantic -O3
CC = g++

.PHONY: all test clean

all: $(BUILD)
	@mkdir -p build
	$(CC) $(CFLAGS) $(BUILD) -o $(TARGET)

test:
	make all
	./$(TARGET)

clean:
	rm -rf build