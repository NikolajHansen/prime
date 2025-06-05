CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lm
TARGET = build/prime
SOURCES = prime.c
OBJECTS = build/$(SOURCES:.c=.o)
BUILD_DIR = build

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

build/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean