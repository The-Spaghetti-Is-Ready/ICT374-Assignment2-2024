CC = gcc
CFLAGS = -Wextra -Wall -Wpedantic

INCLUDE_DIR = include
BUILD_DIR = out

SOURCES = *.c
LIBS = $(INCLUDE_DIR)/*.h


#EXECUTABLE BUILD SETTINGS
a2-part1: main.o commandfunctions.o
	$(CC) $(CFLAGS) -o a2-part1 $(BUILD_DIR)/main.o $(BUILD_DIR)/commandfunctions.o

$(BUILD_DIR)/main.o: $(SOURCES) $(LIBS)

$(BUILD_DIR)/commandfunctions.o: commandfunctions.c $(LIBS)

#if no build dir present, it will make it
$(BUILD_DIR):
	mkdir -p $@