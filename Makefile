BUILD = src/main.cpp src/read.cpp
TARGET = build/mm
CFLAGS = -std=c++17 -Wall -Wextra -Werror -pedantic -O3
CC = g++

.PHONY: all test clean

all: $(BUILD)
	@mkdir -p build
	$(CC) $(CFLAGS) $(BUILD) -o $(TARGET)

test:
	make all
	./$(TARGET) test -d

clean:
	rm -rf build