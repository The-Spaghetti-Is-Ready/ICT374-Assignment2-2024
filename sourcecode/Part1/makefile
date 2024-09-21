CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -MMD -MP

INCLUDE_DIR = include
BUILD_DIR = out

SOURCES = $(wildcard *.c)
OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(SOURCES))
DEPS = $(OBJS:.o=.d) #make .d files based on .o files 

#build directory settings
$(BUILD_DIR)/%.o: %.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

#EXECUTABLE BUILD SETTINGS
all: $(OBJS)
	$(CC) $(CFLAGS) -o a2-part1 $(OBJS)

.PHONY: clean

#clean object and dependency files
clean:
	rm -f $(BUILD_DIR)/*.o a2-part1
	rm -f $(BUILD_DIR)/*.d a2-part1

#include dependencies
-include $(DEPS)
